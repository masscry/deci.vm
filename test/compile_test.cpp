#include <deci.hpp>
#include <iostream>

int main(int argc, char* argv[]) {

  deci::vm_t vm;

  while (true) {
    std::cout << ">> ";

    deci::program_t::source_t source = deci::CompileExpression(std::cin);
    deci::program_t* prog = deci::program_t::Create(source.data(), source.size());
    deci::value_t* result = vm.Run(*prog);
    prog->Delete();

    if (!result->IsNothing())
    {
      std::cout << result->ToText() << std::endl;
    }
    result->Delete();
  }

  return 0;
}