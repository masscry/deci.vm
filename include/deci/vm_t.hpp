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
    typedef std::vector<value_t*> stack_t;

    stack_t dataStack;

    vm_t(const vm_t&) = delete;
    vm_t(vm_t&&) = delete;

    vm_t& operator = (const vm_t&) = delete;
    vm_t& operator = (vm_t&&) = delete;

  public:

    /**
     * Function pushes copy of value on stack.
     *
     * @param val value to put on stack
     */
    void     Push(const value_t& val);

    /**
     * Function take value from top of stack, and returns it.
     *
     * User must call value_t::Delete(), when work with returned value
     * completed.
     *
     * @return zero if stack is empty, pointer otherwise
     */
    value_t* Pop();

    /**
     * Get value from stack top.
     *
     * @param depth how deep from top element required
     * @return depth-th value from top, or throw exception
     */
    value_t& Top(size_t depth) const;

    /**
     * Drop `total` elements from stack.
     *
     * @param total how many elements to drop
     */
    void     Drop(int total);

    /**
     * Take elements from top and try to execute them.
     */
    void     Run();

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
