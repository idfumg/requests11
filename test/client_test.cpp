#include "api.h"
#include "gtest/gtest.h"

#define REAL_TEST 0

template <class T>
void print(T&& value) {
    std::cout << value << std::endl;
}

int main(int argc, char** argv) {
    using namespace crequests;

    if (REAL_TEST) {
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

        std::vector<future_t<response_ptr_t> > futures;
        service_t service;

        for (auto&& url : urls)
            futures.push_back(AsyncGet(service, url));

        for (auto&& future : futures)
            print(future.get()->error());
    
        auto session = service.new_session("http://www.vk.com", timeout_t{3});

        {
            auto r = session->Get();
            print(r.get()->error());
        }

        {
            auto r = session->Get();
            print(r.get()->error());
        }
    
        //service.run();
        return 0;
    }
    else {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
}
