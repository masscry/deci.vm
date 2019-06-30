#pragma once
#ifndef __DECI_COMPILER_HEADER__
#define __DECI_COMPILER_HEADER__

namespace deci
{
  
  /**
   * Reads input stream, creates assembled program code.
   */
  program_t::source_t CompileExpression(const char* fname, std::istream& input);

} // namespace deci

#endif /* __DECI_COMPILER_HEADER__ */