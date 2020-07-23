cmake_minimum_required(VERSION 3.14)
project(blogator VERSION 1.3.2)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(PROJECT_AUTHOR "An7ar35")
set(PROJECT_HOMEPAGE_URL "https://an7ar35.bitbucket.io")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_EXTERNAL_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(CMAKE_RESOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resource")
set(CMAKE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(TEMP_DIR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tmp")

include(ExternalProject)

##########################
# Blogator Source Files #
##########################
set(SOURCE_FILES
        src/cli/ArgParser.cpp
        src/cli/ArgParser.h
        src/cli/MsgInterface.cpp
        src/cli/MsgInterface.h
        src/cli/MsgDisplay.cpp
        src/cli/MsgDisplay.h
        src/dto/Article.h
        src/dto/DateStamp.h
        src/dto/Template.h
        src/dto/Template.cpp
        src/dto/Templates.h
        src/dto/Templates.cpp
        src/dto/Options.h
        src/dto/Options.cpp
        src/dto/Index.h
        src/dto/HTML.cpp
        src/dto/HTML.h
        src/dto/IndexDateTree.h
        src/dto/IndexTagTree.h
        src/dto/Page.h
        src/dto/Line.h
        src/dto/SeekRange.h
        src/dto/InsertPosition.h
        src/dto/TableOfContents.h
        src/dto/TableOfContents.cpp
        src/exception/file_access_failure.h
        src/exception/file_parsing_failure.h
        src/exception/failed_expectation.h
        src/exception/DOMException.h
        src/fs/fs.cpp
        src/fs/fs.h
        src/fs/ConfigReader.cpp
        src/fs/ConfigReader.h
        src/string/helpers.h
        src/html/html.cpp
        src/html/html.h
        src/html/writer/writer.cpp
        src/html/writer/writer.h
        src/html/reader/reader.cpp
        src/html/reader/reader.h
        src/indexer/indexer.cpp
        src/indexer/indexer.h
        src/indexer/FeatAggregator.cpp
        src/indexer/FeatAggregator.h
        src/output/output.cpp
        src/output/output.h
        src/output/abstract/Breadcrumb.cpp
        src/output/abstract/Breadcrumb.h
        src/output/abstract/Page.cpp
        src/output/abstract/Page.h
        src/output/abstract/IndexList.cpp
        src/output/abstract/IndexList.h
        src/output/abstract/Lister.cpp
        src/output/abstract/Lister.h
        src/output/generic/EntryWriter.cpp
        src/output/generic/EntryWriter.h
        src/output/generic/CategoryLister.cpp
        src/output/generic/CategoryLister.h
        src/output/generic/ChronoIndexLister.cpp
        src/output/generic/ChronoIndexLister.h
        src/output/page/Posts.cpp
        src/output/page/Posts.h
        src/output/page/Indices.cpp
        src/output/page/Indices.h
        src/output/page/Landing.cpp
        src/output/page/Landing.h
        src/output/page/ByYearList.cpp
        src/output/page/ByYearList.h
        src/output/page/ByTagList.cpp
        src/output/page/ByTagList.h
        src/output/page/ByAuthorList.cpp
        src/output/page/ByAuthorList.h
        src/output/feed/RSS.cpp
        src/output/feed/RSS.h
        src/output/json/JSON.cpp
        src/output/json/JSON.h
        src/html/editor/code/code.cpp
        src/html/editor/code/code.h
        src/html/editor/editor.cpp
        src/html/editor/editor.h
        src/output/helper/OrderedPostInsertion.cpp
        src/output/helper/OrderedPostInsertion.h
        src/checker/Checker.cpp
        src/checker/Checker.h
        src/encoding/encoding.cpp
        src/encoding/encoding.h
        src/dom/dot/DOT.cpp
        src/dom/dot/DOT.h
        src/dom/dot/DOTNode.cpp
        src/dom/dot/DOTNode.h
        src/dom/dto/Attribute.cpp
        src/dom/dto/Attribute.h
        src/dom/dto/GlobalMaps.h
        src/dom/dto/TagSpecs.h
        src/dom/dto/Text.cpp
        src/dom/dto/Text.h
        src/dom/html5/Html5Properties.cpp
        src/dom/html5/Html5Properties.h
        src/dom/html5/Html5Relationships.cpp
        src/dom/html5/Html5Relationships.h
        src/dom/html5/special_char.h
        src/dom/html5/enums/AttrBoundaryChar.h
        src/dom/html5/enums/Attribute.h
        src/dom/html5/enums/ContentModel.h
        src/dom/html5/enums/InsertMode.h
        src/dom/html5/enums/NameSpaces.h
        src/dom/html5/enums/Tag.h
        src/dom/html5/enums/TagStructure.h
        src/dom/html5/properties/attribute.cpp
        src/dom/html5/properties/attribute.h
        src/dom/html5/properties/content_model.cpp
        src/dom/html5/properties/content_model.h
        src/dom/html5/properties/relationships.cpp
        src/dom/html5/properties/relationships.h
        src/dom/html5/properties/tag.cpp
        src/dom/html5/properties/tag.h
        src/dom/html5/rules/context.h
        src/dom/html5/rules/insert_mode.h
        src/dom/html5/rules/OptionalTags.cpp
        src/dom/html5/rules/OptionalTags.h
        src/dom/html5/validator/validator.cpp
        src/dom/html5/validator/validator.h
        src/dom/parser/Parser.cpp
        src/dom/parser/Parser.h
        src/dom/parser/Tokeniser.cpp
        src/dom/parser/Tokeniser.h)


configure_file(src/cmake/cmake_vars.h ${CMAKE_BINARY_DIR}/generated-src/project_version.h )
include_directories( ${CMAKE_BINARY_DIR}/generated-src/ )

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
    link_directories(${GTEST_DOWNLOAD_PATH}/lib64)
else()
    message(">> GoogleTest package found. Using local version.")
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

set(TEST_FILES
        tests/main.cpp
        tests/html/reader/reader.cpp
        tests/dto/DateStamp.cpp
        tests/dto/Templates.cpp
        tests/fs/fs.cpp
        tests/html/editor/editor.cpp
        tests/dto/HTML.cpp
        tests/dto/TableOfContents.cpp
        tests/output/json/json.cpp
        tests/string/helpers_tests.cpp
        tests/dom/dot/DOT_tests.cpp
        tests/dom/dot/DOTNode_tests.cpp
        tests/dom/parser/Tokeniser_tests.cpp
        tests/dom/parser/Parser_tests.cpp
        tests/dom/html5/validator/validator_tests.cpp
        tests/dom/html5/validator/validator_validateOpeningTag_tests.cpp
        tests/dom/html5/validator/validator_validateClosingTag_tests.cpp
        tests/dom/html5/validator/validator_validateAttributes_tests.cpp
        tests/dom/html5/Html5Properties_tests.cpp
        tests/dom/html5/Html5Relationships_tests.cpp)

enable_testing()

add_executable(
        blogator_tests
        ${SOURCE_FILES}
        ${TEST_FILES}
)

if( NOT GTEST_FOUND )
    add_dependencies(blogator_tests googletest)
    target_link_libraries(blogator_tests gtest)
else()
    target_link_libraries(blogator_tests gtest)
endif()

add_test(blogator_tests ${TEST_FILES})

############
# BLOGATOR #
############
include_directories(external)
add_executable(blogator src/main.cpp ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} stdc++fs)