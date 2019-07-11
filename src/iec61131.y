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
%token               IF         "IF"
%token               THEN       "THEN"
%token               ELSIF      "ELSIF"
%token               ELSE       "ELSE"
%token               END_IF     "END_IF"
%token               FOR        "FOR"
%token               TO         "TO"
%token               BY         "BY"
%token               DO         "DO"
%token               END_FOR    "END_FOR"
%token               WHILE      "WHILE"
%token               END_WHILE  "END_WHILE"
%token               REPEAT     "REPEAT"
%token               UNTIL      "UNTIL"
%token               END_REPEAT "END_REPEAT"
%token               EXIT       "EXIT"

%type <ast_arg_list_t*>         argument_list "argument list"

%type <ast_t*>      statement_list "statement list"
%type <ast_item_t*> statement expression_statement expression
%type <ast_item_t*> assign_expr or_expr xor_expr and_expr eql_expr cmp_expr
%type <ast_item_t*> add_expr mul_expr pow_expr unary_expr postfix_expr primary_expr
%type <ast_item_t*> if_statement for_statement while_statement repeat_statement 
%type <ast_exit_t*> exit_statement

%type <std::string> CMP_OPERATOR

%start entry

%%

entry:
    statement_list                     { ast.Append($1); $1->Parent(&ast); }
;

statement_list:
    statement                      { $$ = new ast_t(); if ($1 != nullptr) { $$->Append($1); $1->Parent($$); } }
  | statement_list statement       { $$ = $1;          if ($2 != nullptr) { $$->Append($2); $2->Parent($$); } }
  | exit_statement                 { $$ = new ast_t(); if ($1 != nullptr) { $$->Append($1); $1->Parent($$); } }
  | statement_list exit_statement  { $$ = $1;          if ($2 != nullptr) { $$->Append($2); $2->Parent($$); } }
;

statement:
    ";"                      { $$ = nullptr;          }
  | expression_statement ";" { $$ = $1;               }
  | if_statement         ";" { $$ = $1;               }
  | for_statement        ";" { $$ = $1;               }
  | while_statement      ";" { $$ = $1;               }
  | repeat_statement     ";" { $$ = $1;               }
;

exit_statement:
    EXIT                 ";" { $$ = new ast_exit_t(); }
;

expression_statement:
    expression               { $$ = $1; }
;

expression:
    assign_expr              { $$ = $1; }
;

assign_expr:
    or_expr                  { $$ = new ast_return_t($1);  $1->Parent($$); }
  | IDENTIFIER ":=" or_expr  { $$ = new ast_set_t($1, $3); $3->Parent($$); }
;

or_expr:
    xor_expr                  { $$ = $1; }
  | or_expr "or" xor_expr     { $$ = new ast_binary_t("or", $1, $3); $1->Parent($$); $3->Parent($$); }
;

xor_expr:
    and_expr                  { $$ = $1; }
  | xor_expr "xor" and_expr   { $$ = new ast_binary_t("xor", $1, $3); $1->Parent($$); $3->Parent($$); }
;

and_expr:
    eql_expr                  { $$ = $1; }
  | and_expr "and" eql_expr   { $$ = new ast_binary_t("and", $1, $3); $1->Parent($$); $3->Parent($$); }
;

eql_expr:
    cmp_expr                  { $$ = $1; }
  | eql_expr "=" cmp_expr     { $$ = new ast_binary_t("eq", $1, $3); $1->Parent($$); $3->Parent($$); }
  | eql_expr "<>" cmp_expr    { $$ = new ast_binary_t("neq", $1, $3); $1->Parent($$); $3->Parent($$); }
;

cmp_expr:
    add_expr                       { $$ = $1; }
  | cmp_expr CMP_OPERATOR add_expr { $$ = new ast_binary_t($2, $1, $3); $1->Parent($$); $3->Parent($$);  }
;

CMP_OPERATOR:
    "<"  { $$ = "ls"; }
  | ">"  { $$ = "gr"; }
  | "<=" { $$ = "le"; }
  | ">=" { $$ = "ge"; }

add_expr:
    mul_expr                  { $$ = $1; }
  | add_expr "+" mul_expr     { $$ = new ast_binary_t("sum", $1, $3); $1->Parent($$); $3->Parent($$); }
  | add_expr "-" mul_expr     { $$ = new ast_binary_t("sub", $1, $3); $1->Parent($$); $3->Parent($$); }
;

mul_expr:
    pow_expr                  { $$ = $1; }
  | mul_expr "*"   pow_expr   { $$ = new ast_binary_t("mul", $1, $3); $1->Parent($$); $3->Parent($$); }
  | mul_expr "mod" pow_expr   { $$ = new ast_binary_t("mod", $1, $3); $1->Parent($$); $3->Parent($$); }
  | mul_expr "/"   pow_expr   { $$ = new ast_binary_t("div", $1, $3); $1->Parent($$); $3->Parent($$); }
;

pow_expr:
    unary_expr               { $$ = $1; }
  | pow_expr "**" unary_expr { $$ = new ast_binary_t("pow", $1, $3); $1->Parent($$); $3->Parent($$); }
;

unary_expr:
    postfix_expr               { $$ = $1; }
  | "not" unary_expr           { $$ = new ast_unary_t("not", $2 ); $2->Parent($$); }
  | "-" unary_expr             { $$ = new ast_unary_t("neg", $2 ); $2->Parent($$); }
;

postfix_expr:
    primary_expr                      { $$ = $1; }
  | IDENTIFIER "(" ")"                { $$ = new ast_postfix_t($1, nullptr); }
  | IDENTIFIER "(" argument_list ")"  { $$ = new ast_postfix_t($1, $3); $3->Parent($$); }
;

argument_list:
    or_expr                          { $$ = new ast_arg_list_t(); $$->Append($1); $1->Parent($$); }
  | argument_list "," or_expr        { $$ = $1;                   $$->Append($3); $3->Parent($$); }
;

primary_expr:
    NUMBER                    { $$ = new ast_number_t($1); }
  | IDENTIFIER                { $$ = new ast_identifier_t($1); }
  | "(" expression ")"        { $$ = $2; }
;

if_statement:
    "IF" or_expr "THEN" statement_list "ELSE" statement_list "END_IF" { $$ = new ast_if_t($2, $4,      $6); $2->Parent($$); $4->Parent($$); $6->Parent($$); }
  | "IF" or_expr "THEN" statement_list "END_IF"                       { $$ = new ast_if_t($2, $4, nullptr); $2->Parent($$); $4->Parent($$); }
;

for_statement:
    "FOR" IDENTIFIER ":=" or_expr "TO" or_expr "BY" or_expr "DO" statement_list "END_FOR" { $$ = new ast_for_t($2, $4, $6, $10, $8);     $4->Parent($$); $6->Parent($$); $8->Parent($$); $10->Parent($$); }
  | "FOR" IDENTIFIER ":=" or_expr "TO" or_expr "DO" statement_list "END_FOR"              { $$ = new ast_for_t($2, $4, $6, $8, nullptr); $4->Parent($$); $6->Parent($$); $8->Parent($$); }
;

while_statement:
    "WHILE" or_expr "DO" statement_list "END_WHILE"      { $$ = new ast_while_t($2, $4); $2->Parent($$); $4->Parent($$); }
;

repeat_statement:
    "REPEAT" statement_list "UNTIL" or_expr "END_REPEAT" { $$ = new ast_repeat_t($4, $2); $2->Parent($$); $4->Parent($$); }
;

%%

void deci::parser_t::error(const location_type& loc, const std::string & msg) {
  std::cerr << loc << ": " << msg << std::endl;
}