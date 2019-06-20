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

  enum opcode_t {
    OP_UNDEFINED = -1,
    OP_NOP = 0,
    OP_ARG,
    OP_PUSH,
    OP_RESULT,
    OP_DROP,
    OP_CALL,
    OP_RETURN,
    OP_TOTAL
  };

  struct command_t {
    opcode_t opcode;
    value_t* arg;
  };

  class program_t: public function_t
  {
  public:
    typedef std::vector<command_t> source_t;

  private:

    source_t source;
    value_t* result;

  public:

    void Evaluate(vm_t& vm, const stack_t& stack, stack_t& local) override;

    std::string ToText() const override {
      return std::string("deci::program");
    }

    program_t(const command_t* source, size_t sourceSize);
    ~program_t();

  };

  program_t::source_t AssembleProgram(std::istream& input);

} // namespace deci



#endif /* __DECI_PROGRAM_HEADER__ */