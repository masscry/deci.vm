#include <deci.hpp>
#include <iostream>
#include <fstream>
#include <cassert>

deci::command_t commands[] = {
  { deci::OP_PUSH,   deci::number_t(100).Copy()         },
  { deci::OP_PUSH,   deci::number_t(10).Copy()          },
  { deci::OP_PUSH,   deci::number_t(50).Copy()          },
  { deci::OP_CALL,   deci::sum_t::Instance().Copy()     },
  { deci::OP_DROP,   deci::number_t(2).Copy()           },
  { deci::OP_RESULT, deci::nothing_t::Instance().Copy() },
  { deci::OP_CALL,   deci::sub_t::Instance().Copy()     },
  { deci::OP_DROP,   deci::number_t(2).Copy()           },
  { deci::OP_RESULT, deci::nothing_t::Instance().Copy() },
  { deci::OP_RETURN, deci::nothing_t::Instance().Copy() }
};

int noargs() {
  deci::value_t* tmp;
  deci::vm_t vm;
  deci::program_t* prog = deci::program_t::Create(commands, deci::countof(commands));

  vm.GlobalStack().Push(*(tmp = vm.Run(*prog)));
  vm.GlobalStack().Print(std::cout);

  tmp->Delete();
  prog->Delete();
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return noargs();
  }

  std::ifstream input;
  input.open(argv[1], std::ios::in);
  if (!input.good()) {
    throw std::runtime_error("Can't open file");
  }

  deci::program_t::source_t source = deci::AssembleProgram(input);
  input.close();

  deci::value_t* tmp;
  deci::vm_t vm;
  deci::program_t* prog = deci::program_t::Create(source.data(), deci::countof(commands));

  vm.GlobalStack().Push(*(tmp = vm.Run(*prog)));
  vm.GlobalStack().Print(std::cout);

  tmp->Delete();
  prog->Delete();

  return 0;
}