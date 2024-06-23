set(base_SRCS
 Timestamp.cpp
 Date.cpp
 TimeZone.cpp
 CurrentThread.cpp
 Thread.cpp
 Condition.cpp
 CountDownLatch.cpp
 Exception.cpp
 ThreadPool.cpp
 LogStream.cpp
 Logging.cpp
 FileUtil.cpp
 ProcessInfo.cpp
 LogFile.cpp
 AsyncLogging.cpp
 )

add_library(io_base ${base_SRCS})
target_link_libraries(io_base pthread rt)

install(TARGETS io_base DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/base)

# add_subdirectory(tests)