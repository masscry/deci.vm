#pragma once
#ifndef __DECI_STRING_T_HEADER__
#define __DECI_STRING_T_HEADER__

#include <deci.hpp>

namespace deci {

  /**
   * @brief Basic string class.
   */
  class string_t: public value_t {

    std::string val;

    size_t DoHashing() const override {
      return std::hash<std::string>()(val);
    }

  public:

    const std::string& Value() const {
      return this->val;
    }

    string_t& operator = (const string_t& copy);

    value_t::type_t Type() const override;

    value_t* Copy() const override;

    void Delete() override;

    std::string ToText() const override;

    string_t();

    explicit string_t(const std::string& copy);
    explicit string_t(const string_t& copy);
    explicit string_t(std::istream& input);

    void Serialize(std::ostream& output) const;

    ~string_t();
  };
  
}

#endif /* __DECI_STRING_T_HEADER__ */