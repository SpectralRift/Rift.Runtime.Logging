cmake_minimum_required(VERSION 3.14)
project(Rift_Runtime_Logging VERSION 0.1.1)

add_library(
        Rift_Runtime_Logging
        STATIC
        private/Engine/Runtime/Logger.cpp
)

target_include_directories(
        Rift_Runtime_Logging
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private"
)

rift_resolve_module_libs("Rift.Core.Runtime" RIFT_LOGGER_DEPS)

target_link_libraries(Rift_Runtime_Logging ${RIFT_LOGGER_DEPS})