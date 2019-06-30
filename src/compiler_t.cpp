#include <deci.hpp>
#include <deci/scanner_t.hpp>
#include <iec61131.y.hpp>
#include <sstream>

namespace deci
{
  program_t::source_t CompileExpression(const char* fname, std::istream& input, bool interactive) {
    std::stringstream tempstr;

    deci_scanner_t scanner(input, fname, interactive
    );
    deci::parser_t parser(scanner, tempstr);

    parser.parse();

    return AssembleProgram(tempstr);
  }
} // namespace deci
