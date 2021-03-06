#pragma once
#ifndef __DECI_AST_HEADER__
#define __DECI_AST_HEADER__

#include <fstream>
#include <vector>

namespace deci
{

  class ast_item_t {
    ast_item_t* parent;
  public:

    ast_item_t* Parent() const {
      return this->parent;
    }

    void Parent(ast_item_t* parent) {
      this->parent = parent;
    }

    virtual const std::string &EndLocationTag() const;
    virtual int EndLocationPos() const;

    virtual int Generate(std::ostream& output, int pc) const = 0;

    ast_item_t();
    virtual ~ast_item_t() = 0;
  };

  class ast_number_t final: public ast_item_t {
    double value;
  public:

    int Generate(std::ostream& output, int pc) const override;

    ast_number_t(double value);
    ~ast_number_t();

  };

  class ast_identifier_t final: public ast_item_t {
    std::string value;
  public:

    int Generate(std::ostream& output, int pc) const override;

    ast_identifier_t(const std::string& value);
    ~ast_identifier_t();

  };

  class ast_arg_list_t final: public ast_item_t {
    std::vector<ast_item_t*> args;
  public:

    size_t Total() const;

    void Append(ast_item_t* item);

    int Generate(std::ostream& output, int pc) const override;

    ast_arg_list_t();
    ~ast_arg_list_t();
  };

  class ast_postfix_t final: public ast_item_t {
    ast_arg_list_t* arglist;
    std::string identifier;

  public:

    int Generate(std::ostream& output, int pc) const override;

    ast_postfix_t(const std::string& identifier, ast_arg_list_t* arglist);
    ~ast_postfix_t();
  };

  class ast_binary_t final: public ast_item_t {    
    std::string identifier;
    ast_item_t* a;
    ast_item_t* b;
  public:

    int Generate(std::ostream& output, int pc) const override;

    ast_binary_t(const std::string& identifier, ast_item_t* a, ast_item_t* b);
    ~ast_binary_t();
  };

  class ast_unary_t final: public ast_item_t {    
    std::string identifier;
    ast_item_t* chain;
  public:

    int Generate(std::ostream& output, int pc) const override;

    ast_unary_t(const std::string& identifier, ast_item_t* chain);
    ~ast_unary_t();
  };

  class ast_return_t final: public ast_item_t {
    ast_item_t* chain;
  public:

    int Generate(std::ostream& output, int pc) const override;

    ast_return_t(ast_item_t* chain);
    ~ast_return_t();
  };

  class ast_set_t final: public ast_item_t {
    std::string identifier;
    ast_item_t* chain;
  public:

    int Generate(std::ostream& output, int pc) const override;

    ast_set_t(const std::string& identifier, ast_item_t* chain);
    ~ast_set_t();
  };

  class ast_t final: public ast_item_t {
    std::vector<ast_item_t*> statements;
  public:
 
    void Append(ast_item_t* item);

    int Generate(std::ostream& output, int pc) const override;

    ast_t();
    ~ast_t();

  };

  class ast_exit_t final : public ast_item_t
  {
  public:
    int Generate(std::ostream &output, int pc) const override;

    ast_exit_t();
    ~ast_exit_t();
  };

  class ast_if_t final: public ast_item_t {
    ast_item_t* condition;
    ast_t*      true_path;
    ast_t*      else_path;
  public:

    int Generate(std::ostream& output, int pc) const override;

    ast_if_t(ast_item_t* condition, ast_t* true_path, ast_t* else_path);
    ~ast_if_t();

  };

  class ast_loop_t: public ast_item_t {
  protected:
    mutable int end_loc_pos;
  public:
    int EndLocationPos() const override;

    ast_loop_t();
    ~ast_loop_t();
  };

  class ast_for_t final: public ast_loop_t {
    std::string identifier;
    ast_item_t* start;
    ast_item_t* finish;
    ast_t*      loop;
    ast_item_t* step;

  public:
    const std::string& EndLocationTag() const override;

    int Generate(std::ostream& output, int pc) const override;

    ast_for_t(const std::string& identifier, ast_item_t* start, ast_item_t* finish, ast_t* loop, ast_item_t* step);
    ~ast_for_t();
  };

  class ast_while_t final: public ast_loop_t {
    ast_item_t* condition;
    ast_t* loop;

  public:
    const std::string &EndLocationTag() const override;

    int Generate(std::ostream &output, int pc) const override;

    ast_while_t(ast_item_t* condition, ast_t* loop);
    ~ast_while_t();
  };

  class ast_repeat_t final: public ast_loop_t {
    ast_item_t* condition;
    ast_t* loop;

  public:
    const std::string& EndLocationTag() const override;

    int Generate(std::ostream& output, int pc) const override;

    ast_repeat_t(ast_item_t* condition, ast_t* loop);
    ~ast_repeat_t();
  };


} // namespace deci



#endif /* __DECI_AST_HEADER__ */