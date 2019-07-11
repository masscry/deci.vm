/**
 * @file array_t.hpp
 * @brief linear array value_t implementation
 * 
 * array_t based on std::vector, stores copies of elements which can be accessed by index
 */

#pragma once
#ifndef __DECI_ARRAY_HEADER__
#define __DECI_ARRAY_HEADER__

namespace deci
{
  /**
   * Linear array.
   */
  class array_t: public value_t {
    typedef std::vector<value_t*> element_t;

    element_t elements;

    /**
     * This rises exception if used.
     */
    size_t DoHashing() const override;

  public:

    array_t();

    array_t(const array_t& copy);

    ~array_t();

    /**
     * Return true, when array has no elements.
     */
    bool Empty() const;

    /**
     * Return current total count of elements.
     */
    size_t Size() const;

    /**
     * Get reference to element in array by index.
     */
    value_t& Element(int index) const;

    /**
     * Replace element in array.
     */
    void Set(int index, const value_t& el);

    /**
     * Insert new element at place.
     */
    void Insert(int index, const value_t& el);

    /**
     * Add new element after last.
     */
    void Append(const value_t& el);

    /**
     * Remove last element.
     */
    void Pop();

    /**
     * Return value_t::ARRAY
     */
    value_t::type_t Type() const override;

    /**
     * Creates deep copy.
     */
    value_t* Copy() const override;

    void Delete() override;

    std::string ToText() const override;

    value_t *Create(void *data, size_t dataSize) const override;

  };

} // namespace deci

#endif /* __DECI_ARRAY_HEADER__ */