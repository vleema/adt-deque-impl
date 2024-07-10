#ifndef DEQUE_H
#define DEQUE_H

#include <algorithm>  // std::copy, std::equal, std::fill
#include <cassert>    // assert()
#include <cstddef>    // std::size_t
#include <iostream>   // std::cout, std::endl
#include <iterator>   // std::advance, std::begin(), std::end(), std::ostream_iterator
#include <memory>     // std::unique_ptr
using std::shared_ptr;
#include <array>
#include <sstream>
#include <vector>
#include <initializer_list>  // std::initializer_list

/// Sequence container namespace.
namespace sc {

// Forward declaration. This is necessary so that we can state
// that deque is a friend of MyIterator.
// Inside deque we need access to the private members of MyIterator.
template <typename T, size_t BlockSize = 3, size_t DefaultBlkMapSize = 1>
class deque;

template <typename T, size_t BlockSize, typename BlockItr, typename ItemItr>
class MyIterator {
public:
  //== Typical iterator aliases
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;

  /// Default constructor
  MyIterator() = default;
  /// Copy constructor

private:
  BlockItr block;   //!< The block the iterator points to.
  ItemItr current;  //!< The last location where an insertion happened inside the block.

  // We need to grant this friendship to allow deque access to the iterator's private attributes.
  friend class deque<T>;
};

template <typename T, size_t BlockSize, size_t DefaultBlkMapSize>
class deque {
public:
  //== Typical container aliases
  using size_type = unsigned long;            //!< The size type.
  using value_type = T;                       //!< The value type.
  using pointer = value_type*;                //!< Pointer to a value stored in the container.
  using reference = value_type&;              //!< Reference to a value.
  using const_reference = const value_type&;  //!< Const reference to a value.
  using difference_type = ptrdiff_t;          //!< Difference type between pointers.

  //== Aliases for the deque types.
  /// A block is a fixed sized array of T that actually holds the data.
  using block_t = std::array<T, BlockSize>;
  /// Basic smart pointer to a block of data items.
  using block_sptr_t = std::shared_ptr<block_t>;
  /// This type represents a list of smart pointers to blocks of memory.
  using block_list_t = std::vector<block_sptr_t>;
  /// Regular iterator.
  using iterator
    = MyIterator<T, BlockSize, typename block_list_t::iterator, typename block_t::iterator>;
  /// Const iterator.
  using const_iterator = MyIterator<const T,
                                    BlockSize,
                                    typename block_list_t::const_iterator,
                                    typename block_t::const_iterator>;

private:
  //== Management variables.
  std::shared_ptr<block_list_t> m_mob;     //!< The dynamic map of blocks.
  iterator m_head_itr;                     //!< Iterator to the head block.
  iterator m_tail_itr;                     //!< Iterator to the tail block.
  size_t m_count{ 0 };                     //!< # of elements stored in the map.
  size_t m_map_size{ DefaultBlkMapSize };  //!< Current length of the map.

public:
  /*! Construct a deque and initialize it with `n` copies of `value` of type `T`. If `value` is
   * not provided, a default constructor `T()` is used.
   */
  deque(size_type n = 0, const_reference value = T()) {}

  /// Clear the deque of all elements by resetting the control iterators to middle of the map.
  void clear() {}

  /// Return the number of elements in the deque.
  [[nodiscard]] size_type size() const { return m_count; }

  /// Return `true` if the deque has no elements, `false` otherwise.
  [[nodiscard]] bool empty() const { return m_count == 0; }

  /// Return an iterator to the deque's first element.
  iterator begin() { return iterator(); }

  /// Return an iterator to a location following the deque's last element.
  iterator end() { return iterator(); }

  /// Insert `value` at the begining of the deque.
  void push_front(const_reference value) { m_count++; }

  /// Insert `value` at the end of the deque.
  void push_back(const_reference value) { m_count++; }

  /// Remove the first element of the deque.
  void pop_front() { m_count--; }

  /// Remove the last element of the deque.
  void pop_back() { m_count--; }

  /// Inserts the value at location pointed by `pos`.
  iterator insert(const_iterator pos, const_reference value) {}

  /// Returns a reference to the element at specified location `pos`. No bounds checking is
  /// performed.
  reference operator[](size_type idx) { return nullptr; }

  [[nodiscard]] std::string to_string() const {}
};

}  // namespace sc

#endif
