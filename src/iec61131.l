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


[+-/*]      { 
              std::cout << "OPERATOR:" << yytext << std::endl;
              return deci::parser_t::make_OPERATOR(*yytext); 
            }
            
[1-9][0-9]* {
              std::cout << "NUMBER:" << yytext << std::endl;
              return deci::parser_t::make_NUMBER(atof(yytext));
            }

<<EOF>>     { yyterminate(); }

.           { 
  
            }
            
%%
