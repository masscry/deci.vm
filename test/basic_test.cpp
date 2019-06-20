#include <deci.hpp>
#include <iostream>
#include <cassert>

deci::command_t commands[] = {
  { deci::OP_PUSH,   deci::number_t(100).Copy()     },
  { deci::OP_PUSH,   deci::number_t(10).Copy()      },
  { deci::OP_PUSH,   deci::number_t(50).Copy()      },
  { deci::OP_CALL,   deci::sum_t::Instance().Copy() },
  { deci::OP_DROP,   deci::number_t(2).Copy()       },
  { deci::OP_RESULT, nullptr                        },
  { deci::OP_CALL,   deci::sum_t::Instance().Copy() },
  { deci::OP_DROP,   deci::number_t(2).Copy()       },
  { deci::OP_RESULT, nullptr                        },
  { deci::OP_RETURN, nullptr                        }
};

int main(int argc, char* argv[]) {
  deci::value_t* tmp;
  deci::vm_t vm;
  deci::program_t prog(commands, deci::countof(commands));

  vm.GlobalStack().Push(*(tmp = vm.Run(prog)));
  vm.GlobalStack().Print(std::cout);

  tmp->Delete();
  return 0;
}
