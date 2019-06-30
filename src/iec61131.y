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
%token               MOD        "mod"
%token               NOT        "not"
%token               ASSIGN     ":="
%token               COMMA      ","
%token               XOR        "xor"
%token               OR         "or"
%token               AND        "and"
%token               EQU        "="
%token               NEQU       "<>"
%token               GR         ">"
%token               LS         "<"
%token               GE         ">="
%token               LE         "<="
%token               POW        "**"
%token               ST_END     ";"

%type  <int>         argument_list "argument list"

%start entry

%%

entry:
    statement_list
;

statement_list:
    statement 
  | statement_list statement
;

statement:
    ";"
  | expression_statement ";"
;

expression_statement:
    expression
;

expression:
    assign_expr
;

assign_expr:
    or_expr                 { output << "ret" << std::endl;        }
  | IDENTIFIER ":=" or_expr { output << "set " << $1 << std::endl; }
;

or_expr:
    xor_expr
  | or_expr "or" xor_expr     { output << "bin or" << std::endl;  }
;

xor_expr:
    and_expr
  | xor_expr "xor" and_expr   { output << "bin xor" << std::endl; }
;

and_expr:
    eql_expr
  | and_expr "and" eql_expr   { output << "bin and" << std::endl; }
;

eql_expr:
    cmp_expr
  | eql_expr "=" cmp_expr     { output << "bin eq"  << std::endl; }
  | eql_expr "<>" cmp_expr    { output << "bin neq" << std::endl; }
;

cmp_expr:
    add_expr
  | cmp_expr "<" add_expr     { output << "bin ls" << std::endl; }
  | cmp_expr ">" add_expr     { output << "bin gr" << std::endl; }
  | cmp_expr "<=" add_expr    { output << "bin le" << std::endl; }
  | cmp_expr ">=" add_expr    { output << "bin ge" << std::endl; }
;

add_expr:
    mul_expr
  | add_expr "+" mul_expr     { output << "bin sum" << std::endl; }
  | add_expr "-" mul_expr     { output << "bin sub" << std::endl; }
;

mul_expr:
    pow_expr
  | mul_expr "*"   pow_expr     { output << "bin mul" << std::endl; }
  | mul_expr "mod" pow_expr     { output << "bin mod" << std::endl; }
  | mul_expr "/"   pow_expr     { output << "bin div" << std::endl; }
;

pow_expr:
    unary_expr
  | pow_expr "**" unary_expr { output << "bin pow" << std::endl; }
;

unary_expr:
    postfix_expr
  | "not" unary_expr           { output << "unr not" << std::endl; }
  | "-" unary_expr             { output << "unr neg" << std::endl; }
;

postfix_expr:
    primary_expr
  | IDENTIFIER "(" ")"                { output << "call " << $1 << "\nresl" << std::endl; }
  | IDENTIFIER "(" argument_list ")"  { output << "call " << $1 << "\ndrop " << $3 << "\nresl" << std::endl; }
;

argument_list:
    or_expr                          { $$ = 1;      }
  | argument_list "," or_expr        { $$ = $1 + 1; }
;

primary_expr:
    NUMBER                    { output << "push " << $1 << std::endl; }
  | IDENTIFIER                { output << "rval " << $1 << std::endl; }
  | "(" expression ")"
;

%%

void deci::parser_t::error(const std::string & msg) {
  std::cerr << msg << std::endl;
}