# BuildEvent.cmake
# Builds libevent as a static library directly from source files,
# using pre-defined config headers (no libevent CMakeLists.txt / autoconf needed).
# Mirrors the approach in event/premake5.lua.

set(EVENT_DIR "${CMAKE_SOURCE_DIR}/event")

set(EVENT_SOURCES
    ${EVENT_DIR}/event.c
    ${EVENT_DIR}/evthread.c
    ${EVENT_DIR}/buffer.c
    ${EVENT_DIR}/bufferevent.c
    ${EVENT_DIR}/bufferevent_sock.c
    ${EVENT_DIR}/bufferevent_filter.c
    ${EVENT_DIR}/bufferevent_pair.c
    ${EVENT_DIR}/listener.c
    ${EVENT_DIR}/bufferevent_ratelim.c
    ${EVENT_DIR}/evmap.c
    ${EVENT_DIR}/log.c
    ${EVENT_DIR}/evutil.c
    ${EVENT_DIR}/evutil_rand.c
    ${EVENT_DIR}/strlcpy.c
    ${EVENT_DIR}/signal.c
    ${EVENT_DIR}/event_tagging.c
    ${EVENT_DIR}/http.c
    ${EVENT_DIR}/evdns.c
    ${EVENT_DIR}/evrpc.c
    ${EVENT_DIR}/evutil_time.c
)

if(WIN32)
    list(APPEND EVENT_SOURCES
        ${EVENT_DIR}/win32select.c
        ${EVENT_DIR}/evthread_win32.c
        ${EVENT_DIR}/buffer_iocp.c
        ${EVENT_DIR}/event_iocp.c
        ${EVENT_DIR}/bufferevent_async.c
    )
elseif(APPLE)
    list(APPEND EVENT_SOURCES
        ${EVENT_DIR}/evthread_pthread.c
        ${EVENT_DIR}/kqueue.c
        ${EVENT_DIR}/poll.c
        ${EVENT_DIR}/select.c
    )
else()  # Linux
    list(APPEND EVENT_SOURCES
        ${EVENT_DIR}/evthread_pthread.c
        ${EVENT_DIR}/epoll.c
        ${EVENT_DIR}/epoll_sub.c
        ${EVENT_DIR}/poll.c
        ${EVENT_DIR}/select.c
    )
endif()

add_library(event STATIC ${EVENT_SOURCES})

target_include_directories(event PUBLIC
    ${EVENT_DIR}/include
    ${EVENT_DIR}/compat
)

if(WIN32)
    target_link_libraries(event PUBLIC ws2_32 iphlpapi)
elseif(NOT APPLE)
    target_link_libraries(event PUBLIC pthread)
endif()
