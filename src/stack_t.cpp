#include <deci.hpp>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace deci {

  void stack_t::Print(std::ostream& output) {
    size_t depth = this->Depth();

    output << "result: " << this->result->ToText() << std::endl;

    for (size_t i = 0; i < depth; ++i) {
      value_t& elem = this->Top(i);
      output << std::right << std::hex << std::setfill('0')
        << std::setw(4) << i << ": "
        << elem.ToText() << std::endl;
    }
  }

}
