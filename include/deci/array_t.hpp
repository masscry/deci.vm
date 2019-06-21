#pragma once
#ifndef __DECI_ARRAY_HEADER__
#define __DECI_ARRAY_HEADER__

namespace deci
{

  class array_t: public value_t {
    typedef std::vector<value_t*> element_t;

    element_t elements;

    size_t DoHashing() const override;

  public:

    array_t();

    array_t(const array_t& copy);

    ~array_t();

    bool Empty() const;

    size_t Size() const;

    value_t& Element(int index) const;

    void Set(int index, const value_t& el);

    void Insert(int index, const value_t& el);

    void Append(const value_t& el);

    void Pop();

    value_t::type_t Type() const override;

    value_t* Copy() const override;

    void Delete() override;
    
    std::string ToText() const override;

  };

} // namespace deci

#endif /* __DECI_ARRAY_HEADER__ */