set(TESTS_SOURCES
    server.cpp
    test_api.cpp
    test_auth.cpp
    test_connection.cpp
    test_cookie.cpp
    test_headers.cpp
    test_params.cpp
    test_parser.cpp
    test_redirects.cpp
    test_request.cpp
    test_uri.cpp
    client_test.cpp
)

find_package(GTest)
if (NOT ${GTEST_FOUND})
   message(FATAL_ERROR "Package Threads not found.")
endif()
   
add_executable(tests ${TESTS_SOURCES})

target_link_libraries(
    tests PUBLIC

    crequests
    ${GTEST_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
