#include <deci.hpp>

namespace deci
{

    array_t::~array_t() {
      for (auto el: this->elements) {
        el->Delete();
      }
    }

    size_t array_t::DoHashing() const {
      throw std::runtime_error("Invalid Operation");
    }

    array_t::array_t():value_t(),elements() {
      ;
    }

    array_t::array_t(const array_t& copy):value_t(),elements() {
      this->elements.reserve(copy.elements.size());
      for (auto el: copy.elements) {
        this->elements.push_back(el->Copy());
      }
    }

    bool array_t::Empty() const {
      return this->elements.empty();
    }

    size_t array_t::Size() const {
      return this->elements.size();
    }

    value_t& array_t::Element(int index) const {
      return *this->elements.at(index);
    }

    void array_t::Set(int index, const value_t& el) {
      this->elements.at(index)->Delete();
      this->elements.at(index) = el.Copy();
    }

    void array_t::Insert(int index, const value_t& el) {
      this->elements.insert(this->elements.begin() + index, el.Copy());
    }

    void array_t::Append(const value_t& el) {
      this->elements.push_back(el.Copy());
    }

    void array_t::Pop() {
      if (this->Empty()) {
        throw std::runtime_error("Empty Array");
      }
      value_t* back = this->elements.back();
      this->elements.pop_back();
      back->Delete();
    }

    value_t::type_t array_t::Type() const {
      return value_t::ARRAY;
    }

    value_t* array_t::Copy() const {
      return new array_t(*this);
    }

    void array_t::Delete() {
      delete this;
    }
    
    std::string array_t::ToText() const {
      return std::string("deci::array_t");
    }

  
} // namespace deci
