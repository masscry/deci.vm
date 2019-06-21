#pragma once
#ifndef __DECI_COMPILER_HEADER__
#define __DECI_COMPILER_HEADER__

namespace deci
{
  
  program_t::source_t CompileExpression(std::istream& input);

} // namespace deci

#endif /* __DECI_COMPILER_HEADER__ */