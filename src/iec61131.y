%skeleton "lalr1.cc"
%require "3.0"
%defines
%define parser_class_name { parser_t }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { deci }

%code requires {

  #include <deci.hpp>
  #define YY_NULLPTR nullptr

  class deci_scanner_t;

}

%code top
{
  #include "iec61131.y.hpp"
  #include <deci/scanner_t.hpp>

  #define yylex(SCANNER) SCANNER.get_next_token()

  using namespace deci;
}

%lex-param   { deci_scanner_t& scanner }

%parse-param { deci_scanner_t& scanner }
%parse-param { std::ostream& output }

%define parse.trace
%define parse.error verbose

%define api.token.prefix {TOK_}

%token END 0           "end of file"


%token <double>      NUMBER     "number" 
%token <std::string> IDENTIFIER "identifier"
%token               BOPEN      "("
%token               BCLOSE     ")"
%token               SUM        "+"
%token               SUB        "-"
%token               MUL        "*"
%token               DIV        "/"
%token               ASSIGN     ":="

%start entry

%%

entry:
    expression_statement
;

expression_statement:
    %empty
  | expression                
;

expression:
    assign_expr
;

assign_expr:
    add_expr                 { output << "ret" << std::endl;        }
  | IDENTIFIER ":=" add_expr { output << "set " << $1 << std::endl; }
;

add_expr:
    mul_expr
  | add_expr "+" mul_expr     { output << "bin sum" << std::endl; }
  | add_expr "-" mul_expr     { output << "bin sub" << std::endl; }
;

mul_expr:
    primary_expr
  | mul_expr "*" primary_expr { output << "bin mul" << std::endl; }
  | mul_expr "/" primary_expr { output << "bin div" << std::endl; }
;

primary_expr:
    NUMBER                    { output << "push " << $1 << std::endl; }
  | IDENTIFIER                { output << "rval " << $1 << std::endl; }
  | "(" expression ")"

%%

void deci::parser_t::error(const std::string & msg) {
  std::cerr << msg << std::endl;
}