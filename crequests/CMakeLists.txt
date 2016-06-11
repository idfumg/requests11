set(CREQUESTS_SOURCES
    auth.cpp
    connection.cpp
    cookies.cpp
    error.cpp   
    headers.cpp
    params.cpp
    parser.cpp
    redirects.cpp
    request.cpp
    response.cpp
    service.cpp
    session.cpp
    types.cpp
    uri.cpp
    utils.cpp
    ssl_auth.cpp
    ssl_certs.cpp
    asyncresponse.cpp
    
    ../external/http_parser/http_parser.c
)

set(CREQUESTS_HEADERS
    api.h
    auth.h
    boost_asio.h
    boost_asio_fwd.h
    connection.h
    cookies.h
    error.h   
    headers.h
    macros.h
    params.h
    parser.h
    redirects.h
    request.h
    response.h
    service.h
    session.h
    types.h
    uri.h
    utils.h
    ssl_auth.h
    ssl_certs.h
    asyncresponse.h
)

find_package(Boost COMPONENTS system iostreams)
if (NOT ${Boost_FOUND})
   message(FATAL_ERROR "Package Boost not found.")
   endif()

find_package(OpenSSL)
if (NOT ${OpenSSL_FOUND})
   message(FATAL_ERROR "Package OpenSSL not found.")
endif()
   
find_package(Threads)
if (NOT ${THREADS_FOUND})
   message(FATAL_ERROR "Package Threads not found.")
endif()

add_library(crequests ${CREQUESTS_SOURCES})

target_link_libraries(
    crequests

    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
)

target_include_directories(crequests PUBLIC
                           crequests
						   ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS crequests DESTINATION lib)
install(FILES ${CREQUESTS_HEADERS} DESTINATION include/crequests)
