%{
  #include <deci.hpp>
  #include <deci/scanner_t.hpp>
	#include "iec61131.y.hpp"

	#define yyterminate() return deci::parser_t::make_END();

%}

%option caseless
%option nodefault
%option noyywrap
%option c++
%option yyclass="deci_scanner_t"
%option prefix="deci_"

L            [A-Z]
D             [0-9]

%%

XOR               {
                    return deci::parser_t::make_XOR();
                  }

OR                {
                    return deci::parser_t::make_OR();
                  }

MOD               {
                    return deci::parser_t::make_MOD();
                  }

NOT               {
                    return deci::parser_t::make_NOT();
                  }

(AND|[&])         {
                    return deci::parser_t::make_AND();
                  }

[=]               {
                    return deci::parser_t::make_EQU();
                  }

\<\>              {
                    return deci::parser_t::make_NEQU();
                  }

\>                {
                    return deci::parser_t::make_GR();
                  }

\<                {
                    return deci::parser_t::make_LS();
                  }

\>\=              {
                    return deci::parser_t::make_GE();
                  }

\<\=              {
                    return deci::parser_t::make_LE();
                  }

\*\*              {
                    return deci::parser_t::make_POW();
                  }

\;                {
                    return deci::parser_t::make_ST_END();
                  }

({L}|_({L}|{D}))(_?({L}|{D}))* {
                    return deci::parser_t::make_IDENTIFIER(deci::ToUpper(deci::ToLower(yytext)));
                  }

[0-9]+\.[0-9]*    {
                    return deci::parser_t::make_NUMBER(atof(yytext));
                  }

[0-9]+            {
                    return deci::parser_t::make_NUMBER(atof(yytext));
                  }

:=                {
                    return deci::parser_t::make_ASSIGN();
                  }

\+                {
                    return deci::parser_t::make_SUM();
                  }

\-                {
                    return deci::parser_t::make_SUB();
                  }

\*                {
                    return deci::parser_t::make_MUL();
                  }

,                 {
                    return deci::parser_t::make_COMMA();
                  }

\/                {
                    return deci::parser_t::make_DIV();
                  }

[)]               {
                    return deci::parser_t::make_BCLOSE();
                  }

[(]               {
                    return deci::parser_t::make_BOPEN();
                  }


[\n]              {
                    return deci::parser_t::make_END();
                  }

<<EOF>>           {
                    return deci::parser_t::make_END();
                  }

[ \t\v]           {
                    // IGNORE SPACES;
                  }

.                 {
                    throw deci::parser_t::syntax_error("Unknown symbol");
                  }

%%
