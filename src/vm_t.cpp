#include <deci.hpp>
#include <stdexcept>
#include <iostream>

namespace deci {

  vm_t::vm_t():dataStack() {
    this->dataStack.reserve(8);
  }

  vm_t::~vm_t() {
    ;
  }

  void vm_t::Push(const value_t& val) {
    this->dataStack.push_back(val.Copy());
  }

  value_t* vm_t::Pop() {
    if (!this->dataStack.empty()) {
      value_t* result = this->dataStack.back();
      this->dataStack.pop_back();
      return result;
    }
    return nullptr;
  }

  value_t& vm_t::Top(size_t depth) const {
    size_t size = this->dataStack.size();

    if (depth >= size)
    {
      throw std::runtime_error("Stack Is Empty");
    }

    return *this->dataStack[size-depth-1];
  }

  void vm_t::Drop(int total) {
    for (int i = 0; i < total; ++i) {
      this->Pop()->Delete();
    }
  }

  void vm_t::Run() {
    while (!this->dataStack.empty())
    {
      value_t* top = this->Pop();
      switch(top->Type())
      {
        case value_t::NUMBER:
        {
          number_t& number = *static_cast<number_t*>(top);
          std::cout << number.Value() << std::endl;
          break;
        }
        case value_t::FUNCTION:
        {
          function_t& function = *static_cast<function_t*>(top);
          function.Evaluate(*this);
          break;
        }
        default:
          throw std::runtime_error("Invalid Value Type");
      }
      top->Delete();
    }
  }

}
