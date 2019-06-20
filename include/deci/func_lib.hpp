#pragma once
#ifndef __DECI_FUNC_LIB_HEADER__
#define __DECI_FUNC_LIB_HEADER__

namespace deci {

  /**
   * @brief Default binary operation template
   * 
   * Class implements boilerplate code for binary operations.
   */
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
    
    /**
     * @brief Take top two arguments from global stack, evaluate and put result in result variable.
     */
    void Evaluate(vm_t&, const stack_t& stack, stack_t& local) override {
      number_t& a = dynamic_cast<number_t&>(stack.Top(1));
      number_t& b = dynamic_cast<number_t&>(stack.Top(0));
      local.Result(number_t(xclass::Execute(a.Value(), b.Value())));
    }

    /**
     * @brief Each function has it's own ID
     */
    std::string ToText() const override {
      return xclass::ID();
    }

    /**
     * @brief Single static instance of binary operation object.
     * 
     * We do not need more than one such object in program.
     */
    static binop_t& Instance() {
      static binop_t self;
      return self;
    }

  };

  /**
   * @brief Macro to make binary operation definitions shorter
   */
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