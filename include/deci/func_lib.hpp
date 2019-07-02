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

    size_t DoHashing() const override {
      return std::hash<const void*>()(this);
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

  #define DEFINE_XCLASS_INT(NAME, ACTION)\
  class NAME ## _xclass_t {\
  public:\
    static double Execute(double a, double b) {\
      return ((int32_t)a) ACTION ((int32_t)b);\
    }\
    static constexpr const char* ID() {\
      return "deci::" # NAME;\
    }\
  }

  DEFINE_XCLASS(sum, +);
  DEFINE_XCLASS(sub, -);
  DEFINE_XCLASS(mul, *);
  DEFINE_XCLASS(div, /);
  DEFINE_XCLASS(ls,  <);
  DEFINE_XCLASS(gr,  >);
  DEFINE_XCLASS(le,  <=);
  DEFINE_XCLASS(ge,  >=);
  DEFINE_XCLASS(eq,  ==);
  DEFINE_XCLASS(neq, !=);
  DEFINE_XCLASS_INT(or,  ||);
  DEFINE_XCLASS_INT(and, &&);
  DEFINE_XCLASS_INT(xor, ^);
  DEFINE_XCLASS_INT(mod, %);

  #undef DEFINE_XCLASS
  #undef DEFINE_XCLASS_INT

  class pow_xclass_t {
  public:
    static double Execute(double a, double b) {
      return pow(a, b);
    }
    static constexpr const char* ID() {
      return "deci::pow";
    }
  };

  typedef binop_t<sum_xclass_t> sum_t;
  typedef binop_t<sub_xclass_t> sub_t;
  typedef binop_t<mul_xclass_t> mul_t;
  typedef binop_t<div_xclass_t> div_t;
  typedef binop_t< ls_xclass_t>  ls_t;
  typedef binop_t< gr_xclass_t>  gr_t;
  typedef binop_t< le_xclass_t>  le_t;
  typedef binop_t< ge_xclass_t>  ge_t;
  typedef binop_t< eq_xclass_t>  eq_t;
  typedef binop_t<neq_xclass_t> neq_t;
  typedef binop_t< or_xclass_t>  or_t;
  typedef binop_t<and_xclass_t> and_t;
  typedef binop_t<xor_xclass_t> xor_t;
  typedef binop_t<pow_xclass_t> pow_t;
  typedef binop_t<mod_xclass_t> mod_t;

  template<typename xclass>
  class unrop_t: public function_t {

    unrop_t(const unrop_t&) = delete;
    unrop_t& operator =(const unrop_t&) = delete;

    unrop_t(){
      ;
    }
    
    ~unrop_t() {
      ;
    }

    size_t DoHashing() const override {
      return std::hash<const void*>()(this);
    }

  public:
    
    /**
     * @brief Take top two arguments from global stack, evaluate and put result in result variable.
     */
    void Evaluate(vm_t&, const stack_t& stack, stack_t& local) override {
      number_t& a = dynamic_cast<number_t&>(stack.Top(0));
      local.Result(number_t(xclass::Execute(a.Value())));
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
    static unrop_t& Instance() {
      static unrop_t self;
      return self;
    }
  };

  #define DEFINE_UNR_XCLASS(NAME, ACTION)\
  class NAME ## _xclass_t {\
  public:\
    static double Execute(double a) {\
      return ACTION a;\
    }\
    static constexpr const char* ID() {\
      return "deci::" # NAME;\
    }\
  }

  #define DEFINE_UNR_XCLASS_INT(NAME, ACTION)\
  class NAME ## _xclass_t {\
  public:\
    static double Execute(double a, double b) {\
      return ACTION ((int32_t)a);\
    }\
    static constexpr const char* ID() {\
      return "deci::" # NAME;\
    }\
  }

  DEFINE_UNR_XCLASS(not, !);
  DEFINE_UNR_XCLASS(neg, -);

  typedef unrop_t<not_xclass_t> not_t;
  typedef unrop_t<neg_xclass_t> neg_t;

  #undef DEFINE_UNR_CLASS
  #undef DEFINE_UNR_CLASS_INT

  class print_t:public function_t {

    print_t(const print_t&) = delete;
    print_t& operator =(const print_t&) = delete;

    print_t(){
      ;
    }
    
    ~print_t() {
      ;
    }

    size_t DoHashing() const override {
      return std::hash<const void*>()(this);
    }

  public:

    void Evaluate(vm_t&, const stack_t& stack, stack_t& local) override {
      std::cout << stack.Top(0).ToText() << std::endl;     
      local.Result(deci::nothing_t::Instance()); 
    }

    static print_t& Instance() {
      static print_t self;
      return self;
    }

    std::string ToText() const override {
      return "deci::print_t";
    }
  };

}

#endif /* __DECI_FUNC_LIB_HEADER__ */