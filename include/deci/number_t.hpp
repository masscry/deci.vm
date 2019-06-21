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

    value_t::type_t Type() const override;

    value_t* Copy() const override;

    void Delete() override;

    std::string ToText() const override;

    ~number_t();
  };
  
}

#endif /* __DECI_NUMBER_T_HEADER__ */