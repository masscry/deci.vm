/**
 * @brief deci VM main header
 * @file deci.hpp
 * @author timur
 */

#pragma once
#ifndef __DECI_HEADER__
#define __DECI_HEADER__

#include <vector>
#include <cstdint>

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

  class number_t: public value_t {

    double val;

  public:

    number_t();

    explicit number_t(double val);
    explicit number_t(const number_t& copy);

    double Value() const {
      return this->val;
    }

    number_t& operator = (const number_t& copy);

    value_t::type_t Type() const override {
      return value_t::NUMBER;
    }

    value_t* Copy() const override {
      return new number_t(*this);
    }

    void Delete() override {
      delete this;
    }

    ~number_t();
  };

  class vm_t final {
    typedef std::vector<value_t*> stack_t;

    stack_t dataStack;

    vm_t(const vm_t&) = delete;
    vm_t(vm_t&&) = delete;

    vm_t& operator = (const vm_t&) = delete;
    vm_t& operator = (vm_t&&) = delete;
  public:

    void     Push(const value_t& val);
    value_t* Pop();

    value_t& Top(int depth) const;
    void     Drop(int total);
    void     Run();

    vm_t();
    ~vm_t();

  };

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

  class sum_t: public function_t {

    sum_t(const sum_t&) = delete;
    sum_t& operator =(const sum_t&) = delete;

    sum_t(){
      ;
    }

    ~sum_t() {
      ;
    }

  public:

    void Evaluate(vm_t& vm) {
      number_t& a = dynamic_cast<number_t&>(vm.Top(1));
      number_t& b = dynamic_cast<number_t&>(vm.Top(0));
      number_t result(a.Value() + b.Value());
      vm.Drop(2);
      vm.Push(result);
    }

    static sum_t& Instance() {
      static sum_t self;
      return self;
    }

  };

}

#endif /* __DECI_HEADER__ */
