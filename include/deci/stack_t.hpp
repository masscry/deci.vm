/**
 * @brief Data stack implementation
 * @file stack_t.hpp
 * @author timur
 */

#pragma once
#ifndef __DECI_STACK_HEADER__
#define __DECI_STACK_HEADER__

namespace deci
{
  class value_t;

  class stack_t final {
    typedef std::vector<value_t*> storage_t;

    storage_t    storage;
    dictionary_t context;
    value_t*     result;

  public:

    const value_t& Variable(const value_t& key) const;

    void Variable(const value_t& key, const value_t& val);

    void MergeVariable(const stack_t& src);

    value_t* ReleaseResult();

    value_t& Result() const;

    void Result(const value_t& val);

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

    bool    Empty() const;
    
    /**
     * Get total number of elements on stack.
     * 
     * @return total elements on stack
     */
    size_t   Depth() const;

    stack_t();

    stack_t& operator = (const stack_t& copy);

    stack_t& operator = (stack_t&& move);

    void Print(std::ostream& output);
    
    stack_t(const stack_t& copy);

    stack_t(stack_t&& move);

    ~stack_t();

  };
} // namespace deci



#endif /* __DECI_STACK_HEADER__ */