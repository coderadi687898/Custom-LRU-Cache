cmake_minimum_required(VERSION 3.14)
project(LRUCacheProject VERSION 1.0 LANGUAGES CXX)

enable_testing()

# Require C++17 for std::optional
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Define our LRU Cache as an INTERFACE library (header-only)
add_library(lru_cache INTERFACE)
target_include_directories(lru_cache INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 2. Fetch Google Test automatically
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 3. Create the test executable
add_executable(lru_cache_test tests/test_lru_cache.cpp)

# 4. Link our library and gtest to the executable
target_link_libraries(lru_cache_test
    PRIVATE
    lru_cache
    GTest::gtest_main
)

# 5. Register the tests with CTest (CMake's test runner)
include(GoogleTest)
gtest_discover_tests(lru_cache_test)