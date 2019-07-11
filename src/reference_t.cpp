#include <deci.hpp>

namespace deci {
  
  reference_t* reference_t::Create(value_t* ref) {
    reference_t* result = new reference_t(ref);
    result->refCount = 1;
    return result;
  }

  value_t* reference_t::Release() {
    value_t* result = this->ref;
    this->ref = nothing_t::Instance().Copy();
    return result;
  }

  void reference_t::Capture(value_t* newRef) {
    this->Release()->Delete();
    this->ref = newRef;
  }

  value_t::type_t reference_t::Type() const {
    return value_t::REFERENCE;
  }

  value_t* reference_t::Copy() const {
    ++this->refCount;
    return const_cast<reference_t*>(this);
  }

  void reference_t::Delete() {
    --this->refCount;
    if (this->refCount <= 0) {
      delete this;
    }
  }

  std::string reference_t::ToText() const  {
    return std::string("deci::reference_t");
  }

  value_t* reference_t::Create(void*, size_t) const {
    //
    // TODO: in future maybe we may have some way to create it, but this requires
    //       some additional support from VM
    //
    throw std::runtime_error("Can't create references!");
  }

}