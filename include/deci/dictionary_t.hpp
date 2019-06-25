#pragma once
#ifndef __DECI_DICTIONARY_HEADER__
#define __DECI_DICTIONARY_HEADER__

namespace deci
{

  struct value_hash_t {
    size_t operator() (const value_t* val) const;
  };

  struct value_equal_to_t {
    bool operator() (const value_t* lhs, const value_t* rhs) const;
  };

  class dictionary_t: public value_t
  {
    typedef std::unordered_map<value_t*, value_t*, value_hash_t, value_equal_to_t> storage_t; 

    storage_t storage;

    size_t DoHashing() const override;

  public:

    dictionary_t();

    ~dictionary_t();

    dictionary_t(const dictionary_t& copy);
    dictionary_t(dictionary_t&& move);

    dictionary_t& operator = (const dictionary_t& copy);
    dictionary_t& operator = (dictionary_t&& move);

    void Put(const value_t& key, const value_t& value);

    void Remove(const value_t& key);

    void Insert(const dictionary_t& src);

    const value_t& Get(const value_t& key) const;

    bool Has(const value_t& key) const;

    type_t Type() const override;

    value_t* Copy() const override;

    void Delete() override;

    std::string ToText() const override;

  };
  
} // namespace deci

#endif /* __DECI_DICTIONARY_HEADER__ */