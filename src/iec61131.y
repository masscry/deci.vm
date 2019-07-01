%skeleton "lalr1.cc"
%require "3.0"
%defines
%locations
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
%parse-param { deci::ast_t& ast }

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
%token               IF         "if"
%token               THEN       "then"
%token               ELSIF      "elsif"
%token               ELSE       "else"
%token               END_IF     "end_if"

%type <ast_arg_list_t*>         argument_list "argument list"

%type <ast_t*>      statement_list
%type <ast_item_t*> statement expression_statement expression
%type <ast_item_t*> assign_expr or_expr xor_expr and_expr eql_expr cmp_expr
%type <ast_item_t*> add_expr mul_expr pow_expr unary_expr postfix_expr primary_expr
%type <ast_item_t*> if_statement

%type <std::string> CMP_OPERATOR

%start entry

%%

entry:
    statement_list           { ast.Append($1); }
;

statement_list:
    statement                { $$ = new ast_t(); if ($1 != nullptr) { $$->Append($1); } }
  | statement_list statement { $$ = $1; $$->Append($2); }
;

statement:
    ";"                      { $$ = nullptr; }
  | expression_statement ";" { $$ = $1;      }
  | if_statement ";"         { $$ = $1;      }
;

expression_statement:
    expression              { $$ = $1; }
;

expression:
    assign_expr             { $$ = $1; }
;

assign_expr:
    or_expr                 { $$ = new ast_return_t($1); }
  | IDENTIFIER ":=" or_expr { $$ = new ast_set_t($1, $3); }
;

or_expr:
    xor_expr                  { $$ = $1; }
  | or_expr "or" xor_expr     { $$ = new ast_binary_t("or", $1, $3);  }
;

xor_expr:
    and_expr                  { $$ = $1; }
  | xor_expr "xor" and_expr   { $$ = new ast_binary_t("xor", $1, $3); }
;

and_expr:
    eql_expr                  { $$ = $1; }
  | and_expr "and" eql_expr   { $$ = new ast_binary_t("and", $1, $3); }
;

eql_expr:
    cmp_expr                  { $$ = $1; }
  | eql_expr "=" cmp_expr     { $$ = new ast_binary_t("eq", $1, $3); }
  | eql_expr "<>" cmp_expr    { $$ = new ast_binary_t("neq", $1, $3); }
;

cmp_expr:
    add_expr                       { $$ = $1; }
  | cmp_expr CMP_OPERATOR add_expr { $$ = new ast_binary_t($2, $1, $3);  }
;

CMP_OPERATOR:
    "<"  { $$ = "ls"; }
  | ">"  { $$ = "gr"; }
  | "<=" { $$ = "le"; }
  | ">=" { $$ = "ge"; }

add_expr:
    mul_expr                  { $$ = $1; }
  | add_expr "+" mul_expr     { $$ = new ast_binary_t("sum", $1, $3); }
  | add_expr "-" mul_expr     { $$ = new ast_binary_t("sub", $1, $3); }
;

mul_expr:
    pow_expr                  { $$ = $1; }
  | mul_expr "*"   pow_expr   { $$ = new ast_binary_t("mul", $1, $3); }
  | mul_expr "mod" pow_expr   { $$ = new ast_binary_t("mod", $1, $3); }
  | mul_expr "/"   pow_expr   { $$ = new ast_binary_t("div", $1, $3); }
;

pow_expr:
    unary_expr               { $$ = $1; }
  | pow_expr "**" unary_expr { $$ = new ast_binary_t("pow", $1, $3); }
;

unary_expr:
    postfix_expr               { $$ = $1; }
  | "not" unary_expr           { $$ = new ast_unary_t("not", $2 ); }
  | "-" unary_expr             { $$ = new ast_unary_t("neg", $2 ); }
;

postfix_expr:
    primary_expr                      { $$ = $1; }
  | IDENTIFIER "(" ")"                { $$ = new ast_postfix_t($1, nullptr); }
  | IDENTIFIER "(" argument_list ")"  { $$ = new ast_postfix_t($1, $3); }
;

argument_list:
    or_expr                          { $$ = new ast_arg_list_t(); $$->Append($1); }
  | argument_list "," or_expr        { $$ = $1;                   $$->Append($3); }
;

primary_expr:
    NUMBER                    { $$ = new ast_number_t($1); }
  | IDENTIFIER                { $$ = new ast_identifier_t($1); }
  | "(" expression ")"        { $$ = $2; }
;

if_statement:
    "if" or_expr "then" statement_list "else" statement_list "end_if" { $$ = new ast_if_t($2, $4, $6, @3.begin.line, @5.begin.line);  }
  | "if" or_expr "then" statement_list "end_if"                       { $$ = new ast_if_t($2, $4, nullptr, @3.begin.line, -1);        }
;

%%

void deci::parser_t::error(const location_type& loc, const std::string & msg) {
  std::cerr << loc << ": " << msg << std::endl;
}