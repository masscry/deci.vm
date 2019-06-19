#include <deci.hpp>
#include <stdexcept>
#include <iostream>

namespace deci {

  number_t::number_t()
    :val(0.0)
  {
    ;
  }

  number_t::number_t(double val)
    :val(val)
  {
    ;
  }

  number_t::number_t(const number_t& copy)
    :val(copy.val)
  {
    ;
  }

  number_t::~number_t() {
    ;
  }

  number_t& number_t::operator = (const number_t& copy) {
    if (this != &copy) {
      this->val = copy.val;
    }
    return *this;
  }

}