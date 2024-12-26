include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_TARGETS
  testEcsCell
  testEcsDelete
)

include(GoogleTest)

foreach(target ${TEST_TARGETS})
    add_executable(${target} ${target}.cpp)
    
    # Link with SimpleECS (this should bring in all dependencies)
    target_link_libraries(${target} 
    PRIVATE 
      SimpleECS
    PUBLIC
      GTest::gtest_main
      Boost::boost
    )
    
    # Add only necessary include path
    target_include_directories(${target} PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
    )
    gtest_discover_tests(${target})
endforeach()