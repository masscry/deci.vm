#include <deci.hpp>
#include <iec61131.y.hpp>

namespace deci {

  const std::string &ast_item_t::EndLocationTag() const {
    static const std::string none("");

    if (this->Parent() != nullptr) 
    {
      return this->Parent()->EndLocationTag();
    }
    return none;
  }

  int ast_item_t::EndLocationPos() const {
    if (this->Parent() != nullptr)
    {
      return this->Parent()->EndLocationPos();
    }
    return -1;
  }

  ast_item_t::ast_item_t() : parent(nullptr)
  { 
    ;
  }

  ast_item_t::~ast_item_t() 
  {
    ;
  }

  int ast_t::Generate(std::ostream& output, int pc) const 
  {
    for (auto item: this->statements) {
      pc = item->Generate(output, pc);
    }
    return pc;
  }

  void ast_t::Append(ast_item_t* item) 
  {
    this->statements.push_back(item);
  }

  ast_t::ast_t():statements() 
  {
    ;
  }

  ast_t::~ast_t() {
    for (auto item: this->statements) {
      delete item;
    }
  }

  int ast_number_t::Generate(std::ostream& output, int pc) const {
    output << "push " << this->value << std::endl;
    return pc + 1;
  }

  ast_number_t::ast_number_t(double value):value(value) {
    ;
  }

  ast_number_t::~ast_number_t() {

  }

  int ast_identifier_t::Generate(std::ostream& output, int pc) const {
    output << "rval " << this->value << std::endl; 
    return pc + 1;
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

  int ast_arg_list_t::Generate(std::ostream& output, int pc) const {
    for (auto item: args) {
      pc = item->Generate(output, pc);
    }
    return pc;
  }

  ast_arg_list_t::ast_arg_list_t():args() {
    ;
  }

  ast_arg_list_t::~ast_arg_list_t() {
    for (auto item: args) {
      delete item;
    }
  }

  int ast_postfix_t::Generate(std::ostream& output, int pc) const {
    if (arglist == nullptr) {
      output << "call " << this->identifier << "\nresl" << std::endl;
      return pc + 2;
    }
    else
    {
      pc = arglist->Generate(output, pc);
      output << "call " << this->identifier << "\ndrop " << arglist->Total() << "\nresl" << std::endl;
      return pc + 3;
    }
  }

  ast_postfix_t::ast_postfix_t(const std::string& identifier, ast_arg_list_t* arglist)
    :arglist(arglist), identifier(identifier) {
      ;
  }
  
  ast_postfix_t::~ast_postfix_t() {
    delete this->arglist;
  }

  int ast_binary_t::Generate(std::ostream& output, int pc) const {
    pc = this->a->Generate(output, pc);
    pc = this->b->Generate(output, pc);
    output << "bin " << this->identifier << std::endl;
    return pc + 1;
  }

  ast_binary_t::ast_binary_t(const std::string& identifier, ast_item_t* a, ast_item_t* b)
    :identifier(identifier),a(a),b(b) {
      ;
  }

  ast_binary_t::~ast_binary_t() {
    delete this->a;
    delete this->b;
  }

  int ast_unary_t::Generate(std::ostream& output, int pc) const {
    pc = this->chain->Generate(output, pc);
    output << "unr " << this->identifier << std::endl;
    return pc + 1;
  }

  ast_unary_t::ast_unary_t(const std::string& identifier, ast_item_t* chain)
    : identifier(identifier), chain(chain)  {
      ;
  }

  ast_unary_t::~ast_unary_t() {
    delete this->chain;
  }

  int ast_return_t::Generate(std::ostream& output, int pc) const {
    pc = this->chain->Generate(output, pc);
    output << "ret" << std::endl;
    return pc + 1;
  }

  ast_return_t::ast_return_t(ast_item_t* chain):chain(chain) {
    ;
  }

  ast_return_t::~ast_return_t() {
    delete this->chain;
  }

  int ast_set_t::Generate(std::ostream& output, int pc) const {
    pc = this->chain->Generate(output, pc);
    output << "set " << this->identifier << std::endl;
    return pc + 1;
  }

  ast_set_t::ast_set_t(const std::string& identifier, ast_item_t* chain):identifier(identifier), chain(chain) {
    ;
  }

  ast_set_t::~ast_set_t() {
    delete this->chain;
  }

  int ast_if_t::Generate(std::ostream& output, int pc) const {
    pc = this->condition->Generate(output, pc);

    int true_loc = pc;

    output << "jz __label_" << true_loc << "__" << std::endl;
    pc = this->true_path->Generate(output, pc + 1);

    if (this->else_path != nullptr)
    {
      int else_loc = pc;
      output << "jmp __label_" << else_loc << "__" << std::endl;
      output << ": __label_" << true_loc << "__" << std::endl;
      pc = this->else_path->Generate(output, pc + 1);
      output << ": __label_" << else_loc << "__" << std::endl;
    }
    else
    {
      output << ": __label_" << true_loc << "__" << std::endl;
    }
    return pc;
  }

  ast_if_t::ast_if_t(ast_item_t* condition, ast_t* true_path, ast_t* else_path)
    : condition(condition), true_path(true_path), else_path(else_path) {
      ;
  }

  ast_if_t::~ast_if_t() {
    delete this->condition;
    delete this->true_path;
    delete this->else_path;
  }

  const std::string &ast_for_t::EndLocationTag() const
  {
    static const std::string repeatText("for");
    return repeatText;
  }

  int ast_for_t::Generate(std::ostream& output, int pc) const {
    pc = start->Generate(output, pc);

    output << "set " << this->identifier << std::endl;
    ++pc;

    int check_loc = pc;

    output << ": __for_" << check_loc << "__" << std::endl;

    pc = finish->Generate(output, pc);
    output << "rval " << this->identifier << std::endl;
    output << "bin gr" << std::endl;
    pc += 2;

    output << "jz __end_for_" << check_loc << "__" << std::endl;
    ++pc;

    this->end_loc_pos = check_loc;
    pc = loop->Generate(output, pc);

    if (this->step != nullptr) {
      pc = this->step->Generate(output, pc);
    }
    else {
      output << "push 1" << std::endl;
      ++pc;
    }
    output << "rval " << this->identifier << std::endl;
    output << "bin sum" << std::endl;
    output << "set " << this->identifier << std::endl;
    pc += 3;

    output << "jmp __for_" << check_loc << "__" << std::endl;
    ++pc;

    output << ": __end_for_" << check_loc << "__" << std::endl;

    return pc;
  }

  ast_for_t::ast_for_t(const std::string& identifier, ast_item_t* start, ast_item_t* finish, ast_t* loop, ast_item_t* step)
    :identifier(identifier), start(start), finish(finish), loop(loop), step(step) {
    ;
  }

  ast_for_t::~ast_for_t() {
    delete this->start;
    delete this->finish;
    delete this->loop;
    delete this->step;
  }

  int ast_while_t::Generate(std::ostream& output, int pc) const {

    int while_loc = pc;

    output << ": __while_" << while_loc << "__" << std::endl;
    pc = this->condition->Generate(output, pc);
    output << "jz __end_while_" << while_loc << "__" << std::endl;
    ++pc;

    this->end_loc_pos = while_loc;
    pc = this->loop->Generate(output, pc);
    output << "jmp __while_" << while_loc << "__" << std::endl;
    ++pc;

    output << ": __end_while_" <<while_loc << "__" << std::endl;
    return pc;
  }

  const std::string &ast_while_t::EndLocationTag() const 
  {
    static const std::string whileText("while");
    return whileText;
  }

  ast_while_t::ast_while_t(ast_item_t* condition, ast_t* loop)
    :condition(condition), loop(loop) {
      ;
  }

  ast_while_t::~ast_while_t() {
    delete this->condition;
    delete this->loop;
  }

  const std::string &ast_repeat_t::EndLocationTag() const
  {
    static const std::string repeatText("repeat");
    return repeatText;
  }

  int ast_repeat_t::Generate(std::ostream& output, int pc) const {

    int repeat_loc = pc;

    this->end_loc_pos = repeat_loc;
    output << ": __repeat_" << repeat_loc << "__" << std::endl;
    pc = this->loop->Generate(output, pc);

    pc = this->condition->Generate(output, pc);
    output << "jz __repeat_" << repeat_loc << "__" << std::endl;
    ++pc;

    output << ": __end_repeat_" << repeat_loc << "__" << std::endl;

    return pc;
  }

  ast_repeat_t::ast_repeat_t(ast_item_t* condition, ast_t* loop)
    :condition(condition), loop(loop) {
      ;
  }
 
  ast_repeat_t::~ast_repeat_t() {
    delete this->condition;
    delete this->loop;
  }

  int ast_exit_t::Generate(std::ostream &output, int pc) const {

    int                end_loc = this->EndLocationPos();
    const std::string& end_tag = this->EndLocationTag();

    if (end_loc < 0) 
    {
      throw std::runtime_error("Exit statement not allowed in non-loop context");
    }

    output << "jmp __end_" << end_tag << "_" << end_loc << "__" << std::endl;
    return pc + 1;
  }

  ast_exit_t::ast_exit_t() {
    ;
  }

  ast_exit_t::~ast_exit_t() {
    ;
  }

  int ast_loop_t::EndLocationPos() const
  {
    return this->end_loc_pos;
  }

  ast_loop_t::ast_loop_t(): end_loc_pos(-1)
  {

  }

  ast_loop_t::~ast_loop_t() 
  {

  }


}