#pragma once
#ifndef __DECI_NUMBER_T_HEADER__
#define __DECI_NUMBER_T_HEADER__

#include <cstring>
#include <algorithm>

namespace deci {

  /**
   * @brief Basic number class.
   * 
   * We'll cut a lot of work, when we drop support for integer data types 
   * altogether. So, all numbers in interpreter are double-precision floating point
   * numbers.
   */
  class number_t: public value_t {

    double val;

    size_t DoHashing() const override {
      return std::hash<double>()(val);
    }

  public:

    number_t();

    explicit number_t(double val);
    explicit number_t(const number_t& copy);

    double Value() const {
      return this->val;
    }

    number_t& operator = (const number_t& copy);

    value_t::type_t Type() const override {
      return value_t::NUMBER;
    }

    value_t* Copy() const override {
      return new number_t(*this);
    }

    void Delete() override {
      delete this;
    }

    std::string ToText() const override {
      char buffer[64];
      snprintf(buffer, sizeof(buffer), "%e", this->val);
      return std::string(buffer);
    }

    ~number_t();
  };
  
}

#endif /* __DECI_NUMBER_T_HEADER__ */