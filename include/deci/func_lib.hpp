#pragma once
#ifndef __DECI_FUNC_LIB_HEADER__
#define __DECI_FUNC_LIB_HEADER__

namespace deci {

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

    void Evaluate(vm_t&, stack_t& stack) override {
      number_t& a = dynamic_cast<number_t&>(stack.Top(1));
      number_t& b = dynamic_cast<number_t&>(stack.Top(0));
      number_t result(a.Value() + b.Value());
      stack.Drop(2);
      stack.Push(result);
    }

    std::string ToText() override {
      return std::string("deci::sum_t");
    }

    static sum_t& Instance() {
      static sum_t self;
      return self;
    }

  };

}

#endif /* __DECI_FUNC_LIB_HEADER__ */