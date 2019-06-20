/**
 * @brief deci VM main header
 * @file deci.hpp
 * @author timur
 */

#pragma once
#ifndef __DECI_VALUE_T_HEADER__
#define __DECI_VALUE_T_HEADER__

#include <string>

namespace deci {

  class value_t {
  public:

    enum type_t {
      UNDEFINED = 0,
      NUMBER,
      STRING,
      ARRAY,
      DICTIONARY,
      FUNCTION,
      REFERENCE,
      NOTHING,
      TYPES_TOTAL
    };

    virtual type_t Type() const = 0;
    virtual value_t* Copy() const = 0;
    virtual void Delete() = 0;
    virtual std::string ToText() const = 0;
    virtual ~value_t() = 0;
  };

  class nothing_t: public value_t {

    ~nothing_t() {
      ;
    }

  public:
    
    value_t::type_t Type() const override {
      return value_t::NOTHING;
    }

    value_t* Copy() const override {
      return const_cast<nothing_t*>(this);
    }

    std::string ToText() const override {
      return std::move(std::string("nothing"));
    }

    void Delete() override {
      ;
    }

    static value_t& Instance() {
      static nothing_t self;
      return self;      
    }
    
  };

}

#endif /* __DECI_VALUE_T_HEADER__ */
