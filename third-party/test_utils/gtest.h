//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#define EXPECT_THROW_VALUE( statement, type, true_expr )    \
try                                                         \
{                                                           \
   statement;                                               \
   FAIL() << "Expected exception not thrown";               \
}                                                           \
catch ( type e )                                            \
{                                                           \
   EXPECT_TRUE( true_expr );                                \
}                                                           \
catch ( ... )                                               \
{                                                           \
   FAIL() << "Unexpected exception thrown";                 \
}

//-----------------------------------------------------------------------------

#define NAMESPACE_GTEST_TEST_CLASS_NAME_(namespace_name, test_case_name, test_name) \
  namespace_name##_##test_case_name##_##test_name##_Test

// A copy of GTEST_TEST_, but with handling for namespace name.

#define NAMESPACE_GTEST_TEST_(namespace_name, test_case_name, test_name, parent_class, parent_id)\
class NAMESPACE_GTEST_TEST_CLASS_NAME_(namespace_name, test_case_name, test_name) : public parent_class {\
 public:\
  NAMESPACE_GTEST_TEST_CLASS_NAME_(namespace_name, test_case_name, test_name)() {}\
 private:\
  virtual void TestBody();\
  static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;\
  GTEST_DISALLOW_COPY_AND_ASSIGN_(\
      NAMESPACE_GTEST_TEST_CLASS_NAME_(namespace_name, test_case_name, test_name));\
};\
\
::testing::TestInfo* const NAMESPACE_GTEST_TEST_CLASS_NAME_(namespace_name, test_case_name, test_name)\
  ::test_info_ =\
    ::testing::internal::MakeAndRegisterTestInfo(\
        #namespace_name "." #test_case_name, #test_name, NULL, NULL, /* <-- Defines the test as "Namespace.Classname" */ \
        ::testing::internal::CodeLocation(__FILE__, __LINE__), \
        (parent_id), \
        parent_class::SetUpTestCase, \
        parent_class::TearDownTestCase, \
        new ::testing::internal::TestFactoryImpl<\
            NAMESPACE_GTEST_TEST_CLASS_NAME_(namespace_name, test_case_name, test_name)>);\
void NAMESPACE_GTEST_TEST_CLASS_NAME_(namespace_name, test_case_name, test_name)::TestBody()

// Simple macro

#define NAMESPACE_TEST(namespace_name, test_case_name, test_name) \
  NAMESPACE_GTEST_TEST_(namespace_name, test_case_name, test_name,\
    ::testing::Test, ::testing::internal::GetTestTypeId())

//-----------------------------------------------------------------------------

#define INSTANTIATE_NAMESPACE_TEST_SUITE_P(namespace_name, prefix, test_suite_name, ...)      \
  static ::testing::internal::ParamGenerator<test_suite_name::ParamType>      \
      gtest_##prefix##test_suite_name##_EvalGenerator_() {                    \
    return GTEST_EXPAND_(GTEST_GET_FIRST_(__VA_ARGS__, DUMMY_PARAM_));        \
  }                                                                           \
  static ::std::string gtest_##prefix##test_suite_name##_EvalGenerateName_(   \
      const ::testing::TestParamInfo<test_suite_name::ParamType>& info) {     \
    if (::testing::internal::AlwaysFalse()) {                                 \
      ::testing::internal::TestNotEmpty(GTEST_EXPAND_(GTEST_GET_SECOND_(      \
          __VA_ARGS__,                                                        \
          ::testing::internal::DefaultParamName<test_suite_name::ParamType>,  \
          DUMMY_PARAM_)));                                                    \
      auto t = std::make_tuple(__VA_ARGS__);                                  \
      static_assert(std::tuple_size<decltype(t)>::value <= 2,                 \
                    "Too Many Args!");                                        \
    }                                                                         \
    return ((GTEST_EXPAND_(GTEST_GET_SECOND_(                                 \
        __VA_ARGS__,                                                          \
        ::testing::internal::DefaultParamName<test_suite_name::ParamType>,    \
        DUMMY_PARAM_))))(info);                                               \
  }                                                                           \
  static int gtest_##prefix##test_suite_name##_dummy_                         \
      GTEST_ATTRIBUTE_UNUSED_ =                                               \
          ::testing::UnitTest::GetInstance()                                  \
              ->parameterized_test_registry()                                 \
              .GetTestSuitePatternHolder<test_suite_name>(                    \
                  GTEST_STRINGIFY_(test_suite_name),                          \
                  ::testing::internal::CodeLocation(__FILE__, __LINE__))      \
              ->AddTestSuiteInstantiation(                                    \
                  GTEST_STRINGIFY_(namespace_name##.##prefix),                         \
                  &gtest_##prefix##test_suite_name##_EvalGenerator_,          \
                  &gtest_##prefix##test_suite_name##_EvalGenerateName_,       \
                  __FILE__, __LINE__)

//-----------------------------------------------------------------------------
