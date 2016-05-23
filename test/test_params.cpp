#include "params.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace crequests;

TEST(Params, FromString) {
    auto params = params_t::from_string("a=1&b=2&c=3&d=4");
    
    EXPECT_EQ(params, (params_t {{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}}));
}

TEST(Params, FromStringUsingUserDefinedLiteral) {
    auto params = "a=1&b=2&c=3&d=4"_params;
    
    EXPECT_EQ(params, (params_t {{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}}));
}

TEST(Params, ToString) {
    auto params = params_t {{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}};
    
    EXPECT_EQ(params.to_string(), "d=4&c=3&b=2&a=1");
}

TEST(Params, Update) {
    auto params1 = params_t {{"a", "1"}, {"b", "2"}};
    auto params2 = params_t {{"c", "3"}, {"d", "4"}};
    params1.update(params2);
    
    EXPECT_EQ(params1.to_string(), "c=3&d=4&b=2&a=1");
}
