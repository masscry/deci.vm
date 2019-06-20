#pragma once
#ifndef __DECI_FUNC_LIB_HEADER__
#define __DECI_FUNC_LIB_HEADER__

namespace deci {

  template<typename xclass>
  class binop_t: public function_t {

    binop_t(const binop_t&) = delete;
    binop_t& operator =(const binop_t&) = delete;

    binop_t(){
      ;
    }
    
    ~binop_t() {
      ;
    }

  public:
    
    void Evaluate(vm_t&, const stack_t& stack, stack_t& local) override {
      number_t& a = dynamic_cast<number_t&>(stack.Top(1));
      number_t& b = dynamic_cast<number_t&>(stack.Top(0));
      local.Result(number_t(xclass::Execute(a.Value(), b.Value())));
    }

    std::string ToText() const override {
      return xclass::ID();
    }

    static binop_t& Instance() {
      static binop_t self;
      return self;
    }

  };

  #define DEFINE_XCLASS(NAME, ACTION)\
  class NAME ## _xclass_t {\
  public:\
    static double Execute(double a, double b) {\
      return a ACTION b;\
    }\
    static constexpr const char* ID() {\
      return "deci::" # NAME;\
    }\
  }

  DEFINE_XCLASS(sum, +);
  DEFINE_XCLASS(sub, -);
  DEFINE_XCLASS(mul, *);
  DEFINE_XCLASS(div, /);

  #undef XCLASS_DEFINE

  typedef binop_t<sum_xclass_t> sum_t;
  typedef binop_t<sub_xclass_t> sub_t;
  typedef binop_t<mul_xclass_t> mul_t;
  typedef binop_t<div_xclass_t> div_t;

}

#endif /* __DECI_FUNC_LIB_HEADER__ */