cmake_minimum_required(VERSION 3.0)
project(CustomerService)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
    endif ()
endif ()


message(STATUS "cmake is building directory ${PROJECT_SOURCE_DIR}")

message(STATUS "search proto files")

file(GLOB PROTO_FILES "*/*.proto")

message(STATUS "protos found: ")
foreach(PROTO_FILE ${PROTO_FILES})
    file(RELATIVE_PATH PROTO_FILE_RELPATH ${PROJECT_SOURCE_DIR} ${PROTO_FILE})
    list(APPEND PROTO_FILES_RELPATH ${PROTO_FILE_RELPATH})
    message(STATUS "    ${PROTO_FILE_RELPATH}")
endforeach()

message(STATUS "generate protobuf cpp classes ")
execute_process(
        COMMAND protoc
            -I protos
            --cpp_out=.
            ${PROTO_FILES_RELPATH}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

message(STATUS "generate protobuf grpc interface")
execute_process(
        COMMAND protoc
            -I protos
            --grpc_out=.
            --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin
            ${PROTO_FILES_RELPATH}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

message(STATUS "search for source files")

file(GLOB SRC_FILES "*.cc" "*.cpp" "*.h" "*.hpp")

message(STATUS "src files found:")
foreach(SRC_FILE ${SRC_FILES})
    file(RELATIVE_PATH SRC_FILE_RELPATH ${PROJECT_SOURCE_DIR} ${SRC_FILE})
    list(APPEND SOURCE_FILES ${SRC_FILE_RELPATH})
    message(STATUS "    ${SRC_FILE_RELPATH}")
endforeach()

message(STATUS ${SRC_FILE_RELPATH})

include_directories(/usr/local/include
                    /usr/local/include/cppconn)
link_directories(/usr/local/lib)

add_executable(CustomerService ${SOURCE_FILES})
target_link_libraries(CustomerService
        grpc++
        grpc
        grpc++_reflection
        protobuf
        pthread
        mysqlcppconn)