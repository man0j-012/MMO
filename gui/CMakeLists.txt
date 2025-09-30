find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

add_executable(mmo_gui_monitor
    src/main.cpp
    src/MonitorWindow.hpp src/MonitorWindow.cpp
)

target_link_libraries(mmo_gui_monitor PRIVATE Qt5::Widgets Qt5::Network)
