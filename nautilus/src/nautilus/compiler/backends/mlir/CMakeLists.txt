
if (ENABLE_MLIR_BACKEND)
    find_package(MLIR REQUIRED CONFIG)
    message(STATUS MLIR_INCLUDE_DIRS=${MLIR_INCLUDE_DIRS})
    target_include_directories(nautilus SYSTEM PRIVATE
            $<BUILD_INTERFACE:${MLIR_INCLUDE_DIRS}>
            $<INSTALL_INTERFACE:include/nebulastream/>)
    add_compile_definitions(MLIR_COMPILER)
    get_property(mlir_libs GLOBAL PROPERTY MLIR_ALL_LIBS)
    get_property(dialect_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)
    get_property(conversion_libs GLOBAL PROPERTY MLIR_CONVERSION_LIBS)
    get_property(extension_libs GLOBAL PROPERTY MLIR_EXTENSION_LIBS)
    message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")
    message(STATUS "mlir_libs: ${mlir_libs}")
    message(STATUS "dialect_libs: ${dialect_libs}")
    message(STATUS "conversion_libs: ${conversion_libs}")
    message(STATUS "extension_libs: ${extension_libs}")


    add_source_files(nautilus
            MLIRLoweringProvider.cpp
            MLIRCompilationBackend.cpp
            MLIRExecutable.cpp
            MLIRPassManager.cpp
            LLVMIROptimizer.cpp
            JITCompiler.cpp
    )

    if (NOT USE_EXTERNAL_MLIR)
        add_library(nautilus_mlir INTERFACE)
        add_custom_target(${CMAKE_CURRENT_BINARY_DIR}/libnautilus_mlir.a)
        add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libnautilus_mlir.a
                PRE_BUILD nautilus_mlir
                COMMAND ${CMAKE_AR} -rc ${CMAKE_CURRENT_BINARY_DIR}/libnautilus_mlir.a
                $<TARGET_FILE:MLIRExecutionEngine>
                $<TARGET_FILE:MLIRFuncAllExtensions>
                $<TARGET_FILE:MLIRSCFDialect>
                $<TARGET_FILE:MLIRFuncToLLVM>
                $<TARGET_FILE:MLIRSCFToControlFlow>
        )
        target_link_libraries(nautilus_mlir INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/libnautilus_mlir.a)
#        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nautilus_mlir.a DESTINATION lib)
        target_link_libraries(nautilus
                PRIVATE
                nautilus_mlir
        )
        list(APPEND EXPORT_TARGETS "nautilus_mlir")
    else ()
        target_link_libraries(nautilus
                PRIVATE
                MLIRExecutionEngine
                MLIRFuncAllExtensions
                # Dialects
                MLIRSCFDialect
                MLIRFuncToLLVM
                MLIRSCFToControlFlow
        )
    endif ()
endif ()

