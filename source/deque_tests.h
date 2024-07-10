#ifndef _deque_TESTS_H_
#define _deque_TESTS_H_

#include <cstdlib>
#include <iostream>

#include "tm/test_manager.h"

#include "deque.h"

// #define which_lib sc
// To run tests with the STL's deque, uncomment the line below.
#define which_lib std

#define YES 1
#define NO  0

// ============================================================================
// TESTING deque AS A CONTAINER OF INTEGERS
// ============================================================================

// Test default ctro's size and capacity initial values.
#define DEFAULT_CTRO YES
// Receives a size and a 'value' as arguments. It crates an empty deque with size() 'values'.
#define CTRO_SIZE_VALUE YES
// Ctro that receives a size as argument. It crates an empty deque with size elements.
#define CTRO_SIZE YES
// Ctro that receives a range of values as its initial value.
#define CTRO_RANGE YES
// Copy Ctro: creates a deque based on another passed in as argument.
#define CTRO_COPY YES
// Ctro that receives a list of values as its initial value.
#define LIST_CTRO YES
// Assign operator, as in dq1 = dq2;
#define ASSIGN_OP YES
// Initializer list assignment, as in deque<int> dq = { 1, 2, 3 };
#define INITIALISZER_ASSIGNMENT YES
// Size method
#define SIZE YES
// Clear method
#define CLEAR YES
// Empty method
#define EMPTY YES
// Push front method
#define PUSH_FRONT YES
// Push back method
#define PUSH_BACK YES
// Pop back method
#define POP_FRONT YES
// Pop back method
#define POP_BACK YES
// Reference front, as in dq.front() = 3;
#define REF_FRONT YES
// Const front, as in x = dq.front();
#define CONST_FRONT YES
// Reference back, as in dq.back() = 3;
#define REF_BACK YES
// Const back, as in x = dq.back();
#define CONST_BACK YES
// Assign `count` elements with `value` to the deque: dq.assign(3,value);
#define ASSIGN_COUNT_VALUES YES
// Const index access operator, as in x = dq[3];
#define CONST_INDEX_OP YES
// Reference index access operator, as in dq[3] = x;
#define REF_INDEX_OP YES
// Const index access operator with bounds check, as in x = dq.at(3);
#define CONST_AT_INDEX YES
// Reference index access operator with bounds check, as in dq.at(3) = x;
#define REF_AT_INDEX YES
// Change the number of elements stored in the container.
#define RESIZE YES
// Shrink storage memory so that the capacity is the same as the # of elements currently stored.
#define SHRINK YES
// Equality operator
#define EQUAL_OP YES
// Different operator
#define DIFFERENT_OP YES
// Insert a single values before pos
#define INSERT_SINGLE_VALUE YES
// Insert a range of elements before pos
#define INSERT_RANGE YES
// Insert a initializer list of elements before pos
#define INSERT_INITIALIZER YES
// Insert multiple copies of a given value.
#define INSERT_MULTIPLE_VALUES YES
// Erase a range of elements begining at pos
#define ERASE_RANGE YES
// Erase a single values at pos
#define ERASE_SINGLE_VALUE YES
// Assign to deque values from a range.
#define ASSIGN_RANGE YES
// Assign to deque from a initialize_list.
#define ASSIGN_INIT_LIST YES

/// Tests the basic operations with a deque of integers.
template <typename T, size_t S>
void run_regular_deque_tests(const std::array<T, S>& values, const std::array<T, S>& source) {
  TestManager tm{ "Testing regular operations on a deque" };

#if DEFAULT_CTRO
  {
    BEGIN_TEST(tm, "DefaultConstructor", "deque<T> dq;");

    which_lib::deque<T> dq;

    EXPECT_EQ(dq.size(), 0);
    EXPECT_TRUE(dq.empty());
  }
#endif

#if CTRO_SIZE_VALUE
  {
    BEGIN_TEST(tm, "ConstructorSizeValue", "dq(size, value)");

    which_lib::deque<T> dq(10, values[0]);

    EXPECT_EQ(dq.size(), 10);
    EXPECT_FALSE(dq.empty());
    EXPECT_TRUE(
      std::all_of(dq.begin(), dq.end(), [=](const T& value) { return value == values[0]; }));
  }
#endif

#if CTRO_SIZE
  {
    BEGIN_TEST(tm, "ConstructorSize", "dq(size)");

    which_lib::deque<T> dq(10);

    EXPECT_EQ(dq.size(), 10);
    EXPECT_FALSE(dq.empty());
    EXPECT_TRUE(std::all_of(dq.begin(), dq.end(), [=](const T& value) { return value == T(); }));
  }
#endif

#if CTRO_RANGE
  {
    BEGIN_TEST(tm, "RangeConstructor", "deque<int> dq{first, last}");
    // Range = the entire deque.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    which_lib::deque<T> dq2{ dq.begin(), dq.end() };

    EXPECT_EQ(dq2.size(), 5);
    EXPECT_FALSE(dq.empty());

    // Cheking out the if the stored values are the same as in the source deque.
    for (auto i{ 0u }; i < dq.size(); ++i)
      EXPECT_EQ(values[i], dq[i]);

    // Creating a range that is part of an existing deque.
    auto offset{ 1 };
    which_lib::deque<T> dq3{ std::next(dq.begin(), offset), std::next(dq.begin(), 3) };
    // which_lib::deque<T> dq3{dq.begin()+offset, dq.begin()+3};
    EXPECT_EQ(dq3.size(), 2);
    EXPECT_FALSE(dq3.empty());

    // Cheking out the if the stored values are the same as in the source deque.
    for (auto i{ 0u }; i < dq3.size(); ++i)
      EXPECT_EQ(dq[i + offset], dq3[i]);
  }
#endif

#if CTRO_COPY
  {
    BEGIN_TEST(tm, "CopyConstructor", "deque<int> vec_clone{ dq }");

    // Original values for later conference.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    // Creating a copy of the original source deque
    which_lib::deque<T> dq2{ dq };

    // Checking out the deque internal state.
    EXPECT_EQ(dq2.size(), 5);
    EXPECT_FALSE(dq2.empty());

    // Check whether the copy worked.
    for (auto i{ 0u }; i < dq2.size(); ++i)
      EXPECT_EQ(values[i], dq2[i]);

    // Change the source deque and check whether that also changes the second dq.
    // Of course, this should not happen.
    dq[0] = 10;
    dq[1] = 20;
    dq[2] = 30;
    dq[3] = 40;
    dq[4] = 50;
    for (auto i{ 0u }; i < dq.size(); ++i)
      EXPECT_EQ(values[i], dq2[i]);
  }
#endif

#if LIST_CTRO
  {
    BEGIN_TEST(tm, "ListContructor", "deque<T> dq{1, 2, 3}");

    // Calling the constructor based on an anonymous list.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    EXPECT_EQ(dq.size(), 5);
    EXPECT_FALSE(dq.empty());

    for (auto i{ 0u }; i < dq.size(); ++i)
      EXPECT_EQ(values[i], dq[i]);
  }
#endif

#if ASSIGN_OP
  {
    BEGIN_TEST(tm, "AssignOperator", "dq1 = dq2");
    // Source deque
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    // Destination deque
    which_lib::deque<T> dq2;

    dq2 = dq;  // Assigning here.

    // Internal state.
    EXPECT_EQ(dq2.size(), 5);
    EXPECT_FALSE(dq2.empty());

    // CHeck whether the assignment worked.
    for (auto i{ 0u }; i < dq2.size(); ++i)
      EXPECT_EQ(values[i], dq2[i]);
  }
#endif

#if INITIALISZER_ASSIGNMENT
  {
    BEGIN_TEST(tm, "ListInitializerAssign", "deque<int> dq = { 1, 2, 3 }");
    // Assignind initial values.
    which_lib::deque<T> dq = { values[0], values[1], values[2], values[3], values[4] };

    EXPECT_EQ(dq.size(), 5);
    EXPECT_FALSE(dq.empty());

    // Check whether the copy worked.
    for (auto i{ 0u }; i < dq.size(); ++i)
      EXPECT_EQ(values[i], dq[i]);
  }
#endif

#if SIZE
  {
    BEGIN_TEST(tm, "Size", "dq.size()");

    // Assignind initial values.
    which_lib::deque<T> dq = { values[0], values[1], values[2], values[3], values[4] };
    EXPECT_EQ(dq.size(), 5);
    dq.clear();
    EXPECT_EQ(dq.size(), 0);

    size_t final_len{ std::size(source) };  // The final length of array.
    which_lib::deque<T> dq2;
    EXPECT_EQ(dq2.size(), 0);
    for (auto i{ 0u }; i < final_len; ++i) {
      dq2.push_back(source[i]);
      EXPECT_EQ(dq2.size(), i + 1);
    }

    which_lib::deque<T> dq3(dq2);
    EXPECT_EQ(dq3.size(), final_len);

    dq3.pop_back();
    dq3.pop_back();
    EXPECT_EQ(dq3.size(), final_len - 2);
    while (not dq3.empty())
      dq3.pop_back();
    EXPECT_EQ(dq3.size(), 0);
  }
#endif

#if CLEAR
  {
    BEGIN_TEST(tm, "Clear", "dq.clear()");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    EXPECT_EQ(dq.size(), 5);
    EXPECT_FALSE(dq.empty());

    dq.clear();

    EXPECT_EQ(dq.size(), 0);
    EXPECT_TRUE(dq.empty());
  }
#endif

#if EMPTY
  {
    BEGIN_TEST(tm, "Empty", "dq.empty()");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    EXPECT_FALSE(dq.empty());

    dq.clear();
    EXPECT_TRUE(dq.empty());
  }
#endif

#if PUSH_FRONT
  {
    BEGIN_TEST(tm, "PushFront", "dq.push_front(value)");
    // Starting off with an empty deque.
    which_lib::deque<T> dq;

    EXPECT_TRUE(dq.empty());
    for (auto i{ 0 }; i < std::size(values); ++i)
      dq.push_front(values[i]);
    EXPECT_FALSE(dq.empty());
    EXPECT_EQ(dq.size(), std::size(values));

    // Checking if the vales are right.
    auto length = std::size(values);
    for (auto i{ 0 }; i < length; ++i)
      EXPECT_EQ(values[i], dq[(length - 1) - i]);

    // Remove all elements.
    dq.clear();
    EXPECT_TRUE(dq.empty());

    // Insert again.
    for (auto i{ 0 }; i < std::size(values); ++i)
      dq.push_front(values[i]);
    EXPECT_FALSE(dq.empty());
    EXPECT_EQ(dq.size(), std::size(values));

    // Checking if the vales are right.
    for (auto i{ 0 }; i < std::size(values); ++i)
      EXPECT_EQ(values[i], dq[(length - 1) - i]);
  }
#endif

#if PUSH_BACK
  {
    BEGIN_TEST(tm, "PushBack", "dq.push_back(value)");
    // Starting off with an empty deque.
    which_lib::deque<T> dq;

    EXPECT_TRUE(dq.empty());
    for (auto i{ 0 }; i < std::size(values); ++i)
      dq.push_back(values[i]);
    EXPECT_FALSE(dq.empty());
    EXPECT_EQ(dq.size(), std::size(values));

    // Checking if the vales are right.
    for (auto i{ 0 }; i < std::size(values); ++i)
      EXPECT_EQ(values[i], dq[i]);

    // Remove all elements.
    dq.clear();
    EXPECT_TRUE(dq.empty());

    // Insert again.
    for (auto i{ 0 }; i < std::size(values); ++i)
      dq.push_back(values[i]);
    EXPECT_FALSE(dq.empty());
    EXPECT_EQ(dq.size(), std::size(values));

    // Checking if the vales are right.
    for (auto i{ 0 }; i < std::size(values); ++i)
      EXPECT_EQ(values[i], dq[i]);
  }
#endif

#if POP_FRONT
  {
    BEGIN_TEST(tm, "PopFront", "dq.pop_front()");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    auto start{ 0U };
    while (not dq.empty()) {
      // Check whether we have the same list except for the last.
      for (auto i{ 0U }; i < dq.size(); ++i) {
        // std::cout << "value[" << i << "] = " << values[i] << " | dq[" << i << "]" << dq[i] <<
        // '\n';
        EXPECT_EQ(values[i + start], dq[i]);
      }
      dq.pop_front();
      start++;
      std::cout << "\n";
    }
  }
#endif

#if POP_BACK
  {
    BEGIN_TEST(tm, "PopBack", "dq.pop_back()");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    while (not dq.empty()) {
      dq.pop_back();
      // Check whether we have the same list except for the last.
      for (auto i{ 0u }; i < dq.size(); ++i)
        EXPECT_EQ(values[i], dq[i]);
    }
  }
#endif

#if REF_FRONT
  {
    BEGIN_TEST(tm, "Front", "reference front() version: dq.front() = x");

    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    T target_value{ source[0] };
    auto i{ 0 };
    while (not dq.empty()) {
      auto current_size = dq.size();
      EXPECT_EQ(dq.front(), values[i++]);
      dq.front() = target_value;  // Changing the first elemento with front().
      EXPECT_EQ(current_size, dq.size());
      EXPECT_EQ(dq[0], target_value);

      // Remove the first element of the deque.
      dq.erase(dq.begin());
    }
  }
#endif

#if CONST_FRONT
  {
    BEGIN_TEST(tm, "FrontConst", "const front() version: x = dq.front()");

    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    EXPECT_EQ(dq.front(), values[0]);

    auto i{ 0 };
    while (not dq.empty()) {
      auto current_size = dq.size();
      EXPECT_EQ(dq.front(), values[i++]);
      EXPECT_EQ(current_size, dq.size());
      // Remove the first element of the deque.
      dq.erase(dq.begin());
    }
  }
#endif

#if REF_BACK
  {
    BEGIN_TEST(tm, "Back", "reference back() version: dq.back() = x");

    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    T target_value{ source[0] };
    auto i{ std::size(values) };
    while (not dq.empty()) {
      auto current_size = dq.size();
      EXPECT_EQ(dq.back(), values[--i]);
      dq.back() = target_value;  // Changing the first elemento with front().
      EXPECT_EQ(current_size, dq.size());
      EXPECT_EQ(dq[i], target_value);
      // Remove the first element of the deque.
      dq.pop_back();
    }
  }
#endif

#if CONST_BACK
  {
    BEGIN_TEST(tm, "BackConst", "const back() version: x = dq.back()");

    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    auto i{ std::size(values) };
    while (not dq.empty()) {
      auto current_size = dq.size();
      EXPECT_EQ(dq.back(), values[--i]);
      EXPECT_EQ(current_size, dq.size());
      // Remove the first element of the deque.
      dq.pop_back();
    }
  }
#endif

#if ASSIGN_COUNT_VALUES
  {
    BEGIN_TEST(tm, "AssignCountValue", "Assign count value: dq.assign(3, value)");

    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    EXPECT_EQ(dq.size(), std::size(values));

    // Test assign with a count SMALLER than the original dq size.
    T target_value{ source[0] };
    dq.assign(size_t(3), target_value);  // 3 < 5
    EXPECT_EQ(dq.size(), 3);
    // Verify the elements.
    for (auto i{ 0u }; i < dq.size(); ++i)
      EXPECT_EQ(target_value, dq[i]);

    // Test assign with a count GREATER than the original dq size.
    target_value = source[1];
    dq.assign(size_t(10), target_value);  // 10 > 5
    EXPECT_EQ(dq.size(), 10);
    // Verify the elements.
    for (auto i{ 0u }; i < dq.size(); ++i)
      EXPECT_EQ(target_value, dq[i]);
  }
#endif

#if CONST_INDEX_OP
  {
    BEGIN_TEST(tm, "OperatorBracketsRHS", "Operator Brackets RHS: x = dq[i]");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    // Assign all values from dq to dq2.
    for (auto i{ 0u }; i < dq.size(); ++i) {
      auto x = dq[i];
      EXPECT_EQ(x, dq[i]);  // Are they the same?
    }
  }
#endif

#if REF_INDEX_OP
  {
    BEGIN_TEST(tm, "OperatorBracketsLHS", "Operator Brackets LHS: dq[i] = x");
    // Creating a deque with a few elements.
    const which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    which_lib::deque<T> dq2(std::size(values));  // Same capacity as above.

    // Assign all values from dq to dq2.
    for (auto i{ 0u }; i < dq.size(); ++i)
      dq2[i] = dq[i];  // Assigning here.

    // Checking if they are equal.
    for (auto i{ 0u }; i < dq.size(); ++i)
      EXPECT_EQ(dq[i], dq2[i]);  // Are they the same?
  }
#endif

#if CONST_AT_INDEX
  {
    BEGIN_TEST(tm, "AtRHS", "at() as RHS: x = dq.at(i);");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    // Just accessing the element.
    for (auto i{ 0u }; i < dq.size(); ++i)
      EXPECT_EQ(dq.at(i), values[i]);

    // Testing exception throwing.
    bool worked{ false };
    try {
      dq.at(40);
    } catch (std::out_of_range& e) {
      worked = true;
    }
    EXPECT_TRUE(worked);
  }
#endif

#if REF_AT_INDEX
  {
    BEGIN_TEST(tm, "AtLHS", "at() as a LHS: dq.at(i) = x;");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq(std::size(values));  // Creating an "empty" deque with some capacity.

    // Chaging internal values with the at()
    for (auto i{ 0u }; i < dq.size(); ++i)
      dq.at(i) = values[i];
    // Did the assignment via at() work?
    for (auto i{ 0u }; i < dq.size(); ++i)
      EXPECT_EQ(dq[i], values[i]);

    // Testing exception throwing.
    bool worked{ false };
    auto target_value{ source[0] };
    try {
      dq.at(dq.size()) = target_value;
    } catch (std::out_of_range& e) {
      worked = true;
    }
    EXPECT_TRUE(worked);
  }
#endif

#if RESIZE
  {
    BEGIN_TEST(tm, "Resize", "resize()");
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    // Prepare 1st condition: resize(size()) leaves deque unchanged.
    //--------------------------------------------------------------------------
    std::vector<T> expected1{ values[0], values[1], values[2], values[3], values[4] };
    auto old_size = dq.size();
    dq.resize(dq.size());
    EXPECT_EQ(dq.size(), old_size);

    // Prepare 2nd condition: resize(size()+3) increases deque.
    //--------------------------------------------------------------------------
    auto expected2 = expected1;
    auto extra_value{ 3 };
    for (auto i{ 0U }; i < extra_value; ++i) {
      expected2.push_back(T());
    }
    //--------------------------------------------------------------------------
    old_size = dq.size();
    dq.resize(dq.size() + extra_value);
    EXPECT_EQ(dq.size(), old_size + extra_value);
    for (auto i{ 0U }; i < dq.size(); ++i) {
      EXPECT_EQ(dq[i], expected2[i]);
    }

    // Prepare 3rd condition: resize(size()-5) reduces deque.
    //--------------------------------------------------------------------------
    auto expected3 = expected2;
    extra_value = 5;
    for (auto i{ 0U }; i < extra_value; ++i) {
      expected3.pop_back();
    }
    //--------------------------------------------------------------------------
    old_size = dq.size();
    dq.resize(dq.size() - 5);
    EXPECT_EQ(dq.size(), old_size - 5);
    for (auto i{ 0U }; i < dq.size(); ++i) {
      EXPECT_EQ(dq[i], expected3[i]);
    }
  }
#endif

#if SHRINK
  {
    BEGIN_TEST(tm, "ShrinkToFit", "shrink_to_fit()");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    // Remove 2 elements.
    dq.pop_back();
    dq.pop_back();
    dq.shrink_to_fit();
    // Did we keep the original values?
    for (auto i{ 0u }; i < dq.size(); ++i) {
      EXPECT_EQ(dq[i], values[i]);
    }
  }
#endif

#if EQUAL_OP
  {
    BEGIN_TEST(tm, "OperatorEqual", "dq1 == dq2");

    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    which_lib::deque<T> dq2{ values[0], values[1], values[2], values[3], values[4] };
    which_lib::deque<T> dq3{ values[4], values[3], values[2], values[1], values[0] };
    which_lib::deque<T> dq4{ values[4], values[3], values[2] };

    EXPECT_EQ(dq, dq2);
    EXPECT_FALSE(dq == dq3);
    EXPECT_FALSE(dq == dq4);
  }
#endif

#if DIFFERENT_OP
  {
    BEGIN_TEST(tm, "OperatorDifferent", "dq1 != dq2");

    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    which_lib::deque<T> dq2{ values[0], values[1], values[2], values[3], values[4] };
    which_lib::deque<T> dq3{ values[4], values[3], values[2], values[1], values[0] };
    which_lib::deque<T> dq4{ values[4], values[3], values[2] };

    EXPECT_FALSE(dq != dq2);
    EXPECT_TRUE(dq != dq3);
    EXPECT_TRUE(dq != dq4);
  }
#endif

#if INSERT_SINGLE_VALUE
  {
    BEGIN_TEST(tm, "InsertSingleValueAtPosition", "dq.insert(pos, value)");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };

    // Insert at front
    dq.insert(dq.cbegin(), values[0]);
    EXPECT_EQ(
      dq,
      (which_lib::deque<T>{ values[0], values[0], values[1], values[2], values[3], values[4] }));
    // Insert in the middle
    dq.insert(dq.cbegin() + 3, values[4]);
    EXPECT_EQ(dq,
              (which_lib::deque<T>{
                values[0], values[0], values[1], values[4], values[2], values[3], values[4] }));
    // Insert at the end
    dq.insert(dq.cend(), values[2]);
    EXPECT_EQ(
      dq,
      (which_lib::deque<T>{
        values[0], values[0], values[1], values[4], values[2], values[3], values[4], values[2] }));
  }
#endif

#if INSERT_RANGE
  {
    BEGIN_TEST(
      tm, "InsertRange", "dq.insert( pos, first, last)");  // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    auto backup{ dq };
    which_lib::deque<T> src{ source[0], source[1], source[2], source[3], source[4] };
    which_lib::deque<T> expect1{ source[0], source[1], source[2], source[3], source[4],
                                 values[0], values[1], values[2], values[3], values[4] };
    which_lib::deque<T> expect2{ values[0], values[1], source[0], source[1], source[2],
                                 source[3], source[4], values[2], values[3], values[4] };
    which_lib::deque<T> expect3{ values[0], values[1], values[2], values[3], values[4],
                                 source[0], source[1], source[2], source[3], source[4] };

    // Insert at the begining.
    dq.insert(dq.cbegin(), src.begin(), src.end());
    EXPECT_EQ(dq, expect1);

    // In the middle
    dq = backup;
    dq.insert(dq.cbegin() + 2, src.begin(), src.end());
    EXPECT_EQ(dq, expect2);

    // At the end
    dq = backup;
    dq.insert(dq.cend(), src.begin(), src.end());
    EXPECT_EQ(dq, expect3);
  }
#endif

#if INSERT_INITIALIZER
  {
    BEGIN_TEST(tm, "InsertInitializarList", "dq.insert(pos, {1, 2, 3, 4 })");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    auto backup{ dq };
    which_lib::deque<T> src{ source[0], source[1], source[2], source[3], source[4] };
    which_lib::deque<T> expect1{ source[0], source[1], source[2], source[3], source[4],
                                 values[0], values[1], values[2], values[3], values[4] };
    which_lib::deque<T> expect2{ values[0], values[1], source[0], source[1], source[2],
                                 source[3], source[4], values[2], values[3], values[4] };
    which_lib::deque<T> expect3{ values[0], values[1], values[2], values[3], values[4],
                                 source[0], source[1], source[2], source[3], source[4] };

    // Insert at the begining.
    dq.insert(dq.cbegin(), { source[0], source[1], source[2], source[3], source[4] });
    EXPECT_EQ(dq, expect1);

    // In the middle
    dq = backup;
    dq.insert(dq.cbegin() + 2, { source[0], source[1], source[2], source[3], source[4] });
    EXPECT_EQ(dq, expect2);

    // At the end
    dq = backup;
    dq.insert(dq.cend(), { source[0], source[1], source[2], source[3], source[4] });
    EXPECT_EQ(dq, expect3);
  }
#endif

#if INSERT_MULTIPLE_VALUES
  {
    BEGIN_TEST(tm, "InsertCountValue", "dq.insert(pos, count, value)");
    // Creating a deque with a few elements.
    for (auto i{ 1U }; i < source.size(); ++i) {
      which_lib::deque<T> dq(i, values[2]);
      EXPECT_EQ(dq.size(), i);
      // Did we keep the original values?
      for (auto i{ 0U }; i < dq.size(); ++i) {
        EXPECT_EQ(dq[i], values[2]);
      }
    }
  }
#endif

#if ERASE_RANGE
  {
    BEGIN_TEST(tm, "EraseRange", "dq.erase(first, last)");
    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    auto backup{ dq };
    which_lib::deque<T> expect1{ values[3], values[4] };
    which_lib::deque<T> expect2{ values[0], values[4] };
    which_lib::deque<T> expect3{ values[0], values[1] };

    // removing a segment from the beginning.
    auto past_last = dq.erase(dq.begin(), dq.begin() + 3);
    EXPECT_EQ(dq.begin(), past_last);
    EXPECT_EQ(dq, expect1);
    // std::cout << *past_last <<" "<< *dq.begin()<< endl;
    EXPECT_EQ(dq.size(), 2);

    // removing at the middle.
    dq = backup;
    past_last = dq.erase(dq.begin() + 1, dq.begin() + 4);
    EXPECT_EQ(dq.begin() + 1, past_last);
    EXPECT_EQ(dq, expect2);
    EXPECT_EQ(dq.size(), 2);

    // removing a segment that reached the end.
    dq = backup;
    past_last = dq.erase(dq.begin() + 2, dq.end());
    EXPECT_EQ(dq.end(), past_last);
    EXPECT_EQ(dq, expect3);
    EXPECT_EQ(dq.size(), 2);

    // removing the entire deque.
    dq = backup;
    past_last = dq.erase(dq.begin(), dq.end());
    EXPECT_EQ(dq.end(), past_last);
    EXPECT_TRUE(dq.empty());
  }
#endif

#if ERASE_SINGLE_VALUE
  {
    BEGIN_TEST(tm, "ErasePos", "dq.erase(pos)");

    // Creating a deque with a few elements.
    which_lib::deque<T> dq{ values[0], values[1], values[2], values[3], values[4] };
    auto backup{ dq };
    which_lib::deque<T> expect1{ values[1], values[2], values[3], values[4] };
    which_lib::deque<T> expect2{ values[0], values[1], values[3], values[4] };
    which_lib::deque<T> expect3{ values[0], values[1], values[2], values[3] };

    // removing a single element.
    auto past_last = dq.erase(dq.begin());
    EXPECT_EQ(dq, expect1);
    EXPECT_EQ(dq.begin(), past_last);
    EXPECT_EQ(dq.size(), 4);

    // removing a single element in the middle.
    dq = backup;
    past_last = dq.erase(dq.begin() + 2);
    EXPECT_EQ(dq, expect2);
    EXPECT_EQ(dq.begin() + 2, past_last);
    EXPECT_EQ(dq.size(), 4);

    // removing a single element at the end.
    dq = backup;
    past_last = dq.erase(dq.begin() + dq.size() - 1);
    EXPECT_EQ(dq, expect3);
    EXPECT_EQ(dq.end(), past_last);
    EXPECT_EQ(dq.size(), 4);
  }
#endif

#if ASSIGN_RANGE
  {
    BEGIN_TEST(tm, "AssignFromRange", "Assign values from [first,last)");

    // CASE #1: Deque is smaller than range.
    which_lib::deque<T> dq{ values[0] };
    // Check size.
    EXPECT_EQ(dq.size(), 1);
    // Verify the elements.
    EXPECT_EQ(dq[0], values[0]);
    // Assigning a larger range to a small deque.
    dq.assign(values.begin(), values.end());
    // Check size.
    EXPECT_EQ(dq.size(), values.size());
    // Verify the elements.
    for (auto i{ 0U }; i < dq.size(); ++i) {
      EXPECT_EQ(dq[i], values[i]);
    }

    // CASE #2: Deque is larger than range.
    auto limite{3};
    // Assigning a larger range to a small deque.
    dq.assign(values.begin(), values.begin()+limite);
    // Check size.
    EXPECT_EQ(dq.size(), limite);
    // Verify the elements.
    for (auto i{ 0U }; i < limite; ++i) {
      EXPECT_EQ(dq[i], values[i]);
    }
  }
#endif

#if ASSIGN_INIT_LIST
  {
    BEGIN_TEST(tm, "AssignFromInitList", "Assign values from initialize list");

    // CASE #1: Deque is smaller than range.
    which_lib::deque<T> dq{ values[0] };
    // Check size.
    EXPECT_EQ(dq.size(), 1);
    // Verify the elements.
    EXPECT_EQ(dq[0], values[0]);
    // Assigning a larger range to a small deque.
    dq.assign({values[0], values[1], values[2], values[3], values[4]});
    // Check size.
    EXPECT_EQ(dq.size(), 5);
    // Verify the elements.
    for (auto i{ 0U }; i < 5; ++i) {
      EXPECT_EQ(dq[i], values[i]);
    }

    // CASE #2: Deque is larger than range.
    // Assigning a larger range to a small deque.
    dq.assign({values[0], values[1], values[2]});
    // Check size.
    EXPECT_EQ(dq.size(), 3);
    // Verify the elements.
    for (auto i{ 0U }; i < 3; ++i) {
      EXPECT_EQ(dq[i], values[i]);
    }
  }
#endif

    tm.summary();
    std::cout << "\n\n";
  }

#endif
