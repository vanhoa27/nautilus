#include "nautilus/compiler/backends/cpp/CPPCompiler.hpp"
#include "nautilus/common/File.hpp"
#include "nautilus/compiler/backends/cpp/CPPCompilerFlags.hpp"
#include "nautilus/compiler/backends/cpp/ExecutablePath.hpp"
#include "nautilus/compiler/backends/cpp/SharedLibrary.hpp"
#include <filesystem>
#include <sstream>
#include <string>

using namespace std::string_literals;
namespace nautilus::compiler::cpp {

std::shared_ptr<CPPCompiler> CPPCompiler::create() {
	return std::make_shared<CPPCompiler>();
}

CPPCompiler::CPPCompiler() : runtimePathConfig(ExecutablePath::loadRuntimePathConfig()) {
}

CPPCompiler::~CPPCompiler() noexcept = default;

SharedLibraryPtr CPPCompiler::compile(const std::string& identifier, const std::string& code) const {
	// Timer timer("CPPCompiler");
	// timer.start();
	std::string fileName = (std::filesystem::temp_directory_path() / identifier);
	auto sourceFileName = fileName + ".cpp";
	auto libraryFileName = fileName +
#ifdef __linux__
	                       ".so";
#elif defined(__APPLE__)
	                       ".dylib";
#else
#error "Unknown platform"
#endif

	auto file = common::File::createFile(sourceFileName, code);
	auto compilationFlags = CPPCompilerFlags::createDefaultCompilerFlags();
	// if (request->enableOptimizations()) {
	//  NES_DEBUG("Compile with optimizations.");
	//     compilationFlags.enableOptimizationFlags();
	// }
	// if (request->enableDebugging()) {
	//     NES_DEBUG("Compile with debugging.");
	//     compilationFlags.enableDebugFlags();
	//    format->formatFile(file);
	//     file->print();
	// }
	// if (request->enableCompilationProfiling()) {
	//     compilationFlags.addFlag(CPPCompilerFlags::TRACE_COMPILATION_TIME);
	//     NES_DEBUG("Compilation Time tracing is activated open:
	//     chrome://tracing/");
	// }
	compilationFlags.addFlag("-shared");
	compilationFlags.addFlag("-g");

	// add header
	for (auto libPaths : runtimePathConfig.libPaths) {
		compilationFlags.addFlag(std::string("-L") + libPaths);
	}
	// add libs
	for (auto libs : runtimePathConfig.libs) {
		compilationFlags.addFlag(libs);
	}
	// add header
	for (auto includePath : runtimePathConfig.includePaths) {
		compilationFlags.addFlag("-I" + includePath);
	}

	compilationFlags.addFlag("-o" + libraryFileName);

	compilationFlags.addFlag(sourceFileName);

	compileSharedLib(compilationFlags, file, libraryFileName);

// wrapper code that loads original library and provides exec2 function
std::string wrapperCode = R"(
#include <dlfcn.h>
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

// Embedded shared memory helper functions
namespace SharedMemoryHelper {
    void* create_shared_memory(const std::string& name, size_t size) {
        int fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
        if (fd == -1) {
            throw std::runtime_error("shm_open failed: " + std::string(strerror(errno)));
        }

        if (ftruncate(fd, size) == -1) {
            close(fd);
            throw std::runtime_error("ftruncate failed: " + std::string(strerror(errno)));
        }

        void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);

        if (ptr == MAP_FAILED) {
            throw std::runtime_error("mmap failed: " + std::string(strerror(errno)));
        }

        return ptr;
    }

    void* open_shared_memory(const std::string& name, size_t size) {
        int fd = shm_open(name.c_str(), O_RDWR, 0666);
        if (fd == -1) {
            throw std::runtime_error("shm_open failed: " + std::string(strerror(errno)));
        }

        void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);

        if (ptr == MAP_FAILED) {
            throw std::runtime_error("mmap failed: " + std::string(strerror(errno)));
        }

        return ptr;
    }

    void cleanup_shared_memory(const std::string& name, void* ptr, size_t size) {
        munmap(ptr, size);
        shm_unlink(name.c_str());
    }

    void write_result(void* shared_mem, int32_t result) {
        *static_cast<int32_t*>(shared_mem) = result;
    }

    int32_t read_result(void* shared_mem) {
        return *static_cast<int32_t*>(shared_mem);
    }
}

// Global variables to store library path and handle
static std::string g_library_path = ")" + libraryFileName + R"(";
static void* g_handle = nullptr;
static bool g_initialized = false;
typedef void (*execute_func_t)(uint8_t*, uint8_t*, uint8_t*);
static execute_func_t g_execute_func = nullptr;

// Initialize function to load the library and get the function pointer
void initialize() {
    if (g_initialized) return;

    g_handle = dlopen(g_library_path.c_str(), RTLD_NOW);
    if (!g_handle) {
        throw std::runtime_error(std::string("Cannot load library: ") + dlerror());
    }

    g_execute_func = (execute_func_t) dlsym(g_handle, "execute");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        dlclose(g_handle);
        throw std::runtime_error(std::string("Cannot load symbol 'execute': ") + dlsym_error);
    }

    g_initialized = true;
}

extern "C" {

void exec2(uint8_t* var1, uint8_t* var2, uint8_t* var3) {
    initialize();

    // setup shared memory
    std::string shm_name = "/query_result_" + std::to_string(getpid());
    void* shared_mem = SharedMemoryHelper::create_shared_memory(shm_name, sizeof(int32_t));

    // Hard-coded size value
    const int32_t size = 80;

	std::string var1FileName = "temp_var1.bin";
	std::string var2FileName = "temp_var2.bin";
	std::string var3FileName = "temp_var3.bin";

	std::ofstream var1File(var1FileName, std::ios::binary);
	var1File.write(reinterpret_cast<const char*>(var1), size);
	var1File.close();

	std::ofstream var2File(var2FileName, std::ios::binary);
	var2File.write(reinterpret_cast<const char*>(var2), size);
	var2File.close();

	std::ofstream var3File(var3FileName, std::ios::binary);
	var3File.write(reinterpret_cast<const char*>(var3), size);
	var3File.close();

    std::string testHarnessCode = R"TEST(
#include <iostream>
#include <cstdint>
#include <dlfcn.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

namespace SharedMemoryHelper {
    void* open_shared_memory(const std::string& name, size_t size) {
        int fd = shm_open(name.c_str(), O_RDWR, 0666);
        if (fd == -1) {
            throw std::runtime_error("shm_open failed: " + std::string(strerror(errno)));
        }

        void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);

        if (ptr == MAP_FAILED) {
            throw std::runtime_error("mmap failed: " + std::string(strerror(errno)));
        }

        return ptr;
    }

    void write_result(void* shared_mem, int32_t result) {
        *static_cast<int32_t*>(shared_mem) = result;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <shm_name>" << std::endl;
        return 1;
    }

    std::string shm_name = argv[1];
    const int32_t size = 20; // Hard-coded size

    std::ifstream var1File("temp_var1.bin", std::ios::binary);
    if (!var1File) {
        std::cerr << "Cannot open var1 file" << std::endl;
        return 1;
    }
    std::vector<uint8_t> var1Data(size);
    var1File.read(reinterpret_cast<char*>(var1Data.data()), size);
    var1File.close();

    std::ifstream var2File("temp_var2.bin", std::ios::binary);
    if (!var2File) {
        std::cerr << "Cannot open var2 file" << std::endl;
        return 1;
    }
    std::vector<uint8_t> var2Data(size);
    var2File.read(reinterpret_cast<char*>(var2Data.data()), size);
    var2File.close();

    std::ifstream var3File("temp_var3.bin", std::ios::binary);
    if (!var3File) {
        std::cerr << "Cannot open var3 file" << std::endl;
        return 1;
    }
    std::vector<uint8_t> var3Data(size);
    var3File.read(reinterpret_cast<char*>(var3Data.data()), size);
    var3File.close();

    void* handle = dlopen(")TEST" + g_library_path + R"TEST(", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot load library: " << dlerror() << std::endl;
        return 1;
    }

    typedef void (*execute_func_t)(uint8_t*, uint8_t*, uint8_t*);
    execute_func_t execute_func = (execute_func_t) dlsym(handle, "execute");

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol 'execute': " << dlsym_error << std::endl;
        dlclose(handle);
        return 1;
    }

    // Call the function - it returns void, so we assume success if it doesn't crash
    try {
        execute_func(var1Data.data(), var2Data.data(), var3Data.data());

        // Since function returns void, we assume success (exit code 0) if we reach here
        void* shared_mem = SharedMemoryHelper::open_shared_memory(shm_name, sizeof(int32_t));
        SharedMemoryHelper::write_result(shared_mem, 0); // 0 indicates success
        munmap(shared_mem, sizeof(int32_t));

    } catch (const std::exception& e) {
        std::cerr << "Function execution failed: " << e.what() << std::endl;
        dlclose(handle);
        return 1;
    }

    dlclose(handle);
    return 0;
}
)TEST";

    std::string testHarnessFileName = "test_wrapper.cpp";
    std::ofstream testFile(testHarnessFileName);
    testFile << testHarnessCode;
    testFile.close();

    std::string testExecName = "test_wrapper_exec";
    std::string compileCmd = "cc -g " + testHarnessFileName + " -ldl -lstdc++ -lrt -o " + testExecName + " 2>&1";

    if (std::system(compileCmd.c_str()) != 0) {
        std::remove(testHarnessFileName.c_str());
        std::remove(var1FileName.c_str());
        std::remove(var2FileName.c_str());
        std::remove(var3FileName.c_str());
        SharedMemoryHelper::cleanup_shared_memory(shm_name, shared_mem, sizeof(int32_t));
        throw std::runtime_error("Test harness compilation failed");
    }

    // execute query with rr - note: no size parameter needed anymore
    std::string rrCmd = "rr record " + testExecName + " " + shm_name;
    int rrResult = std::system(rrCmd.c_str());

	// TODO: handle this properly
    // std::remove(testHarnessFileName.c_str());
    std::remove(testExecName.c_str());
    std::remove(var1FileName.c_str());
    std::remove(var2FileName.c_str());
    std::remove(var3FileName.c_str());

    // Read actual result (0 = success, non-zero = failure)
    int32_t actual_result = SharedMemoryHelper::read_result(shared_mem);

    // cleanup
    SharedMemoryHelper::cleanup_shared_memory(shm_name, shared_mem, sizeof(int32_t));

    if (rrResult != 0) {
        throw std::runtime_error("rr record failed");
    }

    // Since the original function returned void, we return the success status
    // 0 means the function executed without crashing
}

}
)";


// 	// wrapper code that loads original library and provides exec2 function
// std::string wrapperCode = R"(
// #include <dlfcn.h>
// #include <iostream>
// #include <cstdint>
// #include <stdexcept>
// #include <cstdlib>
// #include <fstream>
// #include <string>
// #include <cstdio>
// #include <unistd.h>
// #include <sys/mman.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <cerrno>
// #include <cstring>
//
// // Embedded shared memory helper functions
// namespace SharedMemoryHelper {
//     void* create_shared_memory(const std::string& name, size_t size) {
//         int fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
//         if (fd == -1) {
//             throw std::runtime_error("shm_open failed: " + std::string(strerror(errno)));
//         }
//
//         if (ftruncate(fd, size) == -1) {
//             close(fd);
//             throw std::runtime_error("ftruncate failed: " + std::string(strerror(errno)));
//         }
//
//         void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//         close(fd);
//
//         if (ptr == MAP_FAILED) {
//             throw std::runtime_error("mmap failed: " + std::string(strerror(errno)));
//         }
//
//         return ptr;
//     }
//
//     void* open_shared_memory(const std::string& name, size_t size) {
//         int fd = shm_open(name.c_str(), O_RDWR, 0666);
//         if (fd == -1) {
//             throw std::runtime_error("shm_open failed: " + std::string(strerror(errno)));
//         }
//
//         void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//         close(fd);
//
//         if (ptr == MAP_FAILED) {
//             throw std::runtime_error("mmap failed: " + std::string(strerror(errno)));
//         }
//
//         return ptr;
//     }
//
//     void cleanup_shared_memory(const std::string& name, void* ptr, size_t size) {
//         munmap(ptr, size);
//         shm_unlink(name.c_str());
//     }
//
//     void write_result(void* shared_mem, int32_t result) {
//         *static_cast<int32_t*>(shared_mem) = result;
//     }
//
//     int32_t read_result(void* shared_mem) {
//         return *static_cast<int32_t*>(shared_mem);
//     }
// }
//
// // Global variables to store library path and handle
// static std::string g_library_path = ")" + libraryFileName + R"(";
// static void* g_handle = nullptr;
// static bool g_initialized = false;
// typedef int32_t (*execute_func_t)(int32_t, uint8_t*, uint8_t*);
// static execute_func_t g_execute_func = nullptr;
//
// // Initialize function to load the library and get the function pointer
// void initialize() {
//     if (g_initialized) return;
//
//     g_handle = dlopen(g_library_path.c_str(), RTLD_NOW);
//     if (!g_handle) {
//         throw std::runtime_error(std::string("Cannot load library: ") + dlerror());
//     }
//
//     g_execute_func = (execute_func_t) dlsym(g_handle, "execute");
//     const char* dlsym_error = dlerror();
//     if (dlsym_error) {
//         dlclose(g_handle);
//         throw std::runtime_error(std::string("Cannot load symbol 'execute': ") + dlsym_error);
//     }
//
//     g_initialized = true;
// }
//
// extern "C" {
//
// int32_t exec2(int32_t size, uint8_t* mask, uint8_t* data) {
//     initialize();
//
//     // setup shared memory
//     std::string shm_name = "/query_result_" + std::to_string(getpid());
//     void* shared_mem = SharedMemoryHelper::create_shared_memory(shm_name, sizeof(int32_t));
//
//     std::string maskFileName = "temp_mask.bin";
//     std::string dataFileName = "temp_data.bin";
//
//     std::ofstream maskFile(maskFileName, std::ios::binary);
//     maskFile.write(reinterpret_cast<const char*>(mask), size);
//     maskFile.close();
//
//     std::ofstream dataFileStream(dataFileName, std::ios::binary);
//     dataFileStream.write(reinterpret_cast<const char*>(data), size * sizeof(int32_t));
//     dataFileStream.close();
//
//     std::string testHarnessCode = R"TEST(
// #include <iostream>
// #include <cstdint>
// #include <dlfcn.h>
// #include <fstream>
// #include <vector>
// #include <unistd.h>
// #include <sys/mman.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <cerrno>
// #include <cstring>
//
// namespace SharedMemoryHelper {
//     void* open_shared_memory(const std::string& name, size_t size) {
//         int fd = shm_open(name.c_str(), O_RDWR, 0666);
//         if (fd == -1) {
//             throw std::runtime_error("shm_open failed: " + std::string(strerror(errno)));
//         }
//
//         void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//         close(fd);
//
//         if (ptr == MAP_FAILED) {
//             throw std::runtime_error("mmap failed: " + std::string(strerror(errno)));
//         }
//
//         return ptr;
//     }
//
//     void write_result(void* shared_mem, int32_t result) {
//         *static_cast<int32_t*>(shared_mem) = result;
//     }
// }
//
// int main(int argc, char* argv[]) {
//     if (argc != 3) {
//         std::cerr << "Usage: " << argv[0] << " <size> <shm_name>" << std::endl;
//         return 1;
//     }
//
//     int32_t size = std::stoi(argv[1]);
//     std::string shm_name = argv[2];
//
//     std::ifstream maskFile("temp_mask.bin", std::ios::binary);
//     if (!maskFile) {
//         std::cerr << "Cannot open mask file" << std::endl;
//         return 1;
//     }
//     std::vector<uint8_t> mask(size);
//     maskFile.read(reinterpret_cast<char*>(mask.data()), size);
//     maskFile.close();
//
//     std::ifstream dataFile("temp_data.bin", std::ios::binary);
//     if (!dataFile) {
//         std::cerr << "Cannot open data file" << std::endl;
//         return 1;
//     }
//     std::vector<uint8_t> data(size * sizeof(int32_t));
//     dataFile.read(reinterpret_cast<char*>(data.data()), size * sizeof(int32_t));
//     dataFile.close();
//
//     void* handle = dlopen(")TEST" + g_library_path + R"TEST(", RTLD_LAZY);
//     if (!handle) {
//         std::cerr << "Cannot load library: " << dlerror() << std::endl;
//         return 1;
//     }
//
//     typedef int32_t (*execute_func_t)(int32_t, uint8_t*, uint8_t*);
//     execute_func_t execute_func = (execute_func_t) dlsym(handle, "execute");
//
//     const char* dlsym_error = dlerror();
//     if (dlsym_error) {
//         std::cerr << "Cannot load symbol 'execute': " << dlsym_error << std::endl;
//         dlclose(handle);
//         return 1;
//     }
//
//     int32_t result = execute_func(size, mask.data(), data.data());
//
//     // Write result to shared memory
//     try {
//         void* shared_mem = SharedMemoryHelper::open_shared_memory(shm_name, sizeof(int32_t));
//         SharedMemoryHelper::write_result(shared_mem, result);
//         munmap(shared_mem, sizeof(int32_t));
//     } catch (const std::exception& e) {
//         std::cerr << "Failed to write result to shared memory: " << e.what() << std::endl;
//         dlclose(handle);
//         return 1;
//     }
//
//     dlclose(handle);
//     return 0;
// }
// )TEST";
//
//     std::string testHarnessFileName = "test_wrapper.cpp";
//     std::ofstream testFile(testHarnessFileName);
//     testFile << testHarnessCode;
//     testFile.close();
//
//     std::string testExecName = "test_wrapper_exec";
//     std::string compileCmd = "cc -g " + testHarnessFileName + " -ldl -lstdc++ -lrt -o " + testExecName + " 2>&1";
//
//     if (std::system(compileCmd.c_str()) != 0) {
//         std::remove(testHarnessFileName.c_str());
//         std::remove(maskFileName.c_str());
//         std::remove(dataFileName.c_str());
//         SharedMemoryHelper::cleanup_shared_memory(shm_name, shared_mem, sizeof(int32_t));
//         throw std::runtime_error("Test harness compilation failed");
//     }
//
//     std::string sizeStr = std::to_string(size);
//
//     // execute query with rr
//     std::string rrCmd = "rr record " + testExecName + " " + sizeStr + " " + shm_name;
//     int rrResult = std::system(rrCmd.c_str());
//
//
// 	// TODO: handle this properly
//     // std::remove(testHarnessFileName.c_str());
//     std::remove(testExecName.c_str());
//     std::remove(maskFileName.c_str());
//     std::remove(dataFileName.c_str());
//
//     // Read actual result
//     int32_t actual_result = SharedMemoryHelper::read_result(shared_mem);
//
//     // cleanup
//     SharedMemoryHelper::cleanup_shared_memory(shm_name, shared_mem, sizeof(int32_t));
//
//     if (rrResult != 0) {
//         throw std::runtime_error("rr record failed");
//     }
//
//     return actual_result;
// }
//
// }
// )";

	// Create wrapper shared library
	std::string wrapperFileName = fileName + "_wrapper";
	auto wrapperSourceFileName = wrapperFileName + ".cpp";
	auto wrapperLibraryFileName = wrapperFileName +
#ifdef __linux__
	                       ".so";
#elif defined(__APPLE__)
	                       ".dylib";
#else
#error "Unknown platform"
#endif

	auto wrapperFile = common::File::createFile(wrapperSourceFileName, wrapperCode);

	auto wrapperCompilationFlags = CPPCompilerFlags::createDefaultCompilerFlags();
	wrapperCompilationFlags.addFlag("-shared");
	wrapperCompilationFlags.addFlag("-g");
	wrapperCompilationFlags.addFlag("-ldl");
	wrapperCompilationFlags.addFlag("-fPIC");

	// add header paths for wrapper
	for (auto includePath : runtimePathConfig.includePaths) {
		wrapperCompilationFlags.addFlag("-I" + includePath);
	}

	wrapperCompilationFlags.addFlag("-o" + wrapperLibraryFileName);
	wrapperCompilationFlags.addFlag(wrapperSourceFileName);

	compileSharedLib(wrapperCompilationFlags, wrapperFile, wrapperLibraryFileName);

	// Load the wrapper shared library instead of the original
	auto sharedLibrary = SharedLibrary::load(wrapperLibraryFileName);

	// auto sharedLibrary = SharedLibrary::load(libraryFileName);

	// Clean up files after loading the wrapper (but keep original library since wrapper needs it)
	std::filesystem::remove(sourceFileName);
	std::filesystem::remove(wrapperSourceFileName);
	std::filesystem::remove(wrapperLibraryFileName);

	// Note: We don't remove libraryFileName here because the wrapper library needs it at runtime

	// timer.pause();
	/* if (!request->enableDebugging()) {
	     std::filesystem::remove(sourceFileName);
	     std::filesystem::remove(libraryFileName);
	 }*/
	// NES_INFO("CPPCompiler Runtime: " << (double) timer.getRuntime() / (double)
	// 1000000 << "ms");// print runtime
	return sharedLibrary;
}

void CPPCompiler::compileSharedLib(CPPCompilerFlags flags, std::shared_ptr<common::File> sourceFile,
                                   std::string) const {
	// lock file, such that no one can operate on the file at the same time
	const std::lock_guard<std::mutex> fileLock(sourceFile->getFileMutex());

	std::stringstream compilerCall;
	compilerCall << " cc ";
	for (const auto& arg : flags.getFlags()) {
		compilerCall << arg << " ";
	}
	// NES_ERROR("Compiler: compile with: '" << compilerCall.str() << "'");
	//  Creating a pointer to an open stream and a buffer, to read the output of
	//  the compiler
	FILE* fp = nullptr;
	char buffer[8192];

	// Redirecting stderr to stdout, to be able to read error messages
	compilerCall << " 2>&1";

	// Calling the compiler in a new process
	fp = popen(compilerCall.str().c_str(), "r");

	if (fp == nullptr) {
		// NES_ERROR("Compiler: failed to run command\n");
		return;
	}

	// Collecting the output of the compiler to a string stream
	std::ostringstream strstream;
	while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
		strstream << buffer;
	}

	// Closing the stream, which also gives us the exit status of the compiler
	// call
	auto ret = pclose(fp);

	// If the compilation didn't return with 0, we throw an exception containing
	// the compiler output
	if (ret != 0) {
		// NES_ERROR("Compiler: compilation of " << libraryFileName << " failed.");
		throw std::runtime_error(strstream.str());
	}
}

} // namespace nautilus::compiler::cpp
