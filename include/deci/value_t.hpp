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
    mutable size_t hash;

    virtual size_t DoHashing() const = 0;
  
  protected:

    value_t(const value_t& copy):hash(copy.hash) {
      ;
    }

  public:

    enum type_t {
      UNDEFINED   = -1,
      TYPES_FIRST =  0,
      NUMBER      = TYPES_FIRST,
      STRING        ,
      ARRAY         ,
      DICTIONARY    ,
      FUNCTION      ,
      REFERENCE     ,
      NOTHING       ,
      TYPES_TOTAL 
    };

    size_t Hash() const {
      if ((this->hash & 0x1) != 0x0) {
        this->hash = this->DoHashing();
        this->hash &= ~(1);
      }
      return this->hash;
    }

    value_t():hash(0x1) {
      ;
    }

    inline bool IsNothing() const;

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

    size_t DoHashing() const override {
      return 0;
    }

  public:
    
    value_t::type_t Type() const override {
      return value_t::NOTHING;
    }

    value_t* Copy() const override {
      return const_cast<nothing_t*>(this);
    }

    std::string ToText() const override {
      return std::string("deci::nothing");
    }

    void Delete() override {
      ;
    }

    static value_t& Instance() {
      static nothing_t self;
      return self;      
    }
    
  };

  bool value_t::IsNothing() const {
    return this == &nothing_t::Instance();
  }

}

#endif /* __DECI_VALUE_T_HEADER__ */
