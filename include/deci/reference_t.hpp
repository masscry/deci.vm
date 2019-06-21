#pragma once
#ifndef __DECI_REFERENCE_HEADER__
#define __DECI_REFERENCE_HEADER__

namespace deci {

  class reference_t: public value_t
  {
    value_t*    ref;
    mutable int refCount;

    reference_t(value_t* ref):ref(ref),refCount(0) {
      ;
    }

    ~reference_t() {
      ref->Delete();
    }

    size_t DoHashing() const override {
      throw std::runtime_error("Invalid Operation");
    }

  public:

    static reference_t* Create(value_t* ref) {
      reference_t* result = new reference_t(ref);
      result->refCount = 1;
      return result;
    }

    value_t* Release() {
      value_t* result = this->ref;
      this->ref = nothing_t::Instance().Copy();
      return result;
    }

    void Capture(value_t* newRef) {
      this->Release()->Delete();
      this->ref = newRef;
    }

    value_t& Dereference() const {
      return *this->ref;
    }

    value_t::type_t Type() const override {
      return value_t::REFERENCE;
    }

    value_t* Copy() const override {
      ++this->refCount;
      return const_cast<reference_t*>(this);
    }

    void Delete() override {
      --this->refCount;
      if (this->refCount <= 0) {
        delete this;
      }
    }

    std::string ToText() const override {
      return std::string("deci::reference_t");
    }

  };
  
} // namespace deci

#endif /* __DECI_REFERENCE_HEADER__ */