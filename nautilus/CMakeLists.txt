
add_library(nautilus)

if (ENABLE_TESTS)
    # set path to test dir
    set(TEST_DATA_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/test/data/")
endif ()
# create config file with all compile time options
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/nautilus/common/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/nautilus/config.hpp)

add_subdirectory(src)

get_source(nautilus NAUTILUS_SOURCE_FILES)

# Add Library
target_sources(nautilus PRIVATE ${NAUTILUS_SOURCE_FILES})
target_include_directories(nautilus PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/nautilus/>)

target_include_directories(nautilus PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include/>)

target_include_directories(nautilus PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:src/nautilus/>)

set_target_properties(nautilus PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        EXPORT_NAME nautilus
)

if (ENABLE_LOGGING)
    target_link_libraries(nautilus PRIVATE spdlog::spdlog)
endif ()

if (ENABLE_STACKTRACE)
    # Add Backward to your target (either Backward::Interface, Backward::Object, or Backward::Backward)
    target_link_libraries(nautilus PRIVATE Backward::Interface)
endif ()

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

# Handle CMake Install
include(GNUInstallDirs)
set_target_properties(nautilus PROPERTIES DEBUG_POSTFIX "d")
install(
        TARGETS nautilus ${EXPORT_TARGETS}
        EXPORT nautilus
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(
        EXPORT nautilus
        FILE nautilusConfigTargets.cmake
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/nautilus"
        NAMESPACE nautilus::
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
        ../cmake/nautilusConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/nautilusConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/nautilus"
)

install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/nautilusConfig.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/nautilus"
)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# Install config.hpp
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
