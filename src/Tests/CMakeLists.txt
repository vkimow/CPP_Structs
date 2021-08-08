# auto sources
file(GLOB_RECURSE TEST_SOURCES ${STRUCTS_SOURCE_DIR}/*.cpp ${STRUCTS_SOURCE_DIR}/*.h)

foreach(FILE IN ITEMS ${TEST_SOURCES})
    get_filename_component(FILE_PATH "${FILE}" REALPATH)
    get_filename_component(FILE_PATH "${FILE_PATH}" PATH)
    file(RELATIVE_PATH FILE_PATH "${STRUCTS_SOURCE_DIR}/Tests" "${FILE_PATH}")
    
    string(REPLACE "/" "\\" FILE_PATH "${FILE_PATH}")
    string(REPLACE "..\\" "" FILE_PATH "${FILE_PATH}")

    source_group("${FILE_PATH}" FILES "${FILE}")
endforeach()

# add executable
add_executable(Tests ${TEST_SOURCES})
target_link_libraries(Tests PUBLIC Structs gtest_main)
set_target_properties(Tests PROPERTIES FOLDER Main)

include(GoogleTest)
gtest_discover_tests(Tests)
