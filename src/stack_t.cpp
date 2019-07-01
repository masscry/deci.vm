#include <deci.hpp>

namespace deci {

  const value_t& stack_t::Variable(const value_t& key) const {
    try {
      return this->context.Get(key);
    }
    catch (std::runtime_error&) {
      // Element not found
      return nothing_t::Instance();
    }
  }

  void stack_t::Variable(const value_t& key, const value_t& val) {
    this->context.Put(key, val);
  }

  void stack_t::MergeVariable(const stack_t& src) {
    this->context.Insert(src.context);
  }

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

  value_t* stack_t::ReleaseResult() {
    value_t* ret = this->result;
    this->result = &nothing_t::Instance();
    return ret;
  }
  
  value_t& stack_t::Result() const {
    return *this->result;
  }
  
  void stack_t::Result(const value_t& val) {
    this->result = val.Copy();
  }
  
  void stack_t::Push(const value_t& val) {
    this->storage.push_back(val.Copy());
  }

  value_t* stack_t::Pop() {
    if (!this->storage.empty()) {
      value_t* result = this->storage.back();
      this->storage.pop_back();
      return result;
    }
    throw std::runtime_error("Stack is empty");
  }

  value_t& stack_t::Top(size_t depth) const {
    size_t size = this->storage.size();
      
    if (depth >= size)
    {
      throw std::runtime_error("Stack Is Too Small");
    }      
    return *this->storage[size-depth-1];
  }

  void stack_t::Drop(int total) {
    for (int i = 0; i < total; ++i) {
      this->Pop()->Delete();
    }
  }

  bool stack_t::Empty() const {
    return this->storage.empty();
  }
    
  size_t stack_t::Depth() const {
    return this->storage.size();
  }

  stack_t::stack_t()
    : storage()
    , context()
    , result(&nothing_t::Instance()) 
  {
    storage.reserve(8);
  }

  stack_t& stack_t::operator = (const stack_t& copy) {
    if (this != &copy) {
      this->storage = copy.storage;
      this->context = copy.context;
    }
    return *this;
  }

  stack_t& stack_t::operator = (stack_t&& move) {
    if (this != &move) {
      this->storage = std::move(move.storage);
      this->context = std::move(move.context);
    }
    return *this;
  }

  void Print(std::ostream& output);
    
  stack_t::stack_t(const stack_t& copy)
    : storage(copy.storage)
    , context(copy.context)
    , result(&nothing_t::Instance())
  {
  }

  stack_t::stack_t(stack_t&& move)
    : storage(std::move(move.storage))
    , context(std::move(move.context)) 
    , result(move.result)
  {
      move.result = nothing_t::Instance().Copy();
  }

  stack_t::~stack_t() {
    this->result->Delete();
    this->Drop(this->Depth());
  }

}
