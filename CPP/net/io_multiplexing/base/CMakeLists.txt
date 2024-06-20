set(base_SRCS
 Timestamp.cpp
 )

add_library(io_base ${base_SRCS})
target_link_libraries(io_base pthread rt)

install(TARGETS io_base DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/base)

add_subdirectory(tests)