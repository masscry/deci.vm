#include <deci.hpp>
#include <iostream>
#include <cassert>

int main(int argc, char* argv[]) {
  deci::vm_t vm;
  vm.GlobalStack().Push(deci::number_t(100.0));
  vm.GlobalStack().Push(deci::number_t(10.0));
  vm.GlobalStack().Push(deci::number_t(50.0));
  vm.Run(deci::sum_t::Instance());
  vm.Run(deci::sum_t::Instance());
  vm.GlobalStack().Print(std::cout);
  return 0;
}
