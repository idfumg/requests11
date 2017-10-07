#include "redirects.h"
#include "request.h"
#include "response.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace crequests;

TEST(Redirects, PrepareDefaultAndFindRequest) {
    redirects_t redirects;

    request_t request;
    request.url("google.com"_url);
    request.prepare();

    redirects.add(response_t{request});
    EXPECT_EQ(redirects.get().size(), 1);

    redirects.add(response_t{request});
    EXPECT_EQ(redirects.get().size(), 1);

    request.url("youtube.com"_url);
    request.prepare();
    redirects.add(response_t{request});
    EXPECT_EQ(redirects.get().size(), 2);

    EXPECT_NE(redirects.find(request), boost::none);

    request.url("google.com"_url);
    request.prepare();

    EXPECT_NE(redirects.find(request), boost::none);

    request.url("goooogle.com"_url);
    request.prepare();

    EXPECT_EQ(redirects.find(request), boost::none);
}
