#include <deci.hpp>
#include <deci/scanner_t.hpp>
#include <iec61131.y.hpp>
#include <sstream>

namespace deci
{
  program_t::source_t CompileExpression(std::istream& input) {
    std::stringstream tempstr;

    deci_scanner_t scanner(input);
    deci::parser_t parser(scanner, tempstr);

    parser.parse();

    return AssembleProgram(tempstr);
  }
} // namespace deci
