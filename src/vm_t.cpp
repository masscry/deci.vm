#include <deci.hpp>
#include <stdexcept>
#include <iostream>

namespace deci {

  vm_t::vm_t():dataStack() {
    ;
  }

  vm_t::~vm_t() {
    ;
  }

  void vm_t::Run(value_t& value) {
    switch (value.Type())
    {
    case value_t::FUNCTION: {
      function_t& func = static_cast<function_t&>(value);
      func.Evaluate(*this, this->GlobalStack());
      break;
    }
    default:
      throw std::runtime_error("Unknown value type to run");
      break;
    }
  }

}
