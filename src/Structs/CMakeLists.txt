# auto sources
file(GLOB_RECURSE STRUCTS_SOURCES ${STRUCTS_SOURCE_DIR}/*.cpp ${STRUCTS_SOURCE_DIR}/*.h)
set(STRUCTS_SOURCES ${STRUCTS_SOURCES} CACHE INTERNAL "")

foreach(FILE IN ITEMS ${STRUCTS_SOURCES})
    get_filename_component(FILE_PATH "${FILE}" REALPATH)
    get_filename_component(FILE_PATH "${FILE_PATH}" PATH)
    file(RELATIVE_PATH FILE_PATH "${STRUCTS_SOURCE_DIR}/Structs" "${FILE_PATH}")
    
    string(REPLACE "/" "\\" FILE_PATH "${FILE_PATH}")
    string(REPLACE "..\\" "" FILE_PATH "${FILE_PATH}")

    source_group("${FILE_PATH}" FILES "${FILE}")
endforeach()

# auto public headers
file(GLOB_RECURSE STRUCTS_PUBLIC_HEADERS ${STRUCTS_INCLUDE_DIR}/*.h)
set(STRUCTS_PUBLIC_HEADERS ${STRUCTS_PUBLIC_HEADERS} CACHE INTERNAL "")

foreach(FILE IN ITEMS ${STRUCTS_PUBLIC_HEADERS})
    get_filename_component(FILE_PATH "${FILE}" REALPATH)
    get_filename_component(FILE_PATH "${FILE_PATH}" PATH)
    file(RELATIVE_PATH FILE_PATH "${STRUCTS_INCLUDE_DIR}/Structs" "${FILE_PATH}")
    
    string(REPLACE "/" "\\" FILE_PATH "${FILE_PATH}")
    string(REPLACE "..\\" "" FILE_PATH "${FILE_PATH}")

    source_group("${FILE_PATH}" FILES "${FILE}")
endforeach()

add_library(${PROJECT_NAME} ${STRUCTS_SOURCES} ${STRUCTS_PUBLIC_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${STRUCTS_INCLUDE_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Main)
set_property(DIRECTORY ${STRUCTS_PROJECT_DIR} PROPERTY VS_STARTUP_PROJECT Structs)
