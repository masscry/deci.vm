#include <deci.hpp>
#include <cassert>

int main(int argc, char* argv[]) {
  deci::vm_t vm;
  vm.Push(deci::number_t(100.0));
  vm.Push(deci::number_t(10.0));
  vm.Push(deci::number_t(50.0));
  deci::sum_t::Instance().Evaluate(vm);
  deci::sum_t::Instance().Evaluate(vm);

  vm.Run();

  return 0;
}
