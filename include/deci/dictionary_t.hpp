#pragma once
#ifndef __DECI_DICTIONARY_HEADER__
#define __DECI_DICTIONARY_HEADER__

#include <deci.hpp>
#include <unordered_map>

namespace deci
{

  struct value_hash_t {
    size_t operator() (const value_t* val) const  {
      return val->Hash();
    }
  };

  struct value_equal_to_t {
    bool operator() (const value_t* lhs, const value_t* rhs) const {
      value_t::type_t lhsType = lhs->Type();

      if (lhsType != rhs->Type()) {
        return false;
      }

      switch(lhsType) {
        case value_t::NUMBER:
          return static_cast<const number_t*>(lhs)->Value()
            == static_cast<const number_t*>(rhs)->Value();
        case value_t::STRING:
          throw std::runtime_error("String comparison is not implemented yet");
        case value_t::ARRAY:
          throw std::runtime_error("Array comparison is not possible");
        case value_t::DICTIONARY:
          throw std::runtime_error("Dictionary comparison is not possible");
        case value_t::FUNCTION:
          return lhs == rhs;
        case value_t::REFERENCE:
          return &(static_cast<const reference_t*>(lhs)->Dereference())
            == &(static_cast<const reference_t*>(rhs)->Dereference());
        case value_t::NOTHING:
          return true;
        default:
          throw std::runtime_error("Unknown data type");
      }
    }
  };

  class dictionary_t: public value_t
  {
    typedef std::unordered_map<value_t*, value_t*, value_hash_t, value_equal_to_t> storage_t; 

    storage_t storage;

    size_t DoHashing() const override {
      throw std::runtime_error("Invalid Operation");
    }

    dictionary_t(const dictionary_t& copy):storage() {
      this->storage.reserve(copy.storage.size());
      for (auto el: copy.storage) {
        value_t* key = el.first->Copy();
        value_t* value = el.second->Copy();
        this->storage.insert(std::make_pair(key,value));
      }
    }

    ~dictionary_t() {
      for (auto el: this->storage) {
        el.first->Delete();
        el.second->Delete();
      }
    }

  public:

    dictionary_t():storage() {
      ;
    }

    void Put(const value_t& key, const value_t& value) {
      this->storage.insert(std::make_pair(key.Copy(), value.Copy()));
    }

    void Remove(const value_t& key) {
      value_t* xkey = &const_cast<value_t&>(key);
      auto i = this->storage.find(xkey);
      if (i == this->storage.end()) {
        throw std::runtime_error("Element not found");
      }
      this->storage.erase(i);
    }

    const value_t& Get(const value_t& key) const {
      value_t* xkey = &const_cast<value_t&>(key);
      auto i = this->storage.find(xkey);
      if (i != this->storage.cend()) {
        return *i->second;
      }
      throw std::runtime_error("Element not found");
    }

    bool Has(const value_t& key) const {
      value_t* xkey = &const_cast<value_t&>(key);
      auto i = this->storage.find(xkey);
      return i != this->storage.cend();
    }

    type_t Type() const override {
      return value_t::DICTIONARY;
    }

    value_t* Copy() const override {
      value_t* result = new dictionary_t(*this);
      return result;
    }

    void Delete() override {
      delete this;
    }

    std::string ToText() const override {
      return std::string("deci::dictionary_t");
    }

  };
  
} // namespace deci

#endif /* __DECI_DICTIONARY_HEADER__ */