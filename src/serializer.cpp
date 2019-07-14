#include <deci.hpp>

namespace deci {

  value_t* ReadObject(std::istream& input)
  {
    switch(input.peek()) 
    {
      case STREAM_NUMBER:
        return ReadObject<number_t>(input);
      case STREAM_STRING:
        return ReadObject<string_t>(input);
      default:
        return 0;
    }
  }

  void WriteObject(std::ostream& output, const value_t& obj)
  {
    switch(obj.Type())
    {
      case value_t::NUMBER:
        (static_cast<const number_t&>(obj)).Serialize(output);
        break;
      case value_t::STRING:
        (static_cast<const string_t&>(obj)).Serialize(output);
        break;
      default:
        throw std::runtime_error("Can't write unsupported type");
    }
  }

}