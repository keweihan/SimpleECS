set(TEST_TARGETS
  perfCollisionStress
  perfCollisionSparse
)

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
endforeach()