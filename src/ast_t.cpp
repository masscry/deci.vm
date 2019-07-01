#include <deci.hpp>

namespace deci {

  ast_item_t::~ast_item_t() {
    ;
  }

  void ast_t::Generate(std::ostream& output) const {
    for (auto item: this->statements) {
      item->Generate(output);
    }
  }

  void ast_t::Append(ast_item_t* item) {
    this->statements.push_back(item);
  }

  ast_t::ast_t():statements() {
    ;
  }

  ast_t::~ast_t() {
    for (auto item: this->statements) {
      delete item;
    }
  }

  void ast_number_t::Generate(std::ostream& output) const {
    output << "push " << this->value << std::endl;
  }

  ast_number_t::ast_number_t(double value):value(value) {
    ;
  }

  ast_number_t::~ast_number_t() {

  }

  void ast_identifier_t::Generate(std::ostream& output) const {
    output << "rval " << this->value << std::endl; 
  }

  ast_identifier_t::ast_identifier_t(const std::string& value):value(value) {

  }

  ast_identifier_t::~ast_identifier_t() {
    ;
  }

  size_t ast_arg_list_t::Total() const {
    return this->args.size();
  }

  void ast_arg_list_t::Append(ast_item_t* item) {
    this->args.push_back(item);
  }

  void ast_arg_list_t::Generate(std::ostream& output) const {
    for (auto item: args) {
      item->Generate(output);
    }
  }

  ast_arg_list_t::ast_arg_list_t():args() {
    ;
  }

  ast_arg_list_t::~ast_arg_list_t() {
    for (auto item: args) {
      delete item;
    }
  }

  void ast_postfix_t::Generate(std::ostream& output) const {
    if (arglist == nullptr) {
      output << "call " << this->identifier << "\nresl" << std::endl; 
    }
    else
    {
      arglist->Generate(output);
      output << "call " << this->identifier << "\ndrop " << arglist->Total() << "\nresl" << std::endl;
    }
  }

  ast_postfix_t::ast_postfix_t(const std::string& identifier, ast_arg_list_t* arglist)
    :arglist(arglist), identifier(identifier) {
      ;
  }
  
  ast_postfix_t::~ast_postfix_t() {
    delete this->arglist;
  }

  void ast_binary_t::Generate(std::ostream& output) const {
    this->a->Generate(output);
    this->b->Generate(output);
    output << "bin " << this->identifier << std::endl;
  }

  ast_binary_t::ast_binary_t(const std::string& identifier, ast_item_t* a, ast_item_t* b)
    :identifier(identifier),a(a),b(b) {
      ;
  }

  ast_binary_t::~ast_binary_t() {
    delete this->a;
    delete this->b;
  }

  void ast_unary_t::Generate(std::ostream& output) const {
    this->chain->Generate(output);
    output << "unr " << this->identifier << std::endl;
  }

  ast_unary_t::ast_unary_t(const std::string& identifier, ast_item_t* chain)
    : identifier(identifier), chain(chain)  {
      ;
  }

  ast_unary_t::~ast_unary_t() {
    delete this->chain;
  }

  void ast_return_t::Generate(std::ostream& output) const {
    this->chain->Generate(output);
    output << "ret" << std::endl;
  }

  ast_return_t::ast_return_t(ast_item_t* chain):chain(chain) {
    ;
  }

  ast_return_t::~ast_return_t() {
    delete this->chain;
  }

  void ast_set_t::Generate(std::ostream& output) const {
    this->chain->Generate(output);
    output << "set " << this->identifier << std::endl;
  }

  ast_set_t::ast_set_t(const std::string& identifier, ast_item_t* chain):identifier(identifier), chain(chain) {
    ;
  }

  ast_set_t::~ast_set_t() {
    delete this->chain;
  }

  void ast_if_t::Generate(std::ostream& output) const {
    this->condition->Generate(output);
    output << "jz __label_" << this->true_loc << "__" << std::endl;
    this->true_path->Generate(output);

    if (this->else_path != nullptr)
    {
      output << "jmp __label_" << this->else_loc << "__" << std::endl;
      output << "label __label_" << this->true_loc << "__" << std::endl;
      this->else_path->Generate(output);
      output << "label __label_" << this->else_loc << "__" << std::endl;
    }
    else
    {
      output << "label __label_" << this->true_loc << "__" << std::endl;
    }
  }

  ast_if_t::ast_if_t(ast_item_t* condition, ast_t* true_path, ast_t* else_path, int true_loc, int else_loc)
    : condition(condition), true_path(true_path), else_path(else_path), true_loc(true_loc), else_loc(else_loc) {
      ;
  }

  ast_if_t::~ast_if_t() {
    delete this->condition;
    delete this->true_path;
    delete this->else_path;
  }

}