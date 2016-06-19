# crequests
c++ HTTP library inspired by python requests.

Library dependecies:
- OpenSSL
- Boost: system, iostreams, asio
- C++11

This library is created for making comfortable way to do HTTP requests.
You can do simple ordinary requests like this:
```c++
#include <crequests/api.h>
#include <iostream>

int main() {
    using namespace crequests;
    service_t service;
    auto response = Get(service, "http://yandex.ru");
    std::cout << reponse->content() << std::endl;
    
    return 0;
}
```
Which is printed content of downloaded web page.

Also, you have a full control on result of request operation:
```c++
#include <crequests/api.h>

int main() {
    using namespace crequests;
    service_t service;
    auto response = Get(service, "http://yandex.ru");
    if (response.error()) {
        if (response.error().code() == error_code_t::TIMEOUT) {
            // handle timeout
        }
        else {
            // handle other error
        }
    }
    return 0;
}
```
If you want to save intermediate data between requests like cookies, you can use session mechanism:
```c++
#include <crequests/api.h>
#include <iostream>

int main() {
    using namespace crequests;
    service_t service;
    auto session = service.new_sesion("http://yandex.ru", timeout_t{10});
    {
        auto response = session->Get();
        std::cout << response->content() << std::endl;
    }
    {
        // send new request with saved cookies and settings.
        set_option(session, timeout_t{2}); // you can change parameters arbitrarily.
        auto response = session->Get();
        std::cout << response->content() << std::endl;
    }
    return 0;
}
```
Also, its available to use asynchronous nature of Boost.Asio to send multiple requests.
Ordinary HTTP os HTTPS protocol will be choosen from a given uri. Also you can implicitly
set it by sending parameters to api functions.
```c++
#include <crequests/api.h>

int main() {
    using namespace crequests;
    static const std::vector<std::string> urls = {
        "http://google.ru",
        "http://httpbin.org",
        "https://www.google.ru",
        "http://youtube.com",
        "http://yandex.ru",
        "http://ya.ru",
        "http://boost.org",
        "http://www.vk.com"
    };

    std::vector< asyncresponse_ptr_t > asyncresponses;
    service_t service;

    for (auto&& url : urls)
        asyncresponses.push_back(AsyncGet(service, url));

    for (auto&& asyncresponse : asyncresponses)
        std::cout << asyncresponses->get()->error() << std::endl;
    
    return 0;
}
```
If you do not want to explicit wait response from server you can set final callback to do the work.
This feature is needed if client is running in separate thread or process and you want to grab results later.
All received responses will be saved for a dispose_timeout interval. After timeout they will be removed.
So you can get a response when you want and adjust settings as you want.
```c++
#include <crequests/api.h>

int main() {
    using namespace crequests;
    service_t service{dispose_timeout_t{10}};
    auto callback = [](const response_t& response) {
        // do the work
    };
    AsyncGet(service, "http://boost.org", final_callback_t{callback});
    service.run(); // loop the program for waiting a response.
    
    return 0;
}
```
KeepAlive and redirects is on by default.
You can gzip you POST data on demand by using gzip_t{true} on api functions.

response->raw() function return raw data received from the server.
response->content() function return ungzipped data (if needed or raw data) automatically.

In memory working with ssl certificates:
```c++
#include <crequests/api.h>

int main() {
    using namespace crequests;
    service_t service;
    auto response = Get(service, "https://some_url_with_basic_auth", "user:passwd"_auth);

    auto ssl_cert_key = ssl_auth_t {certificate_t{"asd"}, privatekey_t{"qwe"}};
    auto response = Get(service, "https://some_url", ssl_cert_key);

    auto ssl_certs = ssl_certs_t{certificate_t{"first"}, certificate_t{"second"}};
    auto response = Get(service, "https://some_url", ssl_certs);
    
    return 0;
}
```
Also there are features for working with ceritificates and private keys files.
You can adjust request way you want using type_t parameters or user defined literals.
```c++
#include <crequests/api.h>

int main() {
    auto response = Get(service, "https://some_url", "a=1&b=2"_params, "443"_port);
    return 0;
}
```
All this parameters can be found in request.h.

Thanks to:
- https://github.com/kennethreitz/requests
- https://github.com/whoshuu/cpr
- https://github.com/cpp-netlib/cpp-netlib
