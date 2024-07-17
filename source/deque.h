#ifndef DEQUE_H
#define DEQUE_H

#include <algorithm>
#include <cassert>  // assert()
#include <cstddef>  // std::size_t
#include <iostream>
#include <iterator>  // std::advance, std::begin(), std::end(), std::ostream_iterator
#include <memory>    // std::unique_ptr
#include <type_traits>
using std::shared_ptr;
#include <array>
#include <vector>

/// Sequence container namespace.
namespace sc {

// Forward declaration. This is necessary so that we can state
// that deque is a friend of MyIterator.
// Inside deque we need access to the private members of MyIterator.
template <typename T, size_t BlockSize = 3, size_t DefaultBlkMapSize = 1>
class deque;

template <typename T, size_t BlockSize, typename BlockItr, typename ItemItr>
class MyIterator {
public:  //== Typical iterator aliases
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;

  /// Default constructor
  MyIterator() = default;
  /// Constructor with block and item iterators
  MyIterator(BlockItr block, ItemItr current) : M_block(block), M_current(current) {}
  /// Copy constructor
  MyIterator(const MyIterator& other)
      : M_block(BlockItr(other.M_block)), M_current(ItemItr(other.M_current)) {}
  /// Copy assignment operator
  MyIterator& operator=(const MyIterator& other) {
    if (this != &other) {
      M_block = BlockItr(other.M_block);
      M_current = ItemItr(other.M_current);
    }
    return *this;
  }
  /// Default destructor
  ~MyIterator() = default;
  /// Pre-Increment operator
  MyIterator& operator++() {
    if (M_current == (*M_block)->end() - 1) {
      ++M_block;
      M_current = (*M_block)->begin();
      return *this;
    }
    ++M_current;
    return *this;
  }

  /// Post-Increment operator
  MyIterator operator++(int) {
    MyIterator temp(*this);
    ++(*this);
    return temp;
  }

  /// Pre-Decrement operator
  MyIterator& operator--() { return *this -= 1; }  // FIX: No way this shit it working

  /// Post-Decrement operator
  MyIterator operator--(int) {
    MyIterator temp(*this);
    --(*this);
    return temp;
  }

  /// Dereference operator
  reference operator*() { return *M_current; }

  /// Arrow operator
  pointer operator->() { return &(*M_current); }

  /// Difference between iterators
  difference_type operator-(const MyIterator& other) {
    auto block_diff = std::distance(other.M_block, this->M_block);
    auto diff_to_end = std::distance(this->M_current, (*this->M_block)->end());
    auto diff_to_start = std::distance((*other.M_block)->begin(), other.M_current);
    return diff_to_end + (block_diff - 1) * BlockSize
           + diff_to_start;  // returns the distance [this, other)
  }

  /// Right sum of iterator and integer
  friend MyIterator operator+(int n, MyIterator it) {
    auto current_index = std::distance((*it.M_block)->begin(), it.M_current);
    auto total_index = current_index + n;
    auto blocks_to_advance = total_index / BlockSize;
    std::advance(it.M_block, blocks_to_advance);
    it.M_current = std::next((*it.M_block)->begin(), total_index % BlockSize);
    return it;
  }

  /// Left sum of iterator and integer
  friend MyIterator operator+(MyIterator it, int n) { return n + it; }

  /// Right Difference of iterator and integer
  friend MyIterator operator-(int n, MyIterator it) { return it + (-n); }

  /// Addition assignment operator
  MyIterator& operator+=(int n) { return *this = *this + n; }

  /// Difference assignment operator
  MyIterator& operator-=(int n) { return *this += -n; }
  /// If a iterator is a lower position than another iterator, with lexicographic order
  bool operator<(const MyIterator& other) const {
    return M_block < other.M_block or (M_block == other.M_block and M_current < other.M_current);
  }

  /// If a iterator is a greater position than another iterator, with lexicographic order
  bool operator>(const MyIterator& other) const { return other < *this; }

  /// If a iterator is in the same position then another
  bool operator==(const MyIterator& other) const {
    return M_block == other.M_block and M_current == other.M_current;
  }

  /// If a iterator is in a lower or equal position then another
  bool operator<=(const MyIterator& other) const { return *this < other or *this == other; }

  /// If a iterator is in a greater or equal position then another
  bool operator>=(const MyIterator& other) const { return *this > other or *this == other; }

  /// If a iterator is in a different position then another
  bool operator!=(const MyIterator& other) const { return not(*this == other); }

private:
  BlockItr M_block;   //!< The block the iterator points to.
  ItemItr M_current;  //!< The last location where an insertion happened inside the block.

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
  std::shared_ptr<block_list_t> M_mob;     //!< The dynamic map of blocks.
  iterator M_head_itr;                     //!< Iterator to the head block.
  iterator M_tail_itr;                     //!< Iterator to the tail block.
  size_t M_count{ 0 };                     //!< # of elements stored in the map.
  size_t M_map_size{ DefaultBlkMapSize };  //!< Current length of the map.

  void allocate_all_blocks() {
    for (auto& block : *M_mob) {
      block = std::make_shared<block_t>();
    }
  }

  void reset() {
    auto middle_block_itr = std::next(M_mob->begin(), M_map_size / 2);
    auto current_middle_itr = std::next((*middle_block_itr)->begin(), BlockSize / 2);
    M_head_itr = M_tail_itr = iterator(middle_block_itr, current_middle_itr);
    M_count = 0;
  }

  template <typename InputIt, typename = typename std::iterator_traits<InputIt>::iterator_category>
  void initialize_from_range(InputIt first, InputIt last) {
    auto num_values = std::distance(first, last);
    M_map_size = (num_values + BlockSize) / BlockSize;
    M_mob = std::make_shared<block_list_t>(M_map_size, nullptr);
    allocate_all_blocks();
    M_head_itr = iterator(M_mob->begin(), (*M_mob->begin())->begin());
    M_tail_itr = M_head_itr + num_values;
    std::copy(first, last, M_head_itr);
    M_count = num_values;
  }

public:
  /// Default Constructor.
  deque() {
    M_mob = std::make_shared<block_list_t>(M_map_size, nullptr);
    allocate_all_blocks();
    reset();
  }

  /// Construct a deque with `count` copies of `value`.
  deque(size_type count, const_reference value = T()) {
    const std::vector<value_type> temp(count, value);
    initialize_from_range(temp.cbegin(), temp.cend());
  }

  /// Construct a deque from a range of elements [first, last).
  template <typename InputIt,
            typename = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
  deque(InputIt first, InputIt last) {
    initialize_from_range(first, last);
  }

  ~deque() = default;

  /// Construct a deque from an initializer list.
  deque(std::initializer_list<T> il) : deque(il.begin(), il.end()) {}

  /// Copy constructor.
  deque(const deque& other) : deque(other.begin(), other.end()) {}

  /// Clear the deque of all elements by resetting the control iterators to middle of the map.
  void clear() {}

  /// Return the number of elements in the deque.
  [[nodiscard]] size_type size() const { return M_count; }

  /// Return `true` if the deque has no elements, `false` otherwise.
  [[nodiscard]] bool empty() const { return M_count == 0; }

  /// Return an iterator to the deque's first element.
  iterator begin() { return M_head_itr; }

  /// Return an iterator to a location following the deque's last element.
  iterator end() { return M_tail_itr; }

  /// Insert `value` at the begining of the deque.
  void push_front(const_reference value) { M_count++; }

  /// Insert `value` at the end of the deque.
  void push_back(const_reference value) {}

  /// Remove the first element of the deque.
  void pop_front() { M_count--; }

  /// Remove the last element of the deque.
  void pop_back() { M_count--; }

  /// Inserts the value at location pointed by `pos`.
  iterator insert(const_iterator pos, const_reference value) {}

  /// Returns a reference to the element at specified location `pos`. No bounds checking is
  /// performed.
  reference operator[](size_type idx) { return *(M_head_itr + idx); }

  [[nodiscard]] std::string to_string() const { return "hi"; }
};

}  // namespace sc

#endif
