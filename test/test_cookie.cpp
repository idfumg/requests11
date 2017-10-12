#include "cookies.h"
#include "utils.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace crequests;

TEST(Cookie, Default) {
    const auto cookie = "some=cookie; "_cookie;

    EXPECT_EQ(cookie.to_string(), "some=cookie");
}

TEST(Cookie, WithSecure) {
    const auto cookie = "some=cookie; Secure; "_cookie;

    EXPECT_EQ(cookie.to_string(), "some=cookie; Secure");
    EXPECT_TRUE(cookie.is_secure());
}

TEST(Cookie, WithSecureAndHttpOnly) {
    const auto cookie = "some=cookie; Secure; HttpOnly;"_cookie;

    EXPECT_EQ(cookie.to_string(), "some=cookie; Secure; HttpOnly");
    EXPECT_TRUE(cookie.is_secure());
    EXPECT_TRUE(cookie.is_http_only());
}

TEST(Cookie, WithDifferentSymbolCase) {
    const auto cookie = "SoMe=cooKie; SecurE; HTTPOnly;"_cookie;

    EXPECT_EQ(cookie.to_string(), "SoMe=cooKie; Secure; HttpOnly");
    EXPECT_TRUE(cookie.is_secure());
    EXPECT_TRUE(cookie.is_http_only());
}

TEST(Cookie, WithDash) {
    const auto cookie = "SoMe-otheR=cooKie;"_cookie;

    EXPECT_EQ(cookie.to_string(), "SoMe-otheR=cooKie");
}

TEST(Cookie, ParseDateTimeWithTz) {
    const auto time =
        string_to_time("Wed, 09 Jun 2021 10:18:14 GMT", "%a, %d %b %Y %H:%M:%S %Z");

    EXPECT_EQ(time_to_string(time, "%a, %d %b %Y %H:%M:%S %Z"),
              "Wed, 09 Jun 2021 10:18:14 GMT");
}

TEST(Cookie, WithSecureAndHttpOnlyAndExpires) {
    const auto cookie =
        "some=cookie; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly"_cookie;

    EXPECT_EQ(cookie.to_string(),
              "some=cookie; "
              "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly");
    EXPECT_TRUE(cookie.is_secure());
    EXPECT_TRUE(cookie.is_http_only());
    EXPECT_TRUE(!!cookie.expires());
    EXPECT_EQ(time_to_string(cookie.expires(), "%a, %d %b %Y %H:%M:%S %Z"), "Wed, 09 Jun 2021 10:18:14 GMT");
}

TEST(Cookies, Create) {
    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first=cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly;"};

    const auto cookie2 = cookie_t{
        "google.com", "/",
        "second=cookie2; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure;"};

    const auto cookie3 = cookie_t{
        "google.com", "/",
        "third=cookie3; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; HttpOnly;"};

    const auto cookie4 = cookie_t{
        "google.com", "/",
        "fourth=cookie4; "};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);
    cookies.add(cookie3);
    cookies.add(cookie4);

    EXPECT_EQ(cookies.to_string(),
              "first=cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly\n"
              "fourth=cookie4\n"
              "second=cookie2; Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure\n"
              "third=cookie3; Expires=Wed, 09 Jun 2021 10:18:14 GMT; HttpOnly\n\n");
}

TEST(Cookies, AddExistsCookie) {
    cookies_t cookies;

    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first=cookie1; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly;"};
    cookies.add(cookie1);

    const auto cookie2 = cookie_t{
        "google.com", "/",
        "first=cookie2; Secure;"};
    cookies.add(cookie2);

    EXPECT_EQ(cookies.to_string(), "first=cookie2; Secure\n\n");

    const auto cookie3 = cookie_t{
        "google.com", "/",
        "first=cookie3; Secure; HttpOnly"};
    cookies.add(cookie3);

    EXPECT_EQ(cookies.to_string(), "first=cookie3; Secure; HttpOnly\n\n");
}

TEST(Cookies, AddExistsCookieInDifferentDomain) {
    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first=cookie1; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly"};

    const auto cookie2 = cookie_t{
        "youtube.com", "/",
        "first=cookie2; Secure"};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);

    EXPECT_EQ(cookies.to_string(),
              "first=cookie2; Secure\n\n"
              "first=cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly\n\n");
}

TEST(Cookies, AddExistsCookieInSubDomain) {
    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first=cookie1; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly"};

    const auto cookie2 = cookie_t{
        "www.google.com", "/",
        "first=cookie2; Secure"};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);

    EXPECT_EQ(cookies.to_string(),
              "first=cookie2; Secure\n\n"
              "first=cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly\n\n");
}

TEST(CookiesDomain, EqualToOrigin) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=google.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=google.com\n\n");
}

TEST(CookiesDomain, ForSubDomain) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=www.google.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=www.google.com\n\n");
}

TEST(CookiesDomain, ForSubSubDomain) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=www.api.google.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=www.api.google.com\n\n");
}

TEST(CookiesDomain, ForSubDomainOfSubDomain) {
    const auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=www.api.google.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=www.api.google.com\n\n");
}

TEST(CookiesDomain, ForDifferentDomain) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=youtube.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, ForDifferentSubDomain) {
    const auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=api.www.google.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, ForParentDomain) {
    const auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=google.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=google.com\n\n");
}

TEST(CookiesDomain, ForPublicDomain) {
    const auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, ForSiblingsDomains) {
    const auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=www.google.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, ForSiblingsInDifferentDomains) {
    const auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=www.youtube.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, PublicDomainForPublicDomain) {
    const auto cookie = cookie_t{
        "yandex", "/",
        "first=cookie1; Domain=yandex"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=yandex\n\n");
}

TEST(CookiesDomain, PublicDomainForDifferentPublicDomain) {
    const auto cookie = cookie_t{
        "yandex", "/",
        "first=cookie1; Domain=yahoo"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, RemoveLeadingDotInCoverDomain) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=.api.google.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=api.google.com\n\n");
}

TEST(CookiesDomain, AddExistsCookieWithDifferentDomain) {
    cookies_t cookies;

    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=.api.google.com"};
    cookies.add(cookie1);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=api.google.com\n\n");

    const auto cookie2 = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=.www.google.com"};
    cookies.add(cookie2);

    EXPECT_EQ(cookies.to_string(),
              "first=cookie1; Domain=www.google.com\n\n"
              "first=cookie1; Domain=api.google.com\n\n");
}

TEST(CookiesDomain, ComplexPublicSuffixDomain) {
    const auto cookie = cookie_t{
        "compute.amazonaws.com", "/",
        "first=cookie1;"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1\n\n");
}

TEST(CookiesDomain, SubDomainToComplexPublicSuffixDomain) {
    const auto cookie = cookie_t{
        "api.compute.amazonaws.com", "/",
        "first=cookie1; Domain=compute.amazonaws.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, PublicSuffixHasAParentThatIsNotAPublicSuffix) {
    const auto cookie = cookie_t{
        "api.compute.amazonaws.com", "/",
        "first=cookie1; Domain=amazonaws.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, PublicSuffixToItself) {
    const auto cookie = cookie_t{
        "compute.amazonaws.com", "/",
        "first=cookie1; Domain=compute.amazonaws.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(),
              "first=cookie1; Domain=compute.amazonaws.com\n\n");
}

TEST(CookiesDomain, DomainOfPublicSuffixDomainToSubDomain) {
    const auto cookie = cookie_t{
        "www.api.compute.amazonaws.com", "/",
        "first=cookie1; Domain=www.api.compute.amazonaws.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(),
              "first=cookie1; Domain=www.api.compute.amazonaws.com\n\n");
}

TEST(CookiesCollect, One) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("google.com").to_string(),
              "first=cookie1; ");
}

TEST(CookiesCollect, OneWithDifferentReqCases) {
    const auto cookie = cookie_t{
        "gOOglE.com", "/",
        "first=cookie1"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("Google.com").to_string(),
              "first=cookie1; ");
}

TEST(CookiesCollect, OneWithDifferentReqAndCookiesCases) {
    const auto cookie = cookie_t{
        "aPi.gOOglE.com", "/",
        "first=cookie1; Domain=goOGle.com"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("apI.Google.com").to_string(),
              "first=cookie1; ");
}

TEST(CookiesCollect, Two) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1"};
    const auto cookie2 = cookie_t{
        "google.com", "/",
        "second=cookie2"};

    cookies_t cookies;
    cookies.add(cookie);
    cookies.add(cookie2);

    EXPECT_EQ(cookies.get("google.com").to_string(),
              "first=cookie1; second=cookie2; ");
}

TEST(CookiesCollect, SecondFromParent) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1"};
    const auto cookie2 = cookie_t{
        "www.google.com", "/",
        "second=cookie2"};

    cookies_t cookies;
    cookies.add(cookie);
    cookies.add(cookie2);

    EXPECT_EQ(cookies.get("www.google.com").to_string(),
              "first=cookie1; second=cookie2; ");
}

TEST(CookiesCollect, ThirdFromPublicDomain) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1"};
    const auto cookie2 = cookie_t{
        "www.google.com", "/",
        "second=cookie2"};
    const auto cookie3 = cookie_t{
        "com", "/",
        "third=cookie3"};

    cookies_t cookies;
    cookies.add(cookie);
    cookies.add(cookie2);
    cookies.add(cookie3);

    EXPECT_EQ(cookies.get("www.google.com").to_string(),
              "first=cookie1; second=cookie2; third=cookie3; ");
}

TEST(CookiesCollect, StopOnPublicDomain) {
    const auto cookie = cookie_t{
        "www.compute.amazonaws.com", "/",
        "first=cookie1"};
    const auto cookie2 = cookie_t{
        "api.www.compute.amazonaws.com", "/",
        "second=cookie2"};
    const auto cookie3 = cookie_t{
        "compute.amazonaws.com", "/",
        "third=cookie3"};
    const auto cookie4 = cookie_t{
        "amazonaws.com", "/",
        "fourth=cookie4"};

    cookies_t cookies;
    cookies.add(cookie);
    cookies.add(cookie2);
    cookies.add(cookie3);
    cookies.add(cookie4);

    EXPECT_EQ(cookies.get("api.www.compute.amazonaws.com").to_string(),
              "first=cookie1; second=cookie2; third=cookie3; ");
}

TEST(CookiesCollect, DoNotCollectChildren) {
    const auto cookie = cookie_t{
        "www.compute.amazonaws.com", "/",
        "first=cookie1"};
    const auto cookie2 = cookie_t{
        "api.www.compute.amazonaws.com", "/",
        "second=cookie2"};
    const auto cookie3 = cookie_t{
        "compute.amazonaws.com", "/",
        "third=cookie3"};
    const auto cookie4 = cookie_t{
        "amazonaws.com", "/",
        "fourth=cookie4"};

    cookies_t cookies;
    cookies.add(cookie);
    cookies.add(cookie2);
    cookies.add(cookie3);
    cookies.add(cookie4);

    EXPECT_EQ(cookies.get("www.compute.amazonaws.com").to_string(),
              "first=cookie1; third=cookie3; ");
    EXPECT_EQ(cookies.get("compute.amazonaws.com").to_string(),
              "third=cookie3; ");
    EXPECT_EQ(cookies.get("amazonaws.com").to_string(),
              "fourth=cookie4; ");
}

TEST(CookiesPath, Default) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Path=/"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("google.com").to_string(), "first=cookie1; ");
}

TEST(CookiesPath, SubPathWithoutSlash) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie; Path=/foo"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("google.com", "/foo").to_string(), "first=cookie; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/").to_string(), "first=cookie; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar").to_string(), "first=cookie; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar/").to_string(), "first=cookie; ");
    EXPECT_EQ(cookies.get("google.com", "/fooBar").to_string(), "");
}

TEST(CookiesPath, SubPathWithSlash) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie; Path=/foo/"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("google.com", "/foo").to_string(), "");
    EXPECT_EQ(cookies.get("google.com", "/foo/").to_string(), "first=cookie; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar").to_string(), "first=cookie; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar/").to_string(), "first=cookie; ");
}

TEST(CookiesPath, SubPathWithSubPathWithoutSlash) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie; Path=/foo/bar"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("google.com", "/foo").to_string(), "");
    EXPECT_EQ(cookies.get("google.com", "/foo/").to_string(), "");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar").to_string(), "first=cookie; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar/").to_string(), "first=cookie; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bara").to_string(), "");
}

TEST(CookiesPath, SubPathWithSubPathWithSlash) {
    const auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie; Path=/foo/bar/"};

    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("google.com", "/foo").to_string(), "");
    EXPECT_EQ(cookies.get("google.com", "/foo/").to_string(), "");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar").to_string(), "");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar/").to_string(), "first=cookie; ");
}

TEST(CookiesPath, SubPathWithSubPathAndThreeCookiesWithDifferentDepth) {
    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first; Path=/"};
    const auto cookie2 = cookie_t{
        "google.com", "/",
        "second; Path=/foo"};
    const auto cookie3 = cookie_t{
        "google.com", "/",
        "third; Path=/foo/bar"};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);
    cookies.add(cookie3);

    EXPECT_EQ(cookies.get("google.com", "/").to_string(), "first; ");
    EXPECT_EQ(cookies.get("google.com", "/foo").to_string(), "first; second; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/").to_string(), "first; second; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar").to_string(), "first; second; third; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar/").to_string(), "first; second; third; ");
}

TEST(CookiesPath, SubPathWithSubPathAndThreeCookiesWithDifferentDepthAndDomains) {
    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first; Path=/"};
    const auto cookie2 = cookie_t{
        "www.google.com", "/",
        "second; Path=/foo"};
    const auto cookie3 = cookie_t{
        "api.www.google.com", "/",
        "third; Path=/foo/bar"};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);
    cookies.add(cookie3);

    EXPECT_EQ(cookies.get("google.com", "/").to_string(), "first; ");
    EXPECT_EQ(cookies.get("google.com", "/foo").to_string(), "first; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/").to_string(), "first; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar").to_string(), "first; ");
    EXPECT_EQ(cookies.get("google.com", "/foo/bar/").to_string(), "first; ");

    EXPECT_EQ(cookies.get("www.google.com", "/").to_string(),
              "first; ");
    EXPECT_EQ(cookies.get("www.google.com", "/foo").to_string(),
              "first; second; ");
    EXPECT_EQ(cookies.get("www.google.com", "/foo/").to_string(),
              "first; second; ");
    EXPECT_EQ(cookies.get("www.google.com", "/foo/bar").to_string(),
              "first; second; ");
    EXPECT_EQ(cookies.get("www.google.com", "/foo/bar/").to_string(),
              "first; second; ");

    EXPECT_EQ(cookies.get("api.www.google.com", "/").to_string(),
              "first; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo").to_string(),
              "first; second; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo/").to_string(),
              "first; second; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo/bar").to_string(),
              "first; second; third; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo/bar/").to_string(),
              "first; second; third; ");

    const auto cookie4 = cookie_t{
        "www.google.com", "/",
        "fourth;"};
    cookies.add(cookie4);

    EXPECT_EQ(cookies.get("api.www.google.com", "/").to_string(),
              "first; fourth; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo").to_string(),
              "first; fourth; second; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo/").to_string(),
              "first; fourth; second; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo/bar").to_string(),
              "first; fourth; second; third; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo/bar/").to_string(),
              "first; fourth; second; third; ");

    const auto cookie5 = cookie_t{
        "api.www.google.com", "/",
        "fifth;"};
    cookies.add(cookie5);

    EXPECT_EQ(cookies.get("api.www.google.com", "/").to_string(),
              "fifth; first; fourth; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo").to_string(),
              "fifth; first; fourth; second; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo/").to_string(),
              "fifth; first; fourth; second; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo/bar").to_string(),
              "fifth; first; fourth; second; third; ");
    EXPECT_EQ(cookies.get("api.www.google.com", "/foo/bar/").to_string(),
              "fifth; first; fourth; second; third; ");
}

TEST(CookiesPath, WithEqualPathes) {
    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first; Path=/foo"};
    const auto cookie2 = cookie_t{
        "google.com", "/",
        "second; Path=/foo"};
    const auto cookie3 = cookie_t{
        "google.com", "/",
        "first; Path=/foo"};
    const auto cookie4 = cookie_t{
        "google.com", "/",
        "a=b; Path=/foo"};
    const auto cookie5 = cookie_t{
        "google.com", "/",
        "a=b; Path=/foo"};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);
    cookies.add(cookie3);
    cookies.add(cookie4);
    cookies.add(cookie5);

    EXPECT_EQ(cookies.get("google.com", "/foo").to_string(),
              "a=b; first; second; ");
    EXPECT_EQ(cookies.get("google.com", "/foo").size(), 3);
}

TEST(CookiesPath, WithDifferentPathes) {
    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first; Path=/foo"};
    const auto cookie3 = cookie_t{
        "google.com", "/",
        "first; Path=/bar"};

    const auto cookie2 = cookie_t{
        "google.com", "/",
        "second; Path=/foo"};

    const auto cookie4 = cookie_t{
        "google.com", "/",
        "a=b; Path=/bar"};
    const auto cookie5 = cookie_t{
        "google.com", "/",
        "a=b; Path=/"};
    const auto cookie6 = cookie_t{
        "google.com", "/",
        "a=b; Path=/bar"};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);
    cookies.add(cookie3);
    cookies.add(cookie4);
    cookies.add(cookie5);
    cookies.add(cookie6);

    EXPECT_EQ(cookies.get("google.com", "/").to_string(),
              "a=b; ");
    EXPECT_EQ(cookies.get("google.com", "/foo").to_string(),
              "a=b; first; second; ");
    EXPECT_EQ(cookies.get("google.com", "/bar").to_string(),
              "a=b; first; ");
}

TEST(CookiesPath, WithSecure) {
    const auto cookie1 = cookie_t{
        "google.com", "/",
        "first; secure;"};
    const auto cookie2 = cookie_t{
        "google.com", "/",
        "second; "};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);

    EXPECT_EQ(cookies.get("google.com", "/").to_string(),
              "second; ");
    EXPECT_EQ(cookies.get("google.com", "/", true).to_string(),
              "first; second; ");
}

TEST(Cookies, IpAddress) {
    const auto cookie1 = cookie_t{"127.0.0.1", "/", "first; "};
    const auto cookie2 = cookie_t{"127.0.0.1", "/", "second; "};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);

    EXPECT_EQ(cookies.get("127.0.0.1").to_string(), "first; second; ");
}

TEST(Cookies, OneExpired) {
    const auto cookie1 = cookie_t{
        "127.0.0.1", "/", "first; "
    };
    const auto cookie2 = cookie_t{
        "127.0.0.1", "/", "second; Expires=Wed, 09 Jun 1970 10:18:14 GMT; "
    };
    const auto cookie3 = cookie_t{
        "127.0.0.1", "/", "third; Expires=Wed, 09 Jun 2216 10:18:14 GMT; "
    };

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);
    cookies.add(cookie3);

    EXPECT_EQ(cookies.get("127.0.0.1").to_string(), "first; third; ");
}
