%{
  #include <deci.hpp>
  #include <deci/scanner_t.hpp>
	#include "iec61131.y.hpp"

	#define yyterminate() return deci::parser_t::make_END();

%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="deci_scanner_t"
%option prefix="deci_"

%%

\+              {
                  return deci::parser_t::make_SUM();
                }

\-              {
                  return deci::parser_t::make_SUB();
                }

\*              {
                  return deci::parser_t::make_MUL();
                }

\/              {
                  return deci::parser_t::make_DIV();
                }

[0-9]+\.[0-9]*  {
                  return deci::parser_t::make_NUMBER(atof(yytext));
                }

[0-9]+          {
                  return deci::parser_t::make_NUMBER(atof(yytext));
                }

[)]             {
                  return deci::parser_t::make_BCLOSE();
                }

[(]             {
                  return deci::parser_t::make_BOPEN();
                }


[\n]            {
                  return deci::parser_t::make_END();
                }

<<EOF>>         {
                  return deci::parser_t::make_END();
                }

[ \t\v]         {
                  // IGNORE SPACES;
                }

.               {
                  throw deci::parser_t::syntax_error("Unknown symbol");
                }

%%
