%option noyywrap

%{

#include <stdexcept>
#include "iec61131.y.hpp"
#define YYSTYPE yy::parser::semantic_type  

#define yyterminate() return yy::parser::token::END;

%}

%option debug
%option bison-bridge
%option nodefault


%%

[0-9]+\.[0-9]+ { yylval->build<double>(atof(yytext)); return yy::parser::token::NUMBER;   }
[0-9]+         { yylval->build<double>(atof(yytext)); return yy::parser::token::NUMBER;   }
[+-/*]         { yylval->build<char>(*yytext);        return yy::parser::token::OPERATOR; }

.              { throw yy::parser::syntax_error("Unexpected symbol"); }

%%