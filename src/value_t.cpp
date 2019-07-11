#include <deci.hpp>

namespace deci {

  value_t::~value_t() {
    ;
  }

  value_t* nothing_t::Create(void*, size_t dataSize) const {
    if (dataSize != 0) {
      throw std::runtime_error("Can't create nothing");
    }
    return const_cast<nothing_t*>(this);
  }

}
