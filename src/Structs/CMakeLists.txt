# auto sources
file(GLOB_RECURSE STRUCTS_HEADERS ${STRUCTS_INCLUDE_DIR}/*.h)

foreach(FILE IN ITEMS ${STRUCTS_HEADERS})
    get_filename_component(FILE_PATH "${FILE}" REALPATH)
    get_filename_component(FILE_PATH "${FILE_PATH}" PATH)
    file(RELATIVE_PATH FILE_PATH "${STRUCTS_INCLUDE_DIR}/Structs" "${FILE_PATH}")
    
    string(REPLACE "/" "\\" FILE_PATH "${FILE_PATH}")
    string(REPLACE "..\\" "" FILE_PATH "${FILE_PATH}")

    source_group("${FILE_PATH}" FILES "${FILE}")
endforeach()

# add library
add_library(Structs INTERFACE)
target_include_directories(Structs INTERFACE ${STRUCTS_INCLUDE_DIR}/Structs)
target_compile_features(Structs INTERFACE cxx_std_17)

add_custom_target(Structs_IDE SOURCES ${STRUCTS_HEADERS})
set_target_properties(Structs_IDE PROPERTIES FOLDER Main)