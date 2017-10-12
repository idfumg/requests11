#include "auth.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace crequests;

TEST(Auth, ToString) {
    const auto auth = auth_t{login_t{"hellow"}, password_t{"world"}};

    EXPECT_EQ(auth.to_string(), "hellow:world");
}

TEST(Auth, FromString) {
    const auto auth = auth_t::from_string("hellow:world");

    EXPECT_EQ(auth, (auth_t{login_t{"hellow"}, password_t{"world"}}));
}

TEST(Auth, FromStringUsingUserDefinedLiteral) {
    const auto auth = "hellow:world"_auth;

    EXPECT_EQ(auth, (auth_t{login_t{"hellow"}, password_t{"world"}}));
}
