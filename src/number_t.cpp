#include <deci.hpp>
#include <cfloat>

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
    :value_t(copy), val(copy.val)
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

  value_t::type_t number_t::Type() const {
    return value_t::NUMBER;
  }

  value_t* number_t::Copy() const {
    return new number_t(*this);
  }

  void number_t::Delete() {
    delete this;
  }

  std::string number_t::ToText() const {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.*g", DECIMAL_DIG, this->val);
    return std::string(buffer);
  }

}