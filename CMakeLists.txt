cmake_minimum_required(VERSION 3.14)
project(blogator
        VERSION 2.0.0
        DESCRIPTION  "Static site generator"
        HOMEPAGE_URL "eadavison.com")

set(PROJECT_AUTHOR "E.A.Davison")
set(PROJECT_YEARS  "2019-21")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror=range-loop-construct -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS --coverage)

include(ExternalProject)
include(GNUInstallDirs)

#Build directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_EXTERNAL_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(CMAKE_TEST_HARNESS_DIRECTORY    "${CMAKE_CURRENT_SOURCE_DIR}/test-harness")

#External imports/dependencies directories
set(CMAKE_RESOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(CMAKE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(TEMP_DIR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tmp")
set(SUBMODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/submodules")

#Installation directories
set(CTUNE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
set(CTUNE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
set(CTUNE_INSTALL_MANDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_MANDIR}/man1)


set(CMAKE_INSTALL_RPATH ${CTUNE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

option(TESTING "Set testing on" ON)

if(TESTING)
    add_definitions(-DTESTING)
endif()

#========================================== PRE-CHECKS ============================================#
include(FindGit)
find_package(Git)

if (NOT Git_FOUND)
    message(FATAL_ERROR "Required package 'Git' not found on system.")
endif ()

#========================================== LIB IMPORTS ===========================================#

####################################################################################################
# nlohmann/JSON                                                                                    #
# ------------------------------------------------------------------------------------------------ #
# https://github.com/nlohmann/json                                                                 #
####################################################################################################
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${PROJECT_SOURCE_DIR}/submodules/json)

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
        src/exception/not_implemented.h
        src/exception/logger_exception.cpp
        src/exception/logger_exception.h
        src/dom/exception/DOMException.h
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
        src/dom/dot/DOTConfig.cpp
        src/dom/dot/DOTConfig.h
        src/dom/dot/DOTNode.cpp
        src/dom/dot/DOTNode.h
        src/dom/dot/enums/ComplianceLevel.h
        src/dom/dot/enums/DOTSetting.h
        src/dom/dto/Attribute.cpp
        src/dom/dto/Attribute.h
        src/dom/dto/GlobalMaps.h
        src/dom/dto/TagSpecs.h
        src/dom/dto/Text.cpp
        src/dom/dto/Text.h
        src/dom/html5/special_char.h
        src/dom/html5/enums/AttrBoundaryChar.h
        src/dom/html5/enums/Attribute.h
        src/dom/html5/enums/ContentModel.h
        src/dom/html5/enums/NameSpace.h
        src/dom/html5/enums/Tag.h
        src/dom/html5/enums/TagStructure.h
        src/dom/html5/kb/kb_attributes.cpp
        src/dom/html5/kb/kb_attributes.h
        src/dom/html5/kb/kb_relationships.cpp
        src/dom/html5/kb/kb_relationships.h
        src/dom/html5/kb/kb_tags.cpp
        src/dom/html5/kb/kb_tags.h
        src/dom/html5/kb/KnowledgeBase.cpp
        src/dom/html5/kb/KnowledgeBase.h
        src/dom/html5/properties/content_model.cpp
        src/dom/html5/properties/content_model.h
        src/dom/html5/properties/relationships.cpp
        src/dom/html5/properties/relationships.h
        src/dom/html5/validator/validator.cpp
        src/dom/html5/validator/validator.h
        src/dom/parser/Parser.cpp
        src/dom/parser/Parser.h
        src/dom/parser/Tokeniser.cpp
        src/dom/parser/Tokeniser.h

        src/exception/parsing_failure.cpp
        src/exception/parsing_failure.h

        src/logger/Logger.cpp
        src/logger/Logger.h

        src/parser/Parser.cpp
        src/parser/Parser.h

        src/datastructure/Trie.h
        src/datastructure/TrieNode.h
        src/datastructure/TrieTracker.h

        src/parser/dto/TextPos.cpp
        src/parser/dto/TextPos.h
        src/parser/dto/U32Text.cpp
        src/parser/dto/U32Text.h
        src/parser/dto/Source.cpp
        src/parser/dto/Source.h

        src/parser/specs/Structure.h
        src/parser/specs/Context.cpp
        src/parser/specs/Context.h
        src/parser/specs/config/tokeniser/TokenType.h
        src/parser/specs/native/Element.cpp
        src/parser/specs/native/Element.h
        src/parser/specs/native/ErrorCode.cpp
        src/parser/specs/native/ErrorCode.h
        src/parser/specs/native/InsertionMode.cpp
        src/parser/specs/native/InsertionMode.h
        src/parser/specs/native/specifications.h
        src/parser/specs/infra/tokeniser/TokeniserState.h
        src/parser/specs/infra/tokeniser/TokenType.cpp
        src/parser/specs/infra/tokeniser/TokenType.h
        src/parser/specs/infra/Attribute.cpp
        src/parser/specs/infra/Attribute.h
        src/parser/specs/infra/ContentModel.cpp
        src/parser/specs/infra/ContentModel.h
        src/parser/specs/infra/ContentType.cpp
        src/parser/specs/infra/ContentType.h
        src/parser/specs/infra/DocType.cpp
        src/parser/specs/infra/DocType.h
        src/parser/specs/infra/Element.cpp
        src/parser/specs/infra/Element.h
        src/parser/specs/infra/Namespace.cpp
        src/parser/specs/infra/Namespace.h
        src/parser/specs/infra/NamedCharRef.cpp
        src/parser/specs/infra/NamedCharRef.h
        src/parser/specs/infra/NumericCharRef.cpp
        src/parser/specs/infra/NumericCharRef.h
        src/parser/specs/infra/specifications.h
        src/parser/specs/infra/ErrorCode.h
        src/parser/specs/infra/ErrorCode.cpp
        src/parser/specs/infra/InsertionMode.cpp
        src/parser/specs/infra/InsertionMode.h

        src/parser/specs/markdown/specifications.h

        src/parser/token/Token.cpp
        src/parser/token/Token.h
        src/parser/token/config/tokens.h
        src/parser/token/config/ConfigTk.cpp
        src/parser/token/config/ConfigTk.h
        src/parser/token/html5/HTML5Tk.cpp
        src/parser/token/html5/HTML5Tk.h
        src/parser/token/html5/DoctypeTk.cpp
        src/parser/token/html5/DoctypeTk.h
        src/parser/token/html5/CommentTk.cpp
        src/parser/token/html5/CommentTk.h
        src/parser/token/html5/CharacterTk.cpp
        src/parser/token/html5/CharacterTk.h
        src/parser/token/html5/GenericTagTk.cpp
        src/parser/token/html5/GenericTagTk.h
        src/parser/token/html5/StartTagTk.cpp
        src/parser/token/html5/StartTagTk.h
        src/parser/token/html5/EndTagTk.cpp
        src/parser/token/html5/EndTagTk.h
        src/parser/token/html5/EndOfFileTk.cpp
        src/parser/token/html5/EndOfFileTk.h
        src/parser/token/html5/tokens.h
        src/parser/token/native/NativeTk.cpp
        src/parser/token/native/NativeTk.h
        src/parser/token/native/tokens.h
        src/parser/token/markdown/MarkdownTk.cpp
        src/parser/token/markdown/MarkdownTk.h
        src/parser/token/markdown/tokens.h

        src/parser/dom/node/Node.cpp
        src/parser/dom/node/Node.h

        src/parser/tokeniser/Native.cpp
        src/parser/tokeniser/Native.h
        src/parser/tokeniser/HTML5.cpp
        src/parser/tokeniser/HTML5.h
        src/parser/tokeniser/Markdown.cpp
        src/parser/tokeniser/Markdown.h

        src/parser/encoding/Transcode.cpp
        src/parser/encoding/Transcode.h
        src/parser/encoding/Format.cpp
        src/parser/encoding/Format.h
        src/parser/encoding/Confidence.h

        src/parser/logging/ErrorObject.cpp
        src/parser/logging/ErrorObject.h
        src/parser/logging/ParserLog.cpp
        src/parser/logging/ParserLog.h

        src/unicode/ascii.cpp
        src/unicode/ascii.h
        src/unicode/utf8.cpp
        src/unicode/utf8.h
        src/unicode/utf16.cpp
        src/unicode/utf16.h
        src/unicode/utf32.cpp
        src/unicode/utf32.h
        src/unicode/unicode.cpp
        src/unicode/unicode.h

        src/parser/tokeniser/Config.cpp
        src/parser/tokeniser/Config.h

        src/parser/dom/enum/NodeType.cpp
        src/parser/dom/enum/NodeType.h
        src/parser/dom/enum/QuirksMode.cpp
        src/parser/dom/enum/QuirksMode.h
        src/parser/dom/node/Document.cpp
        src/parser/dom/node/Document.h
        src/parser/dom/node/Element.cpp
        src/parser/dom/node/Element.h
        src/parser/dom/node/Comment.cpp
        src/parser/dom/node/Comment.h
        src/parser/dom/node/Text.cpp
        src/parser/dom/node/Text.h
        src/parser/dom/node/DocumentType.cpp
        src/parser/dom/node/DocumentType.h
        src/parser/dom/node/DocumentFragment.cpp
        src/parser/dom/node/DocumentFragment.h
        src/parser/dom/node/Attr.cpp
        src/parser/dom/node/Attr.h
        src/parser/dom/node/CDATASection.cpp
        src/parser/dom/node/CDATASection.h
        src/parser/dom/node/CharacterData.cpp
        src/parser/dom/node/CharacterData.h

        src/parser/dom/exception/DOMException.h
        src/parser/dom/exception/DOMExceptionType.cpp
        src/parser/dom/exception/DOMExceptionType.h
        src/parser/dom/exception/EvalError.cpp
        src/parser/dom/exception/EvalError.h
        src/parser/dom/exception/RangeError.cpp
        src/parser/dom/exception/RangeError.h
        src/parser/dom/exception/DOMException.cpp
        src/parser/dom/exception/ReferenceError.cpp
        src/parser/dom/exception/ReferenceError.h
        src/parser/dom/exception/TypeError.cpp
        src/parser/dom/exception/TypeError.h
        src/parser/dom/exception/URIError.cpp
        src/parser/dom/exception/URIError.h

        src/parser/dom/exception/exception.h
        src/exception/failed_expectation.cpp

        src/logger/LoggableException.h
        src/logger/LoggableException.cpp
        src/logger/engine/LogQueue.h
        src/logger/engine/LogWriter.cpp
        src/logger/engine/LogWriter.h
        src/logger/formatter/DefaultPrintMono.cpp
        src/logger/formatter/DefaultPrintMono.h
        src/logger/formatter/DefaultPrintColour.cpp
        src/logger/formatter/DefaultPrintColour.h
        src/logger/formatter/DefaultFile.cpp
        src/logger/formatter/DefaultFile.h
        src/logger/output/LogOutput.h
        src/logger/output/File.cpp
        src/logger/output/File.h
        src/logger/output/LogOutput.cpp
        src/logger/output/Terminal.cpp
        src/logger/output/Terminal.h
        src/logger/output/SysLog.cpp
        src/logger/output/SysLog.h
        src/logger/global/LogMsg.cpp
        src/logger/global/LogMsg.h
        src/logger/global/LogLevel.cpp
        src/logger/global/LogLevel.h
        src/logger/global/TimeStamp.cpp
        src/logger/global/TimeStamp.h
        src/logger/formatter/LogFormatter.cpp
        src/logger/formatter/LogFormatter.h
        src/logger/output/Stream.cpp
        src/logger/output/Stream.h
        src/parser/dom/defs.h
        src/parser/dom/validation/Validator.cpp
        src/parser/dom/validation/Validator.h
        src/parser/dom/Variables.cpp
        src/parser/dom/Variables.h
        src/parser/dom/datastructure/NamedNodeMap.cpp
        src/parser/dom/datastructure/NamedNodeMap.h
        src/parser/dom/filter/NodeFilter.cpp
        src/parser/dom/filter/NodeFilter.h
        src/parser/dom/iterator/NodeIterator.cpp
        src/parser/dom/iterator/NodeIterator.h
        src/parser/dom/iterator/ConstNodeIterator.cpp
        src/parser/dom/iterator/ConstNodeIterator.h
        src/parser/dom/iterator/TreeWalker.cpp
        src/parser/dom/iterator/TreeWalker.h
        src/parser/dom/TreeBuilder.cpp
        src/parser/dom/TreeBuilder.h
        src/parser/dom/datastructure/NamespaceMap.cpp
        src/parser/dom/datastructure/NamespaceMap.h
        src/parser/dom/validation/XML.cpp
        src/parser/dom/validation/XML.h
        src/sys/terminate.cpp
        src/sys/terminate.h src/parser/dom/enum/ValidationState.h

        src/parser/specs/markdown/tokeniser/TokenType.cpp
        src/parser/specs/markdown/tokeniser/TokenType.h
        src/parser/specs/markdown/tokeniser/TokeniserState.h

        src/parser/builder/MarkdownToHtml.cpp
        src/parser/builder/MarkdownToHtml.h
        src/parser/token/markdown/CharacterTk.cpp
        src/parser/token/markdown/CharacterTk.h
        src/parser/token/markdown/HeadingTk.cpp
        src/parser/token/markdown/HeadingTk.h
        src/parser/token/markdown/FormatBeginTk.cpp
        src/parser/token/markdown/FormatBeginTk.h
        src/parser/token/markdown/FormattingTk.cpp
        src/parser/token/markdown/FormattingTk.h
        src/parser/token/markdown/FormatEndTk.cpp
        src/parser/token/markdown/FormatEndTk.h
        src/parser/token/markdown/BlockIDTk.cpp
        src/parser/token/markdown/BlockIDTk.h
        src/parser/token/markdown/LineBreakTk.cpp
        src/parser/token/markdown/LineBreakTk.h
        src/parser/token/markdown/EndOfFileTk.cpp
        src/parser/token/markdown/EndOfFileTk.h)

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
            GIT_TAG main
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GTEST_DOWNLOAD_PATH})

    include_directories(${GTEST_DOWNLOAD_PATH}/include)
    link_directories(${GTEST_DOWNLOAD_PATH}/lib64)
else()
    message(">> GoogleTest package found. Using local version.")
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

set(TEST_FILES
        tests/main.cpp
        tests/TestHelpers/AsyncNotify.h
        tests/TestHelpers/UnicodeTestStrings.cpp
        tests/TestHelpers/UnicodeTestStrings.h
        tests/TestHelpers/TestDOMs.cpp
        tests/TestHelpers/TestDOMs.h
        tests/html/reader/reader.cpp
        tests/dto/DateStamp.cpp
        tests/dto/Templates.cpp
        tests/dto/Article.cpp
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
        tests/dom/dto/Attribute.cpp
        tests/parser/tokeniser/Blogator.cpp
        tests/parser/dto/U32Text.cpp
        tests/parser/dto/TextPos.cpp
        tests/datastructure/TrieNode.cpp
        tests/datastructure/Trie.cpp
        tests/parser/specs/infra/NamedCharRef.cpp
        tests/helper.cpp
        tests/helper.h
        tests/unicode/ascii.cpp
        tests/unicode/utf32.cpp
        tests/unicode/utf8.cpp
        tests/parser/encoding/Transcode.cpp
        tests/unicode/utf16.cpp
        tests/parser/dom/node/Node.cpp
        tests/logger/global/LogLevel.cpp
        tests/logger/global/TimeStamp.cpp
        tests/logger/Logger.cpp
        tests/logger/engine/LogQueue.cpp
        tests/parser/specs/infra/Element.cpp
        tests/parser/specs/infra/ContentModel.cpp
        tests/parser/specs/infra/Namespace.cpp
        tests/parser/specs/infra/InsertionMode.cpp
        tests/parser/specs/infra/Attribute.cpp
        tests/parser/specs/Context.cpp
        tests/parser/specs/native/Element.cpp
        tests/parser/specs/native/InsertionMode.cpp
        tests/parser/dom/node/Element.cpp
        tests/parser/dom/node/CharacterData.cpp
        tests/parser/dom/node/Text.cpp
        tests/parser/dom/node/Comment.cpp
        tests/parser/dom/node/CDATASection.cpp
        tests/parser/dom/node/Document.cpp
        tests/parser/dom/node/DocumentType.cpp
        tests/parser/dom/node/Attr.cpp
        tests/parser/dom/filter/NodeFilter.cpp
        tests/parser/dom/iterator/NodeIterator.cpp
        tests/parser/dom/iterator/ConstNodeIterator.cpp
        tests/parser/dom/iterator/TreeWalker.cpp
        tests/parser/dom/datastructure/NamedNodeMap.cpp
        tests/parser/dom/datastructure/NamespaceMap.cpp
        tests/parser/specs/infra/ContentType.cpp
        tests/parser/dom/validation/XML.cpp
        tests/parser/specs/infra/DocType.cpp

        test-harness/helpers/MarkdownTkTest.h
        test-harness/helpers/TreeConstructionTest.h
        test-harness/helpers/helpers.cpp
        test-harness/helpers/helpers.h
        test-harness/html5lib-tests/tokenizer-tests.cpp
        test-harness/html5lib-tests/tree-construction-tests.cpp
        test-harness/commonmark-spec/commonmark-spec-tests.cpp
        test-harness/markdown/tokenizer-tests.cpp
        test-harness/helpers/CommonMarkTest.h)

configure_file(tests/cmake/cmake.h ${CMAKE_BINARY_DIR}/generated-src/cmake_test_vars.h )
configure_file(test-harness/sources.h ${CMAKE_BINARY_DIR}/generated-src/test-harness-sources.h)

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
    target_link_libraries(blogator_tests pthread gtest gmock nlohmann_json::nlohmann_json)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)

    if(Python3_Interpreter_FOUND)
        execute_process(
                COMMAND ${Python_EXECUTABLE} test/spec_tests.py --dump-tests
                WORKING_DIRECTORY ${SUBMODULE_PATH}/commonmark-spec
                OUTPUT_FILE tests.json
        )
    else()
        message(FATAL_ERROR "Required package 'Python3::interpreter' not found on system.")
    endif()
endif()

add_test(blogator_tests ${TEST_FILES})

#(debuging) prints cmake vars
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

############
# BLOGATOR #
############
include_directories(external)
add_executable(blogator src/main.cpp ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} stdc++fs pthread)