#ifndef DEQUE_H
#define DEQUE_H

#include <cassert>   // assert()
#include <cstddef>   // std::size_t
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
  MyIterator(BlockItr block, ItemItr current) : _M_block(block), _M_current(current) {}
  /// Copy constructor
  MyIterator(const MyIterator& other)
      : _M_block(BlockItr(other._M_block)), _M_current(ItemItr(other._M_current)) {}
  /// Copy assignment operator
  MyIterator& operator=(const MyIterator& other) {
    if (this != &other) {
      _M_block = BlockItr(other._M_block);
      _M_current = ItemItr(other._M_current);
    }
    return *this;
  }
  /// Default destructor
  ~MyIterator() = default;
  /// Pre-Increment operator
  MyIterator& operator++() {
    if (++_M_current == (*_M_block)->end()) {
      ++_M_block;
      _M_current = (*_M_block)->begin();
    }
    return *this;
  }

  /// Post-Increment operator
  MyIterator operator++(int) {
    MyIterator temp(*this);
    ++(*this);
    return temp;
  }

  /// Pre-Decrement operator
  MyIterator& operator--() {
    if (_M_current == (*_M_block)->begin()) {
      --_M_block;
      _M_current = (*_M_block)->end();
    }
    --_M_current;
    return *this;
  }

  /// Post-Decrement operator
  MyIterator operator--(int) {
    MyIterator temp(*this);
    --(*this);
    return temp;
  }

  /// Dereference operator
  reference operator*() { return *_M_current; }

  /// Arrow operator
  pointer operator->() { return &(*_M_current); }

  /// Difference between iterators
  difference_type operator-(const MyIterator& other) {
    auto block_diff = std::distance(other._M_block, this->_M_block);
    auto diff_to_end = std::distance(this->_M_current, (*this->_M_block)->end());
    auto diff_to_start = std::distance((*other._M_block)->begin(), other._M_current);
    return diff_to_end + (block_diff - 1) * BlockSize
           + diff_to_start;  // returns the distance [this, other)
  }

  /// Right sum of iterator and integer
  friend MyIterator operator+(int n, MyIterator it) {
    auto current_index = std::distance((*it._M_block)->begin(), it._M_current);
    auto total_index = current_index + n;
    auto blocks_to_advance = total_index / BlockSize;
    std::advance(it._M_block, blocks_to_advance);
    std::advance(it._M_current, total_index % BlockSize);
    return it;
  }

  /// Left sum of iterator and integer
  friend MyIterator operator+(MyIterator it, int n) { return n + it; }

  /// Addition assignment operator
  MyIterator& operator+=(int n) { return *this = *this + n; }

  /// Difference assignment operator
  MyIterator& operator-=(int n) { return *this = *this - n; }

  /// Right Difference of iterator and integer
  friend MyIterator operator-(int n, MyIterator it) { return it + (-n); }

  /// If a iterator is a lower position than another iterator, with lexicographic order
  bool operator<(const MyIterator& other) const {
    return _M_block < other._M_block
           or (_M_block == other._M_block and _M_current < other._M_current);
  }

  /// If a iterator is a greater position than another iterator, with lexicographic order
  bool operator>(const MyIterator& other) const { return other < *this; }

  /// If a iterator is in the same position then another
  bool operator==(const MyIterator& other) const {
    return _M_block == other._M_block and _M_current == other._M_current;
  }

  /// If a iterator is in a lower or equal position then another
  bool operator<=(const MyIterator& other) const { return *this < other or *this == other; }

  /// If a iterator is in a greater or equal position then another
  bool operator>=(const MyIterator& other) const { return *this > other or *this == other; }

  /// If a iterator is in a different position then another
  bool operator!=(const MyIterator& other) const { return not(*this == other); }

private:
  BlockItr _M_block;   //!< The block the iterator points to.
  ItemItr _M_current;  //!< The last location where an insertion happened inside the block.

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
  std::shared_ptr<block_list_t> _M_mob;     //!< The dynamic map of blocks.
  iterator _M_head_itr;                     //!< Iterator to the head block.
  iterator _M_tail_itr;                     //!< Iterator to the tail block.
  size_t _M_count{ 0 };                     //!< # of elements stored in the map.
  size_t _M_map_size{ DefaultBlkMapSize };  //!< Current length of the map.

public:
  /*! Construct a deque and initialize it with `n` copies of `value` of type `T`. If `value` is
   * not provided, a default constructor `T()` is used.
   */
  explicit deque(size_type n = 0, const_reference value = T())
      : _M_mob(std::make_shared<block_list_t>()), _M_head_itr(), _M_tail_itr(), _M_count(0) {
    if (n == 0) {
      return;
    }
    for (size_type i = 0; i < n; ++i) {
      push_back(value);
    }
  }

  /// Construct a deque from a range of elements [first, last).
  template <typename InputIt,
            typename = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
  deque(InputIt first, InputIt last) : deque() {
    for (InputIt it = first; it != last; ++it) {
      push_back(*it);
    }
  }

  ~deque() = default;

  /// Construct a deque from an initializer list.
  deque(std::initializer_list<T> il) : deque(il.begin(), il.end()) {}

  /// Copy constructor.
  deque(const deque& other) : deque(other.begin(), other.end()) {}

  /// Clear the deque of all elements by resetting the control iterators to middle of the map.
  void clear() {}

  /// Return the number of elements in the deque.
  [[nodiscard]] size_type size() const { return _M_count; }

  /// Return `true` if the deque has no elements, `false` otherwise.
  [[nodiscard]] bool empty() const { return _M_count == 0; }

  /// Return an iterator to the deque's first element.
  iterator begin() { return _M_head_itr; }

  /// Return an iterator to a location following the deque's last element.
  iterator end() { return _M_head_itr; }

  /// Insert `value` at the begining of the deque.
  void push_front(const_reference value) { _M_count++; }

  /// Insert `value` at the end of the deque.
  void push_back(const_reference value) {
    if (empty()) {
      _M_mob->push_back(std::make_shared<block_t>());
      _M_head_itr = _M_tail_itr = iterator(_M_mob->begin(), (*_M_mob->begin())->begin());
    } else if (_M_tail_itr._M_current == (*_M_tail_itr._M_block)->end()) {
      _M_mob->push_back(std::make_shared<block_t>());
      _M_tail_itr = iterator(_M_mob->end() - 1, _M_mob->back()->begin());
    }
    *_M_tail_itr._M_current = value;
    _M_tail_itr._M_current++;
    _M_count++;
  }

  /// Remove the first element of the deque.
  void pop_front() { _M_count--; }

  /// Remove the last element of the deque.
  void pop_back() { _M_count--; }

  /// Inserts the value at location pointed by `pos`.
  iterator insert(const_iterator pos, const_reference value) {}

  /// Returns a reference to the element at specified location `pos`. No bounds checking is
  /// performed.
  reference operator[](size_type idx) { return *(_M_head_itr + idx); }

  [[nodiscard]] std::string to_string() const { return "hi"; }
};

}  // namespace sc

#endif
