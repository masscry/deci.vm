#pragma once
#ifndef __DECI_ARRAY_HEADER__
#define __DECI_ARRAY_HEADER__

#include <deci.hpp>

namespace deci
{

  class array_t: public value_t {
    typedef std::vector<value_t*> element_t;

    element_t elements;

    ~array_t();

    size_t DoHashing() const override;

  public:

    array_t();

    array_t(const array_t& copy);

    bool Empty() const;

    size_t Size() const;

    value_t& Element(int index) const;

    void Set(int index, value_t& el);

    void Insert(int index, value_t& el);

    void Append(value_t& el);

    void Pop();

    value_t::type_t Type() const override;

    value_t* Copy() const override;

    void Delete() override;
    
    std::string ToText() const override;

  };

} // namespace deci

#endif /* __DECI_ARRAY_HEADER__ */