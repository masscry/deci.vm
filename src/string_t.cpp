#include <deci.hpp>

namespace deci {

    string_t& string_t::operator = (const string_t& copy) {
      if (this != &copy) {
        this->val = copy.val;
      }
      return *this;
    }

    value_t::type_t string_t::Type() const {
      return value_t::STRING;
    }

    value_t* string_t::Copy() const {
      return new string_t(*this);
    }

    void string_t::Delete() {
      delete this;
    }

    std::string string_t::ToText() const {
      return this->val;
    }

    string_t::string_t():value_t(),val() {
      ;
    }

    string_t::string_t(const std::string& copy):value_t(),val(copy) {
      ;
    }

    string_t::string_t(const string_t& copy):value_t(copy),val(copy.val) {
      ;
    }

    string_t::~string_t() {
      ;
    }

    value_t* string_t::Create(void* data, size_t dataSize) const {
      if ((data == nullptr) || (dataSize == 0)) {
        throw std::runtime_error("Can't create string");
      }
      return new string_t(static_cast<char*>(data));
    }

}