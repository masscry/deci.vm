#include <deci.hpp>
#include <iostream>

int main_repl(int argc, char* argv[]) {

  deci::vm_t vm;

  while (true) {
    std::cout << ">> ";

    deci::program_t::source_t source = deci::CompileExpression("std::cin", std::cin, true);
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

int run(const char* filename) {

  deci::vm_t vm;

  std::ifstream input;
  input.open(filename, std::ios::in);

  if (input.good()) {
    deci::program_t::source_t source = deci::CompileExpression(filename, input,false);
    deci::program_t* prog = deci::program_t::Create(source.data(), source.size());
    deci::value_t* result = vm.Run(*prog);
    prog->Delete();

    if (!result->IsNothing())
    {
      std::cout << result->ToText() << std::endl;
    }
    result->Delete();
    return 0;
  }
  return -1;
}

int main(int argc, char* argv[]) {

  if (argc == 1) {
    return main_repl(argc, argv);
  }

  for (int i = 1; i < argc; ++i) {
    if (run(argv[i]) != 0) {
      return -1;
    }
  }
  return 0;
}