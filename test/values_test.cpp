#include <deci.hpp>

void test_nothing() {
  using namespace deci;

  value_t* item = nothing_t::Instance().Copy();
  assert(item != 0);
  assert(item->Type() == value_t::NOTHING);
  assert(item->Hash() == 0);
  assert(item->Copy() == item);
  assert(item->ToText().compare("deci::nothing") == 0);
}

void test_number() {
  using namespace deci;

  assert(number_t(100.0).Value() == 100.0);
  assert(number_t(200.0).Value() == 200.0);
  assert(number_t(0.0).Value() == 0.0);

  value_t* item = number_t(123.0).Copy();
  assert(item->Type() == value_t::NUMBER);
  assert(item->Hash() == number_t(123.0).Hash());
  value_t* copy = item->Copy();
  assert(item != copy);
  assert(value_hash_t()(item) == value_hash_t()(copy));
  assert(value_equal_to_t()(item, copy) == true);

  copy->Delete();
  copy = number_t(100.0).Copy();
  assert(value_equal_to_t()(item, copy) == false);

  copy->Delete();
  item->Delete();
}

void test_array() {
  using namespace deci;

  array_t array;

  assert(array.Empty() == true);
  assert(array.Size() == 0);
  assert(array.ToText().compare("deci::array_t") == 0);
  assert(array.Type() == value_t::ARRAY);

  try {
    // Hashing for array forbidden
    size_t hash = array.Hash();
    assert(0);
  } catch (std::runtime_error&) {
    ;
  }

  try {
    // Can't pop from empty array
    array.Pop();
    assert(0);
  } catch (std::runtime_error&) {
    ;
  }

  try {
    // Can't set to empty place
    array.Set(0, number_t(100));
    assert(0);
  } catch (std::out_of_range&) {
    ;
  }

  try {
    // Can't get from empty place
    array.Element(0);
    assert(0);
  } catch (std::out_of_range&) {
    ;
  }

  array.Append(number_t(123.0));

  assert(array.Empty() == false);
  assert(array.Size() == 1);
  
  assert(array.Element(0).Type() == value_t::NUMBER);
  assert(array.Element(0).Hash() == number_t(123.0).Hash());
  assert(array.Element(0).ToText().compare(number_t(123.0).ToText()) == 0);

  array.Set(0, number_t(1.0));
  assert(array.Element(0).Type() == value_t::NUMBER);
  assert(array.Element(0).Hash() == number_t(1.0).Hash());
  assert(array.Element(0).ToText().compare(number_t(1.0).ToText()) == 0);

  array_t* a = (array_t*) array.Copy();
  assert(a != &array);
  assert(a->Type() == array.Type());
  assert(a->Element(0).Hash() == number_t(1.0).Hash());
  assert(&a->Element(0) != &array.Element(0));

  a->Delete();
}

int main(int argc, char* argv[]) {

  // NOTHING
  test_nothing();

  // NUMBER
  test_number();

  // STRING
  
  // ARRAY
  test_array();

  // DICTIONARY
  // FUNCTION
  // REFERENCE


  return 0;
}