#pragma once
#ifndef __DECI_REFERENCE_HEADER__
#define __DECI_REFERENCE_HEADER__

namespace deci {

  class reference_t: public value_t
  {
    value_t*    ref;
    mutable int refCount;

    reference_t(value_t* ref):value_t(),ref(ref),refCount(0) {
      ;
    }

    ~reference_t() {
      ref->Delete();
    }

    size_t DoHashing() const override {
      throw std::runtime_error("Invalid Operation");
    }

  public:

    static reference_t* Create(value_t* ref);

    value_t* Release();

    void Capture(value_t* newRef);

    value_t& Dereference() const {
      return *this->ref;
    }

    value_t::type_t Type() const override;

    value_t* Copy() const override;

    void Delete() override;

    std::string ToText() const override;

    value_t *Create(void *data, size_t dataSize) const override;
  };
  
} // namespace deci

#endif /* __DECI_REFERENCE_HEADER__ */