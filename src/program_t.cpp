#include <deci.hpp>
#include <cstring>
#include <iostream>

namespace {

#define CHECK_OPCODE(SIG, OPCODE) if (strcmp(token, (SIG)) == 0) return (OPCODE)

  deci::opcode_t SelectOpcode(const char* token) {

    CHECK_OPCODE("nop"    ,  deci::OP_NOP    );
    CHECK_OPCODE("arg"    ,  deci::OP_ARG    );
    CHECK_OPCODE("push"   ,  deci::OP_PUSH   );
    CHECK_OPCODE("resl"   ,  deci::OP_RESULT );
    CHECK_OPCODE("drop"   ,  deci::OP_DROP   );
    CHECK_OPCODE("call"   ,  deci::OP_CALL   );
    CHECK_OPCODE("ret"    ,  deci::OP_RETURN );

    return deci::OP_UNDEFINED;
  }

  double SelectNumber(const char* token) {
    return atof(token);
  }

  deci::value_t* SelectBuiltin(const char* token) {
    CHECK_OPCODE("sum", deci::sum_t::Instance().Copy());
    CHECK_OPCODE("sub", deci::sub_t::Instance().Copy());
    CHECK_OPCODE("mul", deci::mul_t::Instance().Copy());
    CHECK_OPCODE("div", deci::div_t::Instance().Copy());
    return deci::nothing_t::Instance().Copy();
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

  void program_t::Evaluate(vm_t& vm, const stack_t& stack, stack_t& local) {
    for (auto command: this->source) {
      switch (command.opcode) {
      case OP_NOP:
        break;
      case OP_ARG:
      { 
        number_t& num = *static_cast<number_t*>(command.arg);
        local.Push(stack.Top(static_cast<size_t>(num.Value())));
        break;
      }
      case OP_PUSH:
        local.Push(*command.arg);
        break;
      case OP_DROP:
      {
        number_t& num = *static_cast<number_t*>(command.arg);
        local.Drop(static_cast<int>(num.Value()));
        break;
      }
      case OP_CALL:
      {
        stack_t temp;
        function_t& func = *static_cast<function_t*>(command.arg);
        func.Evaluate(vm, local, temp);
        this->result->Delete();
        this->result = temp.ReleaseResult();
        break;
      }
      case OP_RESULT:
        local.Push(*this->result);
        break;
      case OP_RETURN:
        local.Result(local.Top(0));
        break;
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
      default:
        {
          std::string argToken;
          input >> argToken;
          result.push_back({ opcode, SelectValue(argToken.c_str()) });
          break;
        }
      }
    }

    return std::move(result);
  }

} // namespace deci
