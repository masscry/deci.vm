#pragma once
#ifndef __DECI_SCANNER_HEADER__
#define __DECI_SCANNER_HEADER__

#ifndef yyFlexLexerOnce
#undef yyFlexLexer
#define yyFlexLexer deci_FlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL deci::parser_t::symbol_type deci_scanner_t::get_next_token()

#include <iec61131.y.hpp>

class deci_scanner_t: public deci_FlexLexer {
  std::string fname;
  deci::location cloc;
public:
  deci::parser_t::symbol_type get_next_token();

  deci_scanner_t(std::istream& input, const std::string& fname):deci_FlexLexer(&input),fname(fname),cloc() {
    this->cloc.initialize(&this->fname, 1, 1);
  }

};

#endif /* __DECI_SCANNER_HEADER__ */