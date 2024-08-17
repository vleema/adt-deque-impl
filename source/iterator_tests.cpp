#include <deque>
#include <iostream>

#include "deque.h"
#include "tm/test_manager.h"

#define which_lib sc
// To run tests with the STL's deque, uncomment the line below.
// #define which_lib std

#define YES 1
#define NO  0

// =============================================================
// Second batch of tests, focused on the iterator interface
// =============================================================

// Test begin() method that should return a iterator to the first element in the
// container.
#define BEGIN NO
// Test cbegin() method that should return a const iterator to the first element
// in the container.
#define CBEGIN NO
// Test end() method that should return a iterator to a location just past the
// last elemento of a container.
#define END NO
// Test cend() method that should return a const iterator to a location just
// past the last elemento of a container.
#define CEND NO
// Pre-increment operator. ++it
#define PRE_INC NO
// Post-increment operator. it++
#define POST_INC NO
// Pre-decrement operator. --it
#define PRE_DEC NO
// Post-decrement operator. it--
#define POST_DEC NO
// Dreference operator. *it
#define DEREFERENCE NO
// Differentre between iterators. it1 - it2
#define DIFFERENCE NO
// Add a offset to a iterator. offset + it
#define ADD_OFFSET_IT NO
// Add an iterator and an offset. it + offset
#define ADD_IT_OFFSET NO
// Subtract an iterator and an offset. it - offset
#define SUB_IT_OFFSET YES
// Self addition iterator and offset. it += 0ffset
#define SELF_ADD_IT_OFFSET NO
// Self subtraction iterator and offset. it -= 0ffset
#define SELF_SUB_IT_OFFSET NO
// Less operator. it1 < it2
#define LESS NO
// Greater operator. it1 > it2
#define GREATER NO
// Less equal operator. it1 <= it2
#define LESS_EQ NO
// Greater equal operator. it1 >= it2
#define GREATER_EQ NO
// Equal operator. it1 == it2
#define EQUAL NO
// Different operator. it1 != it2
#define DIFFERENT NO

void run_iterator_tests() {
  TestManager tm{ "Iterator testing" };

#if BEGIN
  {
    BEGIN_TEST(tm, "begin", "dq.begin()");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it = dq.begin();
    EXPECT_EQ(*it, dq[0]);

    auto dq2 = dq;
    dq2[0] = 100;  // Changing the copy
    it = dq2.begin();
    EXPECT_NE(*it, dq[0]);
    EXPECT_EQ(*it, dq2[0]);

    auto dq3{ dq };
    dq3[0] = 200;  // Changing the copy
    it = dq3.begin();
    EXPECT_NE(*it, dq[0]);
    EXPECT_EQ(*it, dq3[0]);

    which_lib::deque<int> vec4 = { 1, 2, 4, 5, 6 };
    it = vec4.begin();
    EXPECT_EQ(*it, vec4[0]);
  }
#endif

#if CBEGIN
  {
    BEGIN_TEST(tm, "cbegin", "dq.cbegin()");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto cit = dq.cbegin();
    EXPECT_EQ(*cit, dq[0]);

    auto dq2 = dq;
    dq2[0] = 100;  // Changing the copy
    cit = dq2.cbegin();
    EXPECT_NE(*cit, dq[0]);
    EXPECT_EQ(*cit, dq2[0]);

    auto dq3{ dq };
    dq3[0] = 200;  // Changing the copy
    cit = dq3.cbegin();
    EXPECT_NE(*cit, dq[0]);
    EXPECT_EQ(*cit, dq3[0]);

    which_lib::deque<int> vec4 = { 1, 2, 4, 5, 6 };
    cit = vec4.cbegin();
    EXPECT_EQ(*cit, vec4[0]);
  }
#endif

#if END
  {
    BEGIN_TEST(tm, "end", "dq.end()");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto len = std::distance(dq.begin(), dq.end());
    EXPECT_EQ(len, 5);

    auto it = dq.begin() + dq.size();
    EXPECT_EQ(it, dq.end());

    auto dq2 = dq;
    it = dq2.end();
    EXPECT_NE(it, dq.end());
    EXPECT_EQ(it, dq2.end());

    auto dq3{ dq };
    it = dq3.end();
    EXPECT_NE(it, dq.end());
    EXPECT_EQ(it, dq3.end());

    which_lib::deque<int> vec4 = { 1, 2, 4, 5, 6 };
    it = vec4.end();
    EXPECT_EQ(it, vec4.end());
  }
#endif

#if CEND
  {
    BEGIN_TEST(tm, "cend", "dq.cend()");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it = dq.cend();
    EXPECT_EQ(it, dq.cend());

    auto dq2 = dq;
    it = dq2.cend();
    EXPECT_NE(it, dq.cend());
    EXPECT_EQ(it, dq2.cend());

    auto dq3{ dq };
    it = dq3.cend();
    EXPECT_NE(it, dq.cend());
    EXPECT_EQ(it, dq3.cend());

    which_lib::deque<int> vec4 = { 1, 2, 4, 5, 6 };
    it = vec4.cend();
    EXPECT_EQ(it, vec4.cend());
  }
#endif

#if PRE_INC
  {
    BEGIN_TEST(tm, "operator++()", "Preincrement, ++it");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it = dq.begin();
    size_t i{ 0 };
    while (it != dq.end()) {
      EXPECT_EQ(*it, dq[i++]);
    }
  }
#endif

#if POST_INC
  {
    BEGIN_TEST(tm, "operator++(int)", "Postincrement, it++");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it = dq.begin();
    size_t i{ 0 };
    while (it != dq.end()) {
      // same address
      EXPECT_EQ(*it, dq[i++]);
      it++;
    }
  }
#endif

#if PRE_DEC
  {
    BEGIN_TEST(tm, "operator--()", "Predecrement, --it");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it = std::prev(dq.end());
    size_t i{ dq.size() };
    while (it != dq.begin()) {
      // std::cout << it << " == " << &dq[i] << "\n";
      // same address
      EXPECT_EQ(*it, dq[--i]);
      --it;
    }
  }
#endif

#if POST_DEC
  {
    BEGIN_TEST(tm, "operator--(int)", "Postdecrement, it--");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it = std::prev(dq.end());
    size_t i{ dq.size() };
    while (it != dq.begin()) {
      // std::cout << it << " == " << &dq[i] << "\n";
      // same address
      EXPECT_EQ(*it, dq[--i]);
      it--;
    }
  }
#endif

#if DEREFERENCE
  {
    BEGIN_TEST(tm, "operator*()", " x = *it1");

    which_lib::deque<int> dq{ 1, 2, 3, 4, 5, 6 };

    auto it = dq.begin();
    int i{ 1 };
    while (it != dq.end())
      EXPECT_EQ(*it++, i++);
  }
#endif

#if DIFFERENCE
  {
    BEGIN_TEST(tm, "operator-()", "it1 - it2");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it1 = dq.begin();
    auto it2 = dq.begin();
    ptrdiff_t i{ 0 };
    while (it1 != dq.end()) {
      // same address
      EXPECT_EQ(it1 - it2, i);
      i++;
      it1++;
    }
  }
#endif

#if ADD_OFFSET_IT
  {
    BEGIN_TEST(tm, "operator+(int, iterator)", "it = 2 + it");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it = dq.begin();
    for (size_t i{ 0 }; i < dq.size(); ++i) {
      // same address
      EXPECT_EQ(*(i + it), dq[i]);
      // std::cout << (i+it) << " == " << &dq[i] << "\n";
    }
  }
#endif

#if ADD_IT_OFFSET
  {
    BEGIN_TEST(tm, "operator+(iterator, int)", "it = it + 2");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it = dq.begin();
    for (size_t i{ 0 }; i < dq.size(); ++i) {
      // same address
      EXPECT_EQ(*(it + i), dq[i]);
      // std::cout << (i+it) << " == " << &dq[i] << "\n";
    }
  }
#endif

#if SUB_IT_OFFSET
  {
    BEGIN_TEST(tm, "operator-(iterator, int)", "it = it - 2");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it = dq.end() - 1;
    for (size_t i{ 0 }; i < dq.size(); ++i) {
      // same address
      EXPECT_EQ(*(it - i), dq[dq.size() - i - 1]);
    }
  }
#endif

#if SELF_ADD_IT_OFFSET
  {
    BEGIN_TEST(tm, "operator+=()", "it += n");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    for (size_t i{ 0 }; i < dq.size(); ++i) {
      auto it = dq.begin();
      it += i;
      // same address
      EXPECT_EQ(*it, dq[i]);
    }
  }
#endif

#if SELF_SUB_IT_OFFSET
  {
    BEGIN_TEST(tm, "operator-=()", "it -= n");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    for (size_t i{ 0 }; i < dq.size(); ++i) {
      auto it = dq.end();
      it -= (dq.size() - i);
      // same address
      EXPECT_EQ(*it, dq[i]);
    }
  }
#endif

#if LESS
  {
    BEGIN_TEST(tm, "operator<()", "it1 < it2");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it1 = dq.begin();
    auto it2 = dq.end();
    while (it1 != dq.end()) {
      EXPECT_TRUE(it1 < it2);
      ++it1;
    }
  }
#endif

#if GREATER
  {
    BEGIN_TEST(tm, "operator>()", "it1 > it2");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it1 = dq.begin();
    auto it2 = dq.end();
    while (it1 != dq.end()) {
      EXPECT_TRUE(it2 > it1);
      ++it1;
    }
  }
#endif

#if LESS_EQ
  {
    BEGIN_TEST(tm, "operator<=()", "it1 <= it2");

    which_lib::deque<int> dq{ 1, 2, 3, 4, 5, 6 };

    auto it1 = dq.begin();
    auto it2 = dq.end();
    while (it1 != dq.end()) {
      EXPECT_TRUE(it1 <= it2);
      ++it1;
    }
  }
#endif

#if GREATER_EQ
  {
    BEGIN_TEST(tm, "operator>=()", "it1 >= it2");

    which_lib::deque<int> dq{ 1, 2, 3, 4, 5, 6 };

    auto it1 = dq.begin();
    auto it2 = dq.end();
    while (it1 != dq.end()) {
      EXPECT_TRUE(it2 >= it1);
      ++it1;
    }
  }
#endif

#if EQUAL
  {
    BEGIN_TEST(tm, "operator==()", "it1 == it2");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it1 = dq.begin();
    auto it2 = dq.begin();
    while (it1 != dq.end())
      EXPECT_EQ(it1++, it2++);
  }
#endif

#if DIFFERENT
  {
    BEGIN_TEST(tm, "operator!=()", "it1 != it2");

    which_lib::deque<int> dq{ 1, 2, 4, 5, 6 };

    auto it1 = dq.begin();
    auto it2 = dq.end();
    while (it1 != it2) {
      EXPECT_TRUE(it1 != it2);
      ++it1;
    }
    EXPECT_FALSE(it1 != it2);
  }
#endif

  tm.summary();
}
