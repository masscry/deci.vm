/**
 * @brief deci VM main header
 * @file deci.hpp
 * @author timur
 */

#pragma once
#ifndef __DECI_VALUE_T_HEADER__
#define __DECI_VALUE_T_HEADER__


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
      TYPES_TOTAL
    };

    virtual type_t Type() const = 0;
    virtual value_t* Copy() const = 0;
    virtual void Delete() = 0;
    virtual ~value_t() = 0;
  };

}

#endif /* __DECI_VALUE_T_HEADER__ */
