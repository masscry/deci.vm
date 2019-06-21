#pragma once
#ifndef __DECI_ARRAY_HEADER__
#define __DECI_ARRAY_HEADER__

#include <deci.hpp>

namespace deci
{

  class array_t: public value_t {
    typedef std::vector<value_t*> element_t;

    element_t elements;

    ~array_t() {
      for (auto el: this->elements) {
        el->Delete();
      }
    }

  public:

    array_t():elements() {
      ;
    }

    array_t(const array_t& copy):elements() {
      this->elements.reserve(copy.elements.size());
      for (auto el: copy.elements) {
        this->elements.push_back(el->Copy());
      }
    }

    bool Empty() const {
      return this->elements.empty();
    }

    size_t Size() const {
      return this->elements.size();
    }

    value_t& Element(int index) const {
      return *this->elements.at(index);
    }

    void Set(int index, value_t& el) {
      this->elements.at(index)->Delete();
      this->elements.at(index) = el.Copy();
    }

    void Insert(int index, value_t& el) {
      this->elements.insert(this->elements.begin() + index, el.Copy());
    }

    void Append(value_t& el) {
      this->elements.push_back(el.Copy());
    }

    void Pop() {
      value_t* back = this->elements.back();
      this->elements.pop_back();
      back->Delete();
    }

    type_t Type() const override {
      return value_t::ARRAY;
    }

    value_t* Copy() const override {
      return new array_t(*this);
    }

    void Delete() override {
      delete this;
    }
    
    std::string ToText() const override {
      return std::string("deci::array_t");
    }

  };

} // namespace deci

#endif /* __DECI_ARRAY_HEADER__ */