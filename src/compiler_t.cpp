#include <deci.hpp>
#include <deci/scanner_t.hpp>
#include <iec61131.y.hpp>
#include <sstream>

namespace deci
{
  program_t::source_t CompileExpression(const char* fname, std::istream& input, bool interactive) {
    std::stringstream tempstr;

    deci_scanner_t scanner(input, fname, interactive);

    deci::ast_t ast;
    deci::parser_t parser(scanner, ast);

    parser.parse();

    int totalLength = ast.Generate(tempstr, 0);

    return AssembleProgram(tempstr, true);
  }
} // namespace deci
