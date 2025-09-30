add_library(common
    common/message.hpp
    common/util.hpp
)

add_executable(mmo_server
    server/main.cpp
    server/server.hpp server/server.cpp
    server/session.hpp server/session.cpp
    server/multicast.hpp server/multicast.cpp
    server/world.hpp server/world.cpp
    server/metrics.hpp server/metrics.cpp
)
target_link_libraries(mmo_server PRIVATE common pthread)

add_executable(mmo_client
    client/main.cpp
)
target_link_libraries(mmo_client PRIVATE common pthread)

add_executable(mmo_loadgen
    tools/loadgen/main.cpp
)
target_link_libraries(mmo_loadgen PRIVATE common pthread)
