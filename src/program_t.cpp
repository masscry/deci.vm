#include <deci.hpp>

namespace {

#define CHECK_OPCODE(SIG, OPCODE) if (strcasecmp(token, (SIG)) == 0) return (OPCODE)

  deci::opcode_t SelectOpcode(const char* token) {

    CHECK_OPCODE("nop"    ,  deci::OP_NOP    );
    CHECK_OPCODE("arg"    ,  deci::OP_ARG    );
    CHECK_OPCODE("push"   ,  deci::OP_PUSH   );
    CHECK_OPCODE("resl"   ,  deci::OP_RESULT );
    CHECK_OPCODE("drop"   ,  deci::OP_DROP   );
    CHECK_OPCODE("call"   ,  deci::OP_CALL   );
    CHECK_OPCODE("ret"    ,  deci::OP_RETURN );
    CHECK_OPCODE("bin"    ,  deci::OP_BIN    );
    CHECK_OPCODE("rval"   ,  deci::OP_RVAL   );
    CHECK_OPCODE("set"    ,  deci::OP_SET    );
    CHECK_OPCODE("unr"    ,  deci::OP_UNR    );
    CHECK_OPCODE("jz"     ,  deci::OP_JZ     );
    CHECK_OPCODE("jmp"    ,  deci::OP_JMP    );
    CHECK_OPCODE(":"      ,  deci::OP_LABEL  );
    CHECK_OPCODE("jnz"    ,  deci::OP_JNZ    );
    
    return deci::OP_UNDEFINED;
  }

  double SelectNumber(const char* token) {
    return atof(token);
  }

  deci::value_t* SelectBuiltin(const char* token) {
    CHECK_OPCODE("sum"  , deci::sum_t::Instance().Copy());
    CHECK_OPCODE("sub"  , deci::sub_t::Instance().Copy());
    CHECK_OPCODE("mul"  , deci::mul_t::Instance().Copy());
    CHECK_OPCODE("div"  , deci::div_t::Instance().Copy());
    CHECK_OPCODE( "ls"  , deci:: ls_t::Instance().Copy());
    CHECK_OPCODE( "gr"  , deci:: gr_t::Instance().Copy());
    CHECK_OPCODE( "le"  , deci:: le_t::Instance().Copy());
    CHECK_OPCODE( "ge"  , deci:: ge_t::Instance().Copy());
    CHECK_OPCODE( "eq"  , deci:: eq_t::Instance().Copy());
    CHECK_OPCODE("neq"  , deci::neq_t::Instance().Copy());
    CHECK_OPCODE( "or"  , deci:: or_t::Instance().Copy());
    CHECK_OPCODE("and"  , deci::and_t::Instance().Copy());
    CHECK_OPCODE("xor"  , deci::xor_t::Instance().Copy());
    CHECK_OPCODE("pow"  , deci::pow_t::Instance().Copy());
    CHECK_OPCODE("mod"  , deci::mod_t::Instance().Copy());
    CHECK_OPCODE("not"  , deci::not_t::Instance().Copy());
    CHECK_OPCODE("neg"  , deci::neg_t::Instance().Copy());
    CHECK_OPCODE("print", deci::print_t::Instance().Copy());
    return deci::string_t(token).Copy();
  }

  deci::value_t* SelectValue(const char* token) {
    switch(token[0]) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        return deci::number_t(SelectNumber(token)).Copy();
      default:
        return SelectBuiltin(token);
    }
  }

#undef CHECK_OPCODE

}

namespace deci
{
  static inline value_t* EvaluateCALL(const command_t& command, vm_t& vm, stack_t& local) {
    stack_t temp;
    function_t& func = *static_cast<function_t*>(command.arg);
    func.Evaluate(vm, local, temp);
    return temp.ReleaseResult();
  }

  void program_t::Evaluate(vm_t& vm, const stack_t& stack, stack_t& local) {
    size_t pc;
    size_t total = this->source.size();

#define NEXT_PC() ++pc; break
#define JUMP_PC(NPC) pc = NPC; break

    for (pc = 0; pc < total;) {
      auto command = this->source[pc];

      switch (command.opcode) {
      case OP_NOP:
        NEXT_PC();
      case OP_ARG:
      {
        number_t& num = *static_cast<number_t*>(command.arg);
        local.Push(stack.Top(static_cast<size_t>(num.Value())));
        NEXT_PC();
      }
      case OP_PUSH:
        local.Push(*command.arg);
        NEXT_PC();
      case OP_DROP:
      {
        number_t& num = *static_cast<number_t*>(command.arg);
        local.Drop(static_cast<int>(num.Value()));
        NEXT_PC();
      }
      case OP_CALL:
        this->result->Delete();
        this->result = EvaluateCALL(command, vm, local);
        NEXT_PC();
      case OP_RESULT:
        local.Push(*this->result);
        NEXT_PC();
      case OP_RETURN:
        local.Result(local.Top(0));
        NEXT_PC();
      case OP_BIN:
      {
        this->result->Delete();
        this->result = EvaluateCALL(command, vm, local);
        local.Drop(2);
        local.Push(*this->result);
        NEXT_PC();
      }
      case OP_RVAL:
      {
        const value_t& loc_value = local.Variable(*command.arg);       
        if (&loc_value != &nothing_t::Instance()) {
          local.Push(loc_value);
          NEXT_PC();
        }
        const value_t& glob_value = stack.Variable(*command.arg);
        if (&glob_value != &nothing_t::Instance()) {
          local.Push(glob_value);
          NEXT_PC();
        }
        local.Push(nothing_t::Instance());
        NEXT_PC();
      }
      case OP_SET:
      {
        local.Variable(*command.arg, local.Top(0));
        local.Drop(1);
        NEXT_PC();
      }
      case OP_UNR:
      {
        this->result->Delete();
        this->result = EvaluateCALL(command, vm, local);
        local.Drop(1);
        local.Push(*this->result);
        NEXT_PC();
      }
      case OP_JZ:
      {
        double topVal = dynamic_cast<number_t&>(local.Top(0)).Value();
        local.Drop(1);
        if (topVal == 0.0) {
          JUMP_PC(dynamic_cast<number_t&>(*command.arg).Value());
        }
        NEXT_PC();
      }
      case OP_JMP:
      {
        JUMP_PC(dynamic_cast<number_t&>(*command.arg).Value());
      }
      case OP_JNZ:
      {
        double topVal = dynamic_cast<number_t&>(local.Top(0)).Value();
        local.Drop(1);
        if (topVal != 0.0) {
          JUMP_PC(dynamic_cast<number_t&>(*command.arg).Value());
        }
        NEXT_PC();
      }
      default:
        throw std::runtime_error("Unknown Operation Code");
      }
    }
  }

  program_t::program_t(const command_t* source, size_t sourceSize)
    : refCount(0)
    , source(source, source + sourceSize)
    , result(&nothing_t::Instance()) {
      ;
  }
  
  program_t::~program_t() {
    result->Delete();
    for (auto command: this->source)
    {
      command.arg->Delete();
    }
  }

  program_t* program_t::Create(const command_t* source, size_t sourceSize) {
    program_t* result = new program_t(source, sourceSize);
    result->refCount = 1;
    return result;
  }
    
  value_t* program_t::Copy() const {
    ++this->refCount;
    return const_cast<program_t*>(this);
  }

  void program_t::Delete() {
    --this->refCount;
    if (this->refCount == 0) {
      delete this;
    }
  }

  program_t::source_t AssembleProgram(std::istream& input) {
    program_t::source_t result;
    std::unordered_map<std::string, int> labels;

    while(!input.eof()) {
      std::string opcodeToken;

      input >> opcodeToken;

      if (opcodeToken.size() == 0) {
        break;
      }

      opcode_t opcode = SelectOpcode(opcodeToken.c_str());
      switch (opcode) {
      case OP_UNDEFINED:
        throw std::runtime_error("Unknown opcode");
      case OP_RESULT:
      case OP_RETURN:
        result.push_back({ opcode, nothing_t::Instance().Copy() });
        break;
      case OP_LABEL:
        {
          std::string labelToken;
          input >> labelToken;
          labels[labelToken] = result.size();
          break;
        }
      default:
        {
          std::string argToken;
          input >> argToken;
          result.push_back({ opcode, SelectValue(argToken.c_str()) });
          break;
        }
      }
    }

    // Fix label pointers
    for (auto& item: result) {
      if (item.arg->Type() == value_t::STRING) {
        string_t& str = static_cast<string_t&>(*item.arg);
        auto it = labels.find(str.Value());
        if (it != labels.end()) {
          item.arg->Delete();
          item.arg = number_t(it->second).Copy();
        }
      }
    }

    return result;
  }

  size_t program_t::DoHashing() const {
    return std::hash<const void*>()(this);
  }

  std::string program_t::ToText() const {
    return std::string("deci::program");
  }

} // namespace deci
