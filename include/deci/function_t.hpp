#pragma once
#ifndef __DECI_FUNCTION_HEADER__
#define __DECI_FUNCTION_HEADER__

namespace deci {

  class function_t: public value_t {
  public:

    virtual void Evaluate(vm_t& vm) = 0;

    value_t::type_t Type() const {
      return value_t::FUNCTION;
    }

    value_t* Copy() const override {
      return const_cast<function_t*>(this);
    }

    void Delete() override {
      ;
    }

  };

}

#endif /* __DECI_FUNCTION_HEADER__ */