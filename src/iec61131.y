%skeleton "lalr1.cc"
%require "3.0"

%defines
%define api.value.type variant
%define parse.assert

%code requires {
  #define YY_NULLPTR nullptr
}

%{

#include <iostream>
#include <string>
#include <cstdio>

#include "iec61131.y.hpp"
int yylex(yy::parser::semantic_type * a);

%}

%token NUMBER
%token OPERATOR
%token END

%type <char> OPERATOR
%type <double> NUMBER

%%

entry
  : END
  | expr END
;

expr
  : item 
  | expr item
;

item
  : NUMBER   {}
  | OPERATOR {}

%%

void yy::parser::error(const std::string & msg) {
  std::cout << msg << std::endl;
}