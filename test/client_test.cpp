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
        static const std::string urls[] = {
            "http://google.ru",
            "http://httpbin.org",
            "https://www.google.ru",
            "http://youtube.com",
            "http://yandex.ru",
            "http://ya.ru",
            "http://boost.org",
            "http://www.vk.com"
        };

        std::vector<asyncresponse_t> asyncresponses;
        service_t service;

        for (const auto& url : urls) {
            asyncresponses.push_back(AsyncGet(service, url));
        }

        for (const auto& asyncresponse : asyncresponses) {
            print(asyncresponse.get().error());
        }
    
        const auto& session =
            service.new_session("http://www.vk.com", timeout_t{3}, gzip_t{true});

        {
            const auto r = session.Get();
            print(r.error());
        }

        {
            const auto r = session.Get();
            print(r.error());
        }
    
        //service.run();
        return 0;
    }
    else {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
}
