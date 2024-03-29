include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set_target_properties(gmock PROPERTIES FOLDER GTest)
set_target_properties(gmock_main PROPERTIES FOLDER GTest)
set_target_properties(gtest PROPERTIES FOLDER GTest)
set_target_properties(gtest_main PROPERTIES FOLDER GTest)
