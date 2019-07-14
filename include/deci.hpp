/**
 * @brief deci VM main header
 * @file deci.hpp
 * @author timur
 */

#pragma once
#ifndef __DECI_HEADER__
#define __DECI_HEADER__

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "deci/value_t.hpp"
#include "deci/number_t.hpp"
#include "deci/string_t.hpp"
#include "deci/reference_t.hpp"
#include "deci/array_t.hpp"
#include "deci/dictionary_t.hpp"
#include "deci/stack_t.hpp"
#include "deci/vm_t.hpp"
#include "deci/function_t.hpp"
#include "deci/func_lib.hpp"
#include "deci/program_t.hpp"
#include "deci/compiler_t.hpp"
#include "deci/ast_t.hpp"

namespace deci {
  
  template <typename T, size_t N>
  constexpr size_t countof(T const (&)[N]) noexcept
  {
    return N;
  }

  template<typename T>
  constexpr size_t countof(T const &item) noexcept
  {
    return item.size();
  }

  inline std::string ToUpper(const std::string& src) {
    std::string result;
    result.reserve(src.size());
    for (auto ch: src) {
      result.push_back(toupper(ch));
    }
    return result;
  }

  inline std::string ToLower(const std::string& src) {
    std::string result;
    result.reserve(src.size());
    for (auto ch: src) {
      result.push_back(tolower(ch));
    }
    return result;
  }

  enum readObjectType_t: int {
    STREAM_UNDEF  = -1,
    STREAM_NUMBER = 'N',
    STREAM_STRING = 'S'
  };

  template<typename T>
  struct SerializableTraits {
    enum {
      SUPPORTED = false,
      STREAM_TYPE = STREAM_UNDEF
    };
  };

  template<>
  struct SerializableTraits<number_t> {
    enum {
      SUPPORTED = true,
      STREAM_TYPE = STREAM_NUMBER
    };
  };

  template<>
  struct SerializableTraits<string_t> {
    enum {
      SUPPORTED = true,
      STREAM_TYPE = STREAM_STRING
    };
  };

  template<typename T>
  T* ReadObject(std::istream& input)
  {
    typedef SerializableTraits<T> TypeTraits;

    static_assert(TypeTraits::SUPPORTED, "Unsupported Type");

    int objType = input.peek();
    if (TypeTraits::STREAM_TYPE != objType)
    {
      return nullptr;
    }
    input.ignore(1);

    try 
    {
      T* result = new T(input);
      if (input.peek() == 0) {
        input.ignore(1);
        return result;
      }
      delete result;
    } catch (const std::bad_alloc&) {
      return nullptr;
    }
    return nullptr;
  }

  value_t* ReadObject(std::istream& input);

  template<typename T>
  void WriteObject(std::ostream& output, const T& obj)
  {
    typedef SerializableTraits<T> TypeTraits;

    static_assert(TypeTraits::SUPPORTED, "Unsupported Type");

    output.put(TypeTraits::STREAM_TYPE);
    obj.Serialize(output);
    output.put(0);
  }

  void WriteObject(std::ostream& output, const value_t& obj);

}

#endif /* __DECI_HEADER__ */
