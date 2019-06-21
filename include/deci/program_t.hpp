/**
 * @brief Basic program implementation
 * 
 * @file program_t.hpp
 * @author timur
 */

#pragma once
#ifndef __DECI_PROGRAM_HEADER__
#define __DECI_PROGRAM_HEADER__

namespace deci
{
  /**
   * @brief Complete list of opcodes.
   */
  enum opcode_t {
    OP_UNDEFINED = -1 , /**< Unknown opcode, can be found only in during assembly */
    OP_FIRST = 0      , /**< Helper for loops */
    OP_NOP = OP_FIRST , /**< No operation */
    OP_ARG            , /**< Take N-th element from global stack and put in local */
    OP_PUSH           , /**< Push immediate value on local stack */
    OP_RESULT         , /**< Push value from result-register on local stack */
    OP_DROP           , /**< Drop N elements from local stack */
    OP_CALL           , /**< Call provided function, local stack becomes global for called function */
    OP_RETURN         , /**< Set result-register to top value from stack */
    OP_TOTAL            /**< Total opcodes */
  };

  /**
   * @brief Basic command structure.
   */
  struct command_t {
    opcode_t opcode; /**< Opcode */
    value_t* arg;    /**< Argument. When no actual value, must be set to nothing_t::Instance().Copy() */
  };

  /**
   * @brief Class implements support for user-defined programs.
   * 
   * User-defined programs do not differ from built-in functions.
   */
  class program_t: public function_t
  {
  public:
    typedef std::vector<command_t> source_t;

  private:

    mutable int refCount;
    source_t    source;
    value_t*    result;

    program_t(const command_t* source, size_t sourceSize);
    ~program_t();

    size_t DoHashing() const override;

  public:

    void Evaluate(vm_t& vm, const stack_t& stack, stack_t& local) override;

    std::string ToText() const override;
    
    static program_t* Create(const command_t* source, size_t sourceSize);

    value_t* Copy() const override;

    void Delete() override;

  };

  program_t::source_t AssembleProgram(std::istream& input);

} // namespace deci



#endif /* __DECI_PROGRAM_HEADER__ */