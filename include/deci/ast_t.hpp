#pragma once
#ifndef __DECI_AST_HEADER__
#define __DECI_AST_HEADER__

#include <fstream>
#include <vector>

namespace deci
{

  class ast_item_t {
  public:
    virtual void Generate(std::ostream& output) const = 0;
    virtual ~ast_item_t() = 0;
  };

  class ast_number_t final: public ast_item_t {
    double value;
  public:

    void Generate(std::ostream& output) const;

    ast_number_t(double value);
    ~ast_number_t();

  };

  class ast_identifier_t final: public ast_item_t {
    std::string value;
  public:

    void Generate(std::ostream& output) const;

    ast_identifier_t(const std::string& value);
    ~ast_identifier_t();

  };

  class ast_arg_list_t final: public ast_item_t {
    std::vector<ast_item_t*> args;
  public:

    size_t Total() const;

    void Append(ast_item_t* item);

    void Generate(std::ostream& output) const;

    ast_arg_list_t();
    ~ast_arg_list_t();
  };

  class ast_postfix_t final: public ast_item_t {
    ast_arg_list_t* arglist;
    std::string identifier;

  public:

    void Generate(std::ostream& output) const;

    ast_postfix_t(const std::string& identifier, ast_arg_list_t* arglist);
    ~ast_postfix_t();
  };

  class ast_binary_t final: public ast_item_t {    
    std::string identifier;
    ast_item_t* a;
    ast_item_t* b;
  public:

    void Generate(std::ostream& output) const;

    ast_binary_t(const std::string& identifier, ast_item_t* a, ast_item_t* b);
    ~ast_binary_t();
  };

  class ast_unary_t final: public ast_item_t {    
    std::string identifier;
    ast_item_t* chain;
  public:

    void Generate(std::ostream& output) const;

    ast_unary_t(const std::string& identifier, ast_item_t* chain);
    ~ast_unary_t();
  };

  class ast_return_t final: public ast_item_t {
    ast_item_t* chain;
  public:

    void Generate(std::ostream& output) const;

    ast_return_t(ast_item_t* chain);
    ~ast_return_t();
  };

  class ast_set_t final: public ast_item_t {
    std::string identifier;
    ast_item_t* chain;
  public:

    void Generate(std::ostream& output) const;

    ast_set_t(const std::string& identifier, ast_item_t* chain);
    ~ast_set_t();
  };

  class ast_t final: public ast_item_t {
    std::vector<ast_item_t*> statements;
  public:

    void Append(ast_item_t* item);

    void Generate(std::ostream& output) const;

    ast_t();
    ~ast_t();

  };

} // namespace deci



#endif /* __DECI_AST_HEADER__ */