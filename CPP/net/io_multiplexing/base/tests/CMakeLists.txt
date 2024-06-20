add_executable(timestamp_test Timestamp_test.cpp)
target_link_libraries(timestamp_test io_base)

add_executable(date_unittest Date_unittest.cpp)
target_link_libraries(date_unittest io_base)
add_test(NAME date_unittest COMMAND date_unittest)

add_executable(timezone_util TimeZone_util.cpp)
target_link_libraries(timezone_util io_base)