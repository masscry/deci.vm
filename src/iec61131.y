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

%define parse.trace
%define parse.error verbose

%define api.token.prefix {TOK_}

%token END 0           "end of file"
%token <double> NUMBER "number" 
%token BOPEN           "("
%token BCLOSE          ")"
%token SUM             "+"
%token SUB             "-"
%token MUL             "*"
%token DIV             "/"

%start entry

%%

entry
  : expression_statement END
;

expression_statement
  : %empty
  | expression                { std::cout << "ret" << std::endl; }
;

expression
  : mul_expr
  | expression "+" mul_expr     { std::cout << "call sum" << std::endl << "drop 2" << std::endl << "resl" << std::endl; }
  | expression "-" mul_expr     { std::cout << "call sub" << std::endl << "drop 2" << std::endl << "resl" << std::endl; }
;

mul_expr
  : primary_expr
  | mul_expr "*" primary_expr { std::cout << "call mul" << std::endl << "drop 2" << std::endl << "resl" << std::endl;; }
  | mul_expr "/" primary_expr { std::cout << "call div" << std::endl << "drop 2" << std::endl << "resl" << std::endl;; }
;

primary_expr
  : NUMBER                    { std::cout << "push " << $1 << std::endl; }
  | "(" expression ")"

%%

void deci::parser_t::error(const std::string & msg) {
  std::cout << msg << std::endl;
}