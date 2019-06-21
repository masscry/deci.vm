#include <deci.hpp>
#include <deci/scanner_t.hpp>
#include <iec61131.y.hpp>

namespace deci
{

  program_t::source_t CompileExpression(std::istream& input) {
    program_t::source_t result;

    deci_scanner_t scanner(input);
    deci::parser_t parser(scanner);

    parser.parse();

    return result;
  }
} // namespace deci
