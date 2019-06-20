#include <deci.hpp>

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
    :source(source, source + sourceSize)
    ,result(&nothing_t::Instance()) {
      ;
  }
  
  program_t::~program_t() {
    result->Delete();
    for (auto command: this->source)
    {
      command.arg->Delete();
    }
  }
 
} // namespace deci
