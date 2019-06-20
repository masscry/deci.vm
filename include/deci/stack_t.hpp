/**
 * @brief Data stack implementation
 * @file stack_t.hpp
 * @author timur
 */

#pragma once
#ifndef __DECI_STACK_HEADER__
#define __DECI_STACK_HEADER__

#include <vector>
#include <cstddef>
#include <stdexcept>

namespace deci
{
  class value_t;

  class stack_t final {
    typedef std::vector<value_t*> storage_t;

    storage_t storage;
    value_t*  result;

  public:

    value_t* ReleaseResult() {
      value_t* ret = this->result;
      this->result = &nothing_t::Instance();
      return ret;
    }

    value_t& Result() const {
      return *this->result;
    }

    void Result(const value_t& val) {
      this->result = val.Copy();
    }

    /**
     * Function pushes copy of value on stack.
     *
     * @param val value to put on stack
     */
    void     Push(const value_t& val) {
      this->storage.push_back(val.Copy());
    }

    /**
     * Function take value from top of stack, and returns it.
     *
     * User must call value_t::Delete(), when work with returned value
     * completed.
     *
     * @return zero if stack is empty, pointer otherwise
     */
    value_t* Pop() {
      if (!this->storage.empty()) {
        value_t* result = this->storage.back();
        this->storage.pop_back();
        return result;
      }
      throw std::runtime_error("Stack is empty");
    }

    /**
     * Get value from stack top.
     *
     * @param depth how deep from top element required
     * @return depth-th value from top, or throw exception
     */
    value_t& Top(size_t depth) const {
      size_t size = this->storage.size();
      
      if (depth >= size)
      {
        throw std::runtime_error("Stack Is Too Small");
      }      
      return *this->storage[size-depth-1];
    }

    /**
     * Drop `total` elements from stack.
     *
     * @param total how many elements to drop
     */
    void     Drop(int total) {
      for (int i = 0; i < total; ++i) {
        this->Pop()->Delete();
      }
    }

    bool    Empty() const {
      return this->storage.empty();
    }
    
    /**
     * Get total number of elements on stack.
     * 
     * @return total elements on stack
     */
    size_t   Depth() const {
      return this->storage.size();
    }

    stack_t():storage(),result(&nothing_t::Instance()) {
      storage.reserve(8);
    }

    stack_t& operator = (const stack_t& copy) {
      if (this != &copy) {
        this->storage = copy.storage;
      }
      return *this;
    }

    stack_t& operator = (stack_t&& move) {
      if (this != &move) {
        this->storage = std::move(move.storage);
      }
      return *this;
    }

    void Print(std::ostream& output);
    
    stack_t(const stack_t& copy):storage(copy.storage),result(&nothing_t::Instance()) {
      ;
    }

    stack_t(stack_t&& move):storage(std::move(move.storage)),result(move.result) {
      ;
    }

    ~stack_t() {
      this->result->Delete();
    }

  };
} // namespace deci



#endif /* __DECI_STACK_HEADER__ */