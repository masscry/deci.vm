/**
 * @brief Virtual Machine implementation
 * @file vm_t.hpp
 * @author timur
 */

#pragma once
#ifndef __DECI_VM_HEADER__
#define __DECI_VM_HEADER__

namespace deci {

  /**
   * VM class.
   */
  class vm_t final {

    stack_t dataStack;
    
    vm_t(const vm_t&) = delete;
    vm_t(vm_t&&) = delete;

    vm_t& operator = (const vm_t&) = delete;
    vm_t& operator = (vm_t&&) = delete;

  public:
    
    /**
     * Run element in VM.
     */
    value_t* Run(value_t& value);

    stack_t& GlobalStack() {
      return this->dataStack;
    }

    const stack_t& GlobalStack() const {
      return this->dataStack;
    }

    /**
     * Default constructor.
     */
    vm_t();

    /**
     * Destructor
     */
    ~vm_t();

  };

}

#endif /* __DECI_VM_HEADER__ */
