#include <deci.hpp>

namespace deci {

  vm_t::vm_t():dataStack() {
    ;
  }

  vm_t::~vm_t() {
    ;
  }

  value_t* vm_t::Run(value_t& value) {
    switch (value.Type())
    {
    case value_t::FUNCTION: {
      stack_t local;
      function_t& func = static_cast<function_t&>(value);
      func.Evaluate(*this, this->dataStack, local);
      this->dataStack.MergeVariable(local);
      return local.ReleaseResult();
    }
    default:
      throw std::runtime_error("Unknown value type to run");
    }
  }

}
