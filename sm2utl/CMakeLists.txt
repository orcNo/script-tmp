cmake_minimum_required (VERSION 2.6)

set (BIN_NAME sm2utl)
project (${BIN_NAME})


set(SOURCES main.c
	)

add_executable(${BIN_NAME} ${SOURCES})

set(CMAKE_C_FLAGS "-g -Wall -I. -I$(SWSSL)/include -fPIC -Wno-unused-but-set-variable -Wno-unused-function")
set(SWSSL_INCLUDE_DIRS "/usr/local/swssl/include")
set(SWSSL_LIBRARIES "-lssl -lcrypto -lpthread -ldl")
set(SWSSL_LINK_DIRS "/usr/local/swssl/lib")

target_include_directories(${BIN_NAME} PUBLIC
	${SWSSL_INCLUDE_DIRS}
	)
target_link_libraries(${BIN_NAME} PRIVATE
	-lpthread
	${SWSSL_LIBRARIES}
	)
target_link_directories(${BIN_NAME} PRIVATE
	${SWSSL_LINK_DIRS}
	)
