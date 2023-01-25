cmake_minimum_required(VERSION 3.22)
project(mactest LANGUAGES C VERSION 0.1)

include(GNUInstallDirs)

option(USE_COMPONENTS "use components")

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/mactest.c" [[
char *hello(void) {
    return "Hello, World!";
}
]])
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/mactest.h" [[
extern char *hello(void);
]])
add_library(mactest SHARED
    "${CMAKE_CURRENT_BINARY_DIR}/mactest.c"
    "${CMAKE_CURRENT_BINARY_DIR}/mactest.h"
)
target_include_directories(mactest
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>"
)
set_target_properties(mactest PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/mactest.h"

    FRAMEWORK 1
    FRAMEWORK_VERSION "A"
)

if(APPLE)
    set(cmake_dir "mactest.framework/Resources/CMake")
else()
    set(cmake_dir "${CMAKE_INSTALL_LIBDIR}/cmake/mactest")
endif()

if(USE_COMPONENTS)
    install(TARGETS mactest EXPORT mactest_EXPORTS
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT devel
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT devel
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT runtime
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT runtime
        RESOURCE DESTINATION "mactest.framework/Resources" COMPONENT runtime
        FRAMEWORK DESTINATION "." COMPONENT runtime
    )
    install(EXPORT mactest_EXPORTS
        DESTINATION "${cmake_dir}"
        NAMESPACE "mactest::"
        FILE "mactest-targets.cmake"
        COMPONENT devel
    )
else()
    install(TARGETS mactest EXPORT mactest_EXPORTS
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        RESOURCE DESTINATION "mactest.framework/Resources"
        FRAMEWORK DESTINATION "."
    )
    install(EXPORT mactest_EXPORTS
        DESTINATION "${cmake_dir}"
        NAMESPACE "mactest::"
        FILE "mactest-targets.cmake"
    )
endif()

if(APPLE)
    set(CPACK_GENERATOR DragNDrop)
else()
    set(CPACK_GENERATOR ZIP)
endif()

include(CPack)
