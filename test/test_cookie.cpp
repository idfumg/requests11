#include "cookies.h"
#include "utils.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace requests;

TEST(Cookie, Default) {
    auto cookie = "some=cookie; "_cookie;
    
    EXPECT_EQ(cookie.to_string(), "some=cookie");
}

TEST(Cookie, WithSecure) {
    auto cookie = "some=cookie; Secure; "_cookie;
    
    EXPECT_EQ(cookie.to_string(), "some=cookie; Secure");
    EXPECT_TRUE(cookie.is_secure());
}

TEST(Cookie, WithSecureAndHttpOnly) {
    auto cookie = "some=cookie; Secure; HttpOnly;"_cookie;
    
    EXPECT_EQ(cookie.to_string(), "some=cookie; Secure; HttpOnly");
    EXPECT_TRUE(cookie.is_secure());
    EXPECT_TRUE(cookie.is_http_only());
}

TEST(Cookie, WithDifferentSymbolCase) {
    auto cookie = "SoMe=cooKie; SecurE; HTTPOnly;"_cookie;
    
    EXPECT_EQ(cookie.to_string(), "SoMe=cooKie; Secure; HttpOnly");
    EXPECT_TRUE(cookie.is_secure());
    EXPECT_TRUE(cookie.is_http_only());
}

TEST(Cookie, WithDash) {
    auto cookie = "SoMe-otheR=cooKie;"_cookie;
    
    EXPECT_EQ(cookie.to_string(), "SoMe-otheR=cooKie");
}

TEST(Cookie, ParseDateTimeWithTz) {
    auto time = string_to_time("Wed, 09 Jun 2021 10:18:14 GMT", "%a, %d %b %Y %H:%M:%S %Z");
    EXPECT_EQ(time_to_string(time, "%a, %d %b %Y %H:%M:%S %Z"), "Wed, 09 Jun 2021 10:18:14 GMT");
}

TEST(Cookie, WithSecureAndHttpOnlyAndExpires) {
    auto cookie =
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
    auto cookie1 = cookie_t{
        "google.com", "/",
        "first=cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly;"};

    auto cookie2 = cookie_t{
        "google.com", "/",
        "second=cookie2; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure;"};

    auto cookie3 = cookie_t{
        "google.com", "/",
        "third=cookie3; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; HttpOnly;"};

    auto cookie4 = cookie_t{
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
    
    auto cookie1 = cookie_t{
        "google.com", "/",
        "first=cookie1; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly;"};
    cookies.add(cookie1);
    
    auto cookie2 = cookie_t{
        "google.com", "/",
        "first=cookie2; Secure;"};
    cookies.add(cookie2);

    EXPECT_EQ(cookies.to_string(), "first=cookie2; Secure\n\n");

    auto cookie3 = cookie_t{
        "google.com", "/",
        "first=cookie3; Secure; HttpOnly"};
    cookies.add(cookie3);

    EXPECT_EQ(cookies.to_string(), "first=cookie3; Secure; HttpOnly\n\n");
}

TEST(Cookies, AddExistsCookieInDifferentDomain) {
    auto cookie1 = cookie_t{
        "google.com", "/",
        "first=cookie1; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly"};

    auto cookie2 = cookie_t{
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
    auto cookie1 = cookie_t{
        "google.com", "/", 
        "first=cookie1; "
        "Expires=Wed, 09 Jun 2021 10:18:14 GMT; Secure; HttpOnly"};

    auto cookie2 = cookie_t{
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
    auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=google.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=google.com\n\n");
}

TEST(CookiesDomain, ForSubDomain) {
    auto cookie = cookie_t{
        "google.com", "/", 
        "first=cookie1; Domain=www.google.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=www.google.com\n\n");
}

TEST(CookiesDomain, ForSubSubDomain) {
    auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=www.api.google.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=www.api.google.com\n\n");
}

TEST(CookiesDomain, ForSubDomainOfSubDomain) {
    auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=www.api.google.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=www.api.google.com\n\n");
}

TEST(CookiesDomain, ForDifferentDomain) {
    auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=youtube.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, ForDifferentSubDomain) {
    auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=api.www.google.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, ForParentDomain) {
    auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=google.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=google.com\n\n");
}

TEST(CookiesDomain, ForPublicDomain) {
    auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, ForSiblingsDomains) {
    auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=www.google.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, ForSiblingsInDifferentDomains) {
    auto cookie = cookie_t{
        "api.google.com", "/",
        "first=cookie1; Domain=www.youtube.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, PublicDomainForPublicDomain) {
    auto cookie = cookie_t{
        "yandex", "/",
        "first=cookie1; Domain=yandex"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=yandex\n\n");
}

TEST(CookiesDomain, PublicDomainForDifferentPublicDomain) {
    auto cookie = cookie_t{
        "yandex", "/",
        "first=cookie1; Domain=yahoo"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, RemoveLeadingDotInCoverDomain) {
    auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=.api.google.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=api.google.com\n\n");
}

TEST(CookiesDomain, AddExistsCookieWithDifferentDomain) {
    cookies_t cookies;
    
    auto cookie1 = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=.api.google.com"};
    cookies.add(cookie1);

    EXPECT_EQ(cookies.to_string(), "first=cookie1; Domain=api.google.com\n\n");

    auto cookie2 = cookie_t{
        "google.com", "/",
        "first=cookie1; Domain=.www.google.com"};
    cookies.add(cookie2);

    EXPECT_EQ(cookies.to_string(),
              "first=cookie1; Domain=www.google.com\n\n"
              "first=cookie1; Domain=api.google.com\n\n");
}

TEST(CookiesDomain, ComplexPublicSuffixDomain) {
    auto cookie = cookie_t{
        "compute.amazonaws.com", "/",
        "first=cookie1;"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "first=cookie1\n\n");
}

TEST(CookiesDomain, SubDomainToComplexPublicSuffixDomain) {
    auto cookie = cookie_t{
        "api.compute.amazonaws.com", "/",
        "first=cookie1; Domain=compute.amazonaws.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, PublicSuffixHasAParentThatIsNotAPublicSuffix) {
    auto cookie = cookie_t{
        "api.compute.amazonaws.com", "/",
        "first=cookie1; Domain=amazonaws.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(), "");
}

TEST(CookiesDomain, PublicSuffixToItself) {
    auto cookie = cookie_t{
        "compute.amazonaws.com", "/",
        "first=cookie1; Domain=compute.amazonaws.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(),
              "first=cookie1; Domain=compute.amazonaws.com\n\n");
}

TEST(CookiesDomain, DomainOfPublicSuffixDomainToSubDomain) {
    auto cookie = cookie_t{
        "www.api.compute.amazonaws.com", "/",
        "first=cookie1; Domain=www.api.compute.amazonaws.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.to_string(),
              "first=cookie1; Domain=www.api.compute.amazonaws.com\n\n");
}

TEST(CookiesCollect, One) {
    auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("google.com").to_string(),
              "first=cookie1; ");
}

TEST(CookiesCollect, OneWithDifferentReqCases) {
    auto cookie = cookie_t{
        "gOOglE.com", "/",
        "first=cookie1"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("Google.com").to_string(),
              "first=cookie1; ");
}

TEST(CookiesCollect, OneWithDifferentReqAndCookiesCases) {
    auto cookie = cookie_t{
        "aPi.gOOglE.com", "/",
        "first=cookie1; Domain=goOGle.com"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("apI.Google.com").to_string(),
              "first=cookie1; ");
}

TEST(CookiesCollect, Two) {
    auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1"};
    auto cookie2 = cookie_t{
        "google.com", "/",
        "second=cookie2"};
    
    cookies_t cookies;
    cookies.add(cookie);
    cookies.add(cookie2);

    EXPECT_EQ(cookies.get("google.com").to_string(),
              "first=cookie1; second=cookie2; ");
}

TEST(CookiesCollect, SecondFromParent) {
    auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1"};
    auto cookie2 = cookie_t{
        "www.google.com", "/",
        "second=cookie2"};
    
    cookies_t cookies;
    cookies.add(cookie);
    cookies.add(cookie2);

    EXPECT_EQ(cookies.get("www.google.com").to_string(),
              "first=cookie1; second=cookie2; ");
}

TEST(CookiesCollect, ThirdFromPublicDomain) {
    auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1"};
    auto cookie2 = cookie_t{
        "www.google.com", "/",
        "second=cookie2"};
    auto cookie3 = cookie_t{
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
    auto cookie = cookie_t{
        "www.compute.amazonaws.com", "/",
        "first=cookie1"};
    auto cookie2 = cookie_t{
        "api.www.compute.amazonaws.com", "/",
        "second=cookie2"};
    auto cookie3 = cookie_t{
        "compute.amazonaws.com", "/",
        "third=cookie3"};
    auto cookie4 = cookie_t{
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
    auto cookie = cookie_t{
        "www.compute.amazonaws.com", "/",
        "first=cookie1"};
    auto cookie2 = cookie_t{
        "api.www.compute.amazonaws.com", "/",
        "second=cookie2"};
    auto cookie3 = cookie_t{
        "compute.amazonaws.com", "/",
        "third=cookie3"};
    auto cookie4 = cookie_t{
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
    auto cookie = cookie_t{
        "google.com", "/",
        "first=cookie1; Path=/"};
    
    cookies_t cookies;
    cookies.add(cookie);

    EXPECT_EQ(cookies.get("google.com").to_string(), "first=cookie1; ");
}

TEST(CookiesPath, SubPathWithoutSlash) {
    auto cookie = cookie_t{
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
    auto cookie = cookie_t{
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
    auto cookie = cookie_t{
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
    auto cookie = cookie_t{
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
    auto cookie1 = cookie_t{
        "google.com", "/",
        "first; Path=/"};
    auto cookie2 = cookie_t{
        "google.com", "/",
        "second; Path=/foo"};
    auto cookie3 = cookie_t{
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
    auto cookie1 = cookie_t{
        "google.com", "/",
        "first; Path=/"};
    auto cookie2 = cookie_t{
        "www.google.com", "/",
        "second; Path=/foo"};
    auto cookie3 = cookie_t{
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

    auto cookie4 = cookie_t{
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

    auto cookie5 = cookie_t{
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
    auto cookie1 = cookie_t{
        "google.com", "/",
        "first; Path=/foo"};
    auto cookie2 = cookie_t{
        "google.com", "/",
        "second; Path=/foo"};
    auto cookie3 = cookie_t{
        "google.com", "/",
        "first; Path=/foo"};
    auto cookie4 = cookie_t{
        "google.com", "/", 
        "a=b; Path=/foo"};
    auto cookie5 = cookie_t{
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
    auto cookie1 = cookie_t{
        "google.com", "/", 
        "first; Path=/foo"};
    auto cookie3 = cookie_t{
        "google.com", "/", 
        "first; Path=/bar"};
    
    auto cookie2 = cookie_t{
        "google.com", "/", 
        "second; Path=/foo"};
    
    auto cookie4 = cookie_t{
        "google.com", "/", 
        "a=b; Path=/bar"};
    auto cookie5 = cookie_t{
        "google.com", "/", 
        "a=b; Path=/"};
    auto cookie6 = cookie_t{
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
    auto cookie1 = cookie_t{
        "google.com", "/",
        "first; secure;"};
    auto cookie2 = cookie_t{
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
    auto cookie1 = cookie_t{"127.0.0.1", "/", "first; "};
    auto cookie2 = cookie_t{"127.0.0.1", "/", "second; "};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);

    EXPECT_EQ(cookies.get("127.0.0.1").to_string(), "first; second; ");
}

TEST(Cookies, OneExpired) {
    auto cookie1 = cookie_t{"127.0.0.1", "/", "first; "};
    auto cookie2 = cookie_t{"127.0.0.1", "/", "second; Expires=Wed, 09 Jun 1970 10:18:14 GMT; "};
    auto cookie3 = cookie_t{"127.0.0.1", "/", "third; Expires=Wed, 09 Jun 2216 10:18:14 GMT; "};

    cookies_t cookies;
    cookies.add(cookie1);
    cookies.add(cookie2);
    cookies.add(cookie3);

    EXPECT_EQ(cookies.get("127.0.0.1").to_string(), "first; third; ");
}
