#include "auth.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace crequests;

TEST(Auth, ToString) {
    auto auth = auth_t{"hellow", "world"};
    
    EXPECT_EQ(auth.to_string(), "hellow:world");
}

TEST(Auth, FromString) {
    auto auth = auth_t::from_string("hellow:world");
    
    EXPECT_EQ(auth, (auth_t{"hellow", "world"}));
}

TEST(Auth, FromStringUsingUserDefinedLiteral) {
    auto auth = "hellow:world"_auth;
    
    EXPECT_EQ(auth, (auth_t{"hellow", "world"}));
}
