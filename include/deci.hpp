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
#include "deci/stack_t.hpp"
#include "deci/vm_t.hpp"
#include "deci/function_t.hpp"
#include "deci/func_lib.hpp"
#include "deci/program_t.hpp"
#include "deci/reference_t.hpp"
#include "deci/array_t.hpp"
#include "deci/dictionary_t.hpp"
#include "deci/compiler_t.hpp"

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

}

#endif /* __DECI_HEADER__ */
