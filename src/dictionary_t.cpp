#include <deci.hpp>

namespace deci
{
  size_t value_hash_t::operator() (const value_t* val) const  {
    return val->Hash();
  }

  bool value_equal_to_t::operator() (const value_t* lhs, const value_t* rhs) const {
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
  
  size_t dictionary_t::DoHashing() const {
    throw std::runtime_error("Invalid Operation");
  }

  dictionary_t::dictionary_t(const dictionary_t& copy):storage() {
    this->storage.reserve(copy.storage.size());
    for (auto el: copy.storage) {
      value_t* key = el.first->Copy();
      value_t* value = el.second->Copy();
      this->storage.insert(std::make_pair(key,value));
    }
  }

  dictionary_t::~dictionary_t() {
    for (auto el: this->storage) {
      el.first->Delete();
      el.second->Delete();
    }
  }
  
  dictionary_t::dictionary_t():storage() {
    ;
  }

  void dictionary_t::Put(const value_t& key, const value_t& value) {
    this->storage.insert(std::make_pair(key.Copy(), value.Copy()));
  }

  void dictionary_t::Remove(const value_t& key) {
    value_t* xkey = &const_cast<value_t&>(key);
    auto i = this->storage.find(xkey);
    if (i == this->storage.end()) {
      throw std::runtime_error("Element not found");
    }
    this->storage.erase(i);
  }

  const value_t& dictionary_t::Get(const value_t& key) const {
    value_t* xkey = &const_cast<value_t&>(key);
    auto i = this->storage.find(xkey);
    if (i != this->storage.cend()) {
      return *i->second;
    }
    throw std::runtime_error("Element not found");
  }

  bool dictionary_t::Has(const value_t& key) const {
    value_t* xkey = &const_cast<value_t&>(key);
    auto i = this->storage.find(xkey);
    return i != this->storage.cend();
  }

  value_t::type_t dictionary_t::Type() const {
    return value_t::DICTIONARY;
  }
  
  value_t* dictionary_t::Copy() const {
    value_t* result = new dictionary_t(*this);
    return result;
  }

  void dictionary_t::Delete() {
    delete this;
  }

  std::string dictionary_t::ToText() const {
    return std::string("deci::dictionary_t");
  }
  
} // namespace deci
