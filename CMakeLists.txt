cmake_minimum_required(VERSION 3.14)
project(blogator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_EXTERNAL_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(CMAKE_RESOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resource")
set(CMAKE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(TEMP_DIR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tmp")

include(ExternalProject)

#####################
# EADLIB dependency #
#####################
set(EADLIB_RELEASE_URL "https://github.com/An7ar35/eadlib/releases/download/v0.1.3a/eadlib.tar.gz")
set(EADLIB_RELEASE_MD5 "7418adcd9b942f68315c4c86ea6a571a")
set(EADLIB_TAR_FILENAME "eadlib.tar.gz")
set(EADLIB_DOWNLOAD_PATH "${CMAKE_EXTERNAL_OUTPUT_DIRECTORY}/${EADLIB_TAR_FILENAME}")
set(EADLIB_EXTRACTED_FILEPATH "${CMAKE_EXTERNAL_OUTPUT_DIRECTORY}/eadlib")

if(NOT EXISTS ${EADLIB_EXTRACTED_FILEPATH})
    message(">> EADlib dependency missing. Downloading...")

    file(DOWNLOAD "${EADLIB_RELEASE_URL}" "${EADLIB_DOWNLOAD_PATH}"
            STATUS status
            #SHOW_PROGRESS 1
            EXPECTED_MD5 ${EADLIB_RELEASE_MD5})

    list(GET status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Could not download EADlib dependency package.")
    else()
        message(">> EADlib package downloaded.")
        file(MAKE_DIRECTORY ${EADLIB_EXTRACTED_FILEPATH})
    endif()

    execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf ${EADLIB_DOWNLOAD_PATH}
            WORKING_DIRECTORY ${EADLIB_EXTRACTED_FILEPATH})

    file(REMOVE ${EADLIB_DOWNLOAD_PATH})
else()
    message("Found EADLIB dependency folder.")
endif()

##########################
# Blogator Source Files #
##########################
set(SOURCE_FILES
        src/main.cpp
        src/dto/Article.h
        src/dto/DateStamp.h
        src/dto/Options.h
        src/dto/Index.h
        src/dto/HTML.h
        src/dto/IndexDateTree.h
        src/dto/IndexTagTree.h
        src/dto/Template.h
        src/exception/file_access_failure.h
        src/exception/file_parsing_failure.h
        src/exception/failed_expectation.h
        src/html/html.cpp
        src/html/html.h
        src/html/writer/writer.cpp
        src/html/writer/writer.h
        src/html/reader/reader.cpp
        src/html/reader/reader.h
        src/indexer/indexer.cpp
        src/indexer/indexer.h
        src/fs/fs.cpp
        src/fs/fs.h
        src/fs/ConfigReader.cpp
        src/fs/ConfigReader.h
        src/generator/Generator.cpp
        src/generator/Generator.h
        src/generator/PostMaker.cpp
        src/generator/PostMaker.h
        src/generator/IndexMaker.cpp
        src/generator/IndexMaker.h
        src/generator/RSS.cpp
        src/generator/RSS.h
        src/generator/LandingMaker.cpp
        src/generator/LandingMaker.h)

#####################
# Google Unit-Tests #
#####################
find_package(GTest QUIET)

if( NOT GTEST_FOUND )
    message(">> GoogleTest package not found. Getting it from git repo...")

    set(GTEST_DOWNLOAD_PATH "${CMAKE_EXTERNAL_OUTPUT_DIRECTORY}/googletest")

    ExternalProject_Add(googletest
            PREFIX ${GTEST_DOWNLOAD_PATH}
            GIT_REPOSITORY https://github.com/google/googletest.git
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GTEST_DOWNLOAD_PATH})

    include_directories(${GTEST_DOWNLOAD_PATH}/include)
    link_directories(${GTEST_DOWNLOAD_PATH}/lib)
else()
    message(">> GoogleTest package found. Using local version.")
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

set(TEST_FILES
        tests/main.cpp
        tests/html/reader/reader.cpp
        tests/dto/DateStamp.cpp)

enable_testing()

add_executable(
        blogator_tests
        ${TEST_FILES}
)

if( NOT GTEST_FOUND )
    add_dependencies(blogator_tests googletest)
endif()

target_link_libraries(blogator_tests gtest)
add_test(blogator_tests ${TEST_FILES})

############
# BLOGATOR #
############
include_directories(external)
add_executable(blogator ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} stdc++fs)