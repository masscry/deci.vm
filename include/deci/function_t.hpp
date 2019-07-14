#pragma once
#ifndef __DECI_FUNCTION_HEADER__
#define __DECI_FUNCTION_HEADER__

namespace deci {

  /**
   * @brief Class defines interface for basic executable entities.
   */
  class function_t: public value_t {
  public:

    /**
     * @brief Virtual machine calls this function when need to perform some programmable action.
     */
    virtual void Evaluate(vm_t& vm, const stack_t& stack, stack_t& local) = 0;

    value_t::type_t Type() const override {
      return value_t::FUNCTION;
    }

    /**
     * @brief By default, we do not copy this function objects.
     * 
     * We assume that basic function objects have single instances in global
     * memory.
     */
    value_t* Copy() const override {
      return const_cast<function_t*>(this);
    }

    /**
     * @brief By default, we do not need anything to delete.
     * 
     * We assume that basic functio objects would be deleted, when program exists
     * in automatic manner.
     */
    void Delete() override {
      ;
    }

  };

}

#endif /* __DECI_FUNCTION_HEADER__ */