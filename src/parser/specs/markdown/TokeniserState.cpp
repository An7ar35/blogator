#include "TokeniserState.h"

#include <sstream>
#include <unordered_map>

/**
 * Output stream operator
 * @param os Output stream
 * @param attr TokeniserState enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::markdown::operator <<( std::ostream &os, blogator::parser::specs::markdown::TokeniserState state ) {
    static const auto map = std::unordered_map<TokeniserState, std::string>( {
        { TokeniserState::BEFORE_BLOCK,                                                                                  "BEFORE_BLOCK" },
        { TokeniserState::AFTER_BLOCK,                                                                                   "AFTER_BLOCK" },
        { TokeniserState::AFTER_BLOCK_DOUBLE_NEWLINE,                                                                    "AFTER_BLOCK_DOUBLE_NEWLINE" },
        { TokeniserState::BLOCK_BEGIN,                                                                                   "BLOCK_BEGIN" },
        { TokeniserState::BLOCK_BEGIN_WHITESPACE,                                                                        "BLOCK_BEGIN_WHITESPACE" },
        { TokeniserState::BLOCK_BEGIN_AFTER_WHITESPACE,                                                                  "BLOCK_BEGIN_AFTER_WHITESPACE" },
        { TokeniserState::BLOCK_BEGIN_IN_CONTAINER_BLOCK,                                                                "BLOCK_BEGIN_IN_CONTAINER_BLOCK" },
        { TokeniserState::BLOCK_BEGIN_UNDERSCORE,                                                                        "BLOCK_BEGIN_UNDERSCORE" },
        { TokeniserState::BLOCK_BEGIN_UNDERSCORE_UNDERSCORE,                                                             "BLOCK_BEGIN_UNDERSCORE_UNDERSCORE" },
        { TokeniserState::BLOCK_BEGIN_ASTERISK,                                                                          "BLOCK_BEGIN_ASTERISK" },
        { TokeniserState::BLOCK_BEGIN_ASTERISK_ASTERISK,                                                                 "BLOCK_BEGIN_ASTERISK_ASTERISK" },
        { TokeniserState::BLOCK_BEGIN_ASTERISK_SPACE,                                                                    "BLOCK_BEGIN_ASTERISK_SPACE" },
        { TokeniserState::BLOCK_BEGIN_ASTERISK_SPACE_SPACE,                                                              "BLOCK_BEGIN_ASTERISK_SPACE_SPACE" },
        { TokeniserState::BLOCK_BEGIN_ASTERISK_TAB,                                                                      "BLOCK_BEGIN_ASTERISK_TAB" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN,                                                                            "BLOCK_BEGIN_HYPHEN" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_HYPHEN,                                                                     "BLOCK_BEGIN_HYPHEN_HYPHEN" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE,                                                                      "BLOCK_BEGIN_HYPHEN_SPACE" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE_SPACE,                                                                "BLOCK_BEGIN_HYPHEN_SPACE_SPACE" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_TAB,                                                                        "BLOCK_BEGIN_HYPHEN_TAB" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET,                                                  "BLOCK_BEGIN (\"- [\")" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET,                             "BLOCK_BEGIN (\"- []\")" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE,                                            "BLOCK_BEGIN (\"- [ \")" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE_RIGHT_SQUARE_BRACKET,                       "BLOCK_BEGIN (\"- [ ]\")" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X,                                      "BLOCK_BEGIN (\"- [x\")" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X_RIGHT_SQUARE_BRACKET,                 "BLOCK_BEGIN (\"- [x]\")" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X,                                      "BLOCK_BEGIN (\"- [X\")" },
        { TokeniserState::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X_RIGHT_SQUARE_BRACKET,                 "BLOCK_BEGIN (\"- [X]\")" },
        { TokeniserState::BLOCK_BEGIN_TASK_LIST_CHECKED_ITEM,                                                            "BLOCK_BEGIN_TASK_LIST_CHECKED_ITEM" },
        { TokeniserState::BLOCK_BEGIN_TASK_LIST_UNCHECKED_ITEM,                                                          "BLOCK_BEGIN_TASK_LIST_UNCHECKED_ITEM" },
        { TokeniserState::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN,                                                        "BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN" },

        { TokeniserState::LINE_BEGIN,                                                                                    "LINE_BEGIN" },
        { TokeniserState::LINE_BEGIN_ASTERISK,                                                                           "LINE_BEGIN_ASTERISK" },
        { TokeniserState::LINE_BEGIN_ASTERISK_SPACE,                                                                     "LINE_BEGIN_ASTERISK_SPACE" },
        { TokeniserState::LINE_BEGIN_EQUAL_SIGN,                                                                         "LINE_BEGIN_EQUAL_SIGN" },
        { TokeniserState::LINE_BEGIN_EQUAL_SIGN_EQUAL_SIGN,                                                              "LINE_BEGIN_EQUAL_SIGN_EQUAL_SIGN" },
        { TokeniserState::LINE_BEGIN_PLUS_SIGN,                                                                          "LINE_BEGIN_PLUS_SIGN" },
        { TokeniserState::LINE_BEGIN_HYPHEN,                                                                             "LINE_BEGIN_HYPHEN" },
        { TokeniserState::LINE_BEGIN_HYPHEN_HYPHEN,                                                                      "LINE_BEGIN_HYPHEN_HYPHEN" },
        { TokeniserState::LINE_BEGIN_HYPHEN_SPACE,                                                                       "LINE_BEGIN_HYPHEN_SPACE" },
        { TokeniserState::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET,                                                   "LINE_BEGIN (\"- [\")" },
        { TokeniserState::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET,                              "LINE_BEGIN (\"- []\")" },
        { TokeniserState::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE,                                             "LINE_BEGIN (\"- [ \")" },
        { TokeniserState::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE_RIGHT_SQUARE_BRACKET,                        "LINE_BEGIN (\"- [ ]\")" },
        { TokeniserState::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X,                                       "LINE_BEGIN (\"- [x\")" },
        { TokeniserState::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X_RIGHT_SQUARE_BRACKET,                  "LINE_BEGIN (\"- [x]\")" },
        { TokeniserState::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X,                                       "LINE_BEGIN (\"- [X\")" },
        { TokeniserState::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X_RIGHT_SQUARE_BRACKET,                  "LINE_BEGIN (\"- [X]\")" },
        { TokeniserState::LINE_BEGIN_TASK_LIST_CHECKED_ITEM,                                                             "LINE_BEGIN_TASK_LIST_CHECKED_ITEM" },
        { TokeniserState::LINE_BEGIN_TASK_LIST_UNCHECKED_ITEM,                                                           "LINE_BEGIN_TASK_LIST_UNCHECKED_ITEM" },
        { TokeniserState::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN,                                                         "LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN" },
        { TokeniserState::LINE_BEGIN_LOWER_ALPHA,                                                                        "LINE_BEGIN_LOWER_ALPHA" },
        { TokeniserState::LINE_BEGIN_LOWER_ALPHA_FULL_STOP,                                                              "LINE_BEGIN_LOWER_ALPHA_FULL_STOP" },
        { TokeniserState::LINE_BEGIN_UPPER_ALPHA,                                                                        "LINE_BEGIN_UPPER_ALPHA" },
        { TokeniserState::LINE_BEGIN_UPPER_ALPHA_FULL_STOP,                                                              "LINE_BEGIN_UPPER_ALPHA_FULL_STOP" },
        { TokeniserState::LINE_BEGIN_NUMERAL,                                                                            "LINE_BEGIN_NUMERAL" },
        { TokeniserState::LINE_BEGIN_NUMERAL_FULL_STOP,                                                                  "LINE_BEGIN_NUMERAL_FULL_STOP" },
        { TokeniserState::LINE_BEGIN_LEFT_SQUARE_BRACKET,                                                                "LINE_BEGIN_LEFT_SQUARE_BRACKET" },
        { TokeniserState::LINE_BEGIN_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT,                                              "LINE_BEGIN (\"[^\")" },
        { TokeniserState::LINE_BEGIN_FOOTNOTE_ID,                                                                        "LINE_BEGIN (\"[^...\")" },
        { TokeniserState::LINE_BEGIN_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET,                                                   "LINE_BEGIN (\"[^...]\")" },
        { TokeniserState::LINE_BEGIN_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET_COLON,                                             "LINE_BEGIN (\"[^...]:\")" },
        { TokeniserState::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK,                                                             "LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK" },
        { TokeniserState::LINE_BEGIN_COLON,                                                                              "LINE_BEGIN_COLON," },
        { TokeniserState::LINE_BEGIN_COLON_SPACE,                                                                        "LINE_BEGIN_COLON_SPACE," },

        { TokeniserState::NEWLINE_CONSUME_PREFIXES_BEGIN,                                                                "NEWLINE_CONSUME_PREFIXES_BEGIN" },
        { TokeniserState::NEWLINE_CONSUME_PREFIXES_CONTINUE,                                                             "NEWLINE_CONSUME_PREFIXES_CONTINUE" },
        { TokeniserState::NEWLINE_CONSUME_PREFIXES_END,                                                                  "NEWLINE_CONSUME_PREFIXES_END" },
        { TokeniserState::NEWLINE_CONSUME_WHITESPACE,                                                                    "NEWLINE_CONSUME_WHITESPACE" },
        { TokeniserState::NEWLINE_CONSUME_BLOCKQUOTE_PREFIX,                                                             "NEWLINE_CONSUME_BLOCKQUOTE_PREFIX" },
        { TokeniserState::NEWLINE_CONSUME_LIST_CONTAINER,                                                                "NEWLINE_CONSUME_LIST_CONTAINER" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX,                                                                   "NEWLINE_CONSUME_LIST_PREFIX" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_NUMERIC,                                                           "NEWLINE_CONSUME_LIST_PREFIX_NUMERIC" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_NUMERIC_FULL_STOP,                                                 "NEWLINE_CONSUME_LIST_PREFIX_NUMERIC_FULL_STOP" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_LOWER,                                                       "NEWLINE_CONSUME_LIST_PREFIX_ALPHA_LOWER" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_LOWER_FULL_STOP,                                             "NEWLINE_CONSUME_LIST_PREFIX_ALPHA_LOWER_FULL_STOP" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_UPPER,                                                       "NEWLINE_CONSUME_LIST_PREFIX_ALPHA_UPPER" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_UPPER_FULL_STOP,                                             "NEWLINE_CONSUME_LIST_PREFIX_ALPHA_UPPER_FULL_STOP" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_ASTERISK,                                                          "NEWLINE_CONSUME_LIST_PREFIX (\"*\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_PLUS_SIGN,                                                         "NEWLINE_CONSUME_LIST_PREFIX (\"+\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN,                                                            "NEWLINE_CONSUME_LIST_PREFIX (\"-\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE,                                                      "NEWLINE_CONSUME_LIST_PREFIX (\"- \")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET,                                  "NEWLINE_CONSUME_LIST_PREFIX (\"- [\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET,             "NEWLINE_CONSUME_LIST_PREFIX (\"- []\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE,                            "NEWLINE_CONSUME_LIST_PREFIX (\"- [ \")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE_RIGHT_SQUARE_BRACKET,       "NEWLINE_CONSUME_LIST_PREFIX (\"- [ ]\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X,                      "NEWLINE_CONSUME_LIST_PREFIX (\"- [x\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X_RIGHT_SQUARE_BRACKET, "NEWLINE_CONSUME_LIST_PREFIX (\"- [x]\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X,                      "NEWLINE_CONSUME_LIST_PREFIX (\"- [X\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X_RIGHT_SQUARE_BRACKET, "NEWLINE_CONSUME_LIST_PREFIX (\"- [X]\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_LEFT_SQUARE_BRACKET,                                               "NEWLINE_CONSUME_LIST_PREFIX (\"[\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT,                             "NEWLINE_CONSUME_LIST_PREFIX (\"[^\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_ID,                                                       "NEWLINE_CONSUME_LIST_PREFIX (\"[^...\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET,                                  "NEWLINE_CONSUME_LIST_PREFIX (\"[^...]\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET_COLON,                            "NEWLINE_CONSUME_LIST_PREFIX (\"[^...]:\")" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_CHECKED_ITEM,                                            "NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_CHECKED_ITEM" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_UNCHECKED_ITEM,                                          "NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_UNCHECKED_ITEM" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN,                                        "NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_DEFINITION_ITEM,                                          "NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_DEFINITION_ITEM" },
        { TokeniserState::NEWLINE_CONSUME_LIST_PREFIX_ABORT,                                                             "NEWLINE_CONSUME_LIST_PREFIX_ABORT" },

        { TokeniserState::NEWLINE_AFTER_EMPTY_LINE,                                                                      "NEWLINE_AFTER_EMPTY_LINE" },

        { TokeniserState::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK,                                                           "PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK" },
        { TokeniserState::PARAGRAPH_BLOCK_LINE_CONTENT,                                                                  "PARAGRAPH_BLOCK_LINE_CONTENT" },

        { TokeniserState::INLINE_CONTENT_SINGLE_SPACE,                                                                   "INLINE_CONTENT_SINGLE_SPACE" },
        { TokeniserState::INLINE_CONTENT_LINE_BREAK,                                                                     "INLINE_CONTENT_LINE_BREAK" },
        { TokeniserState::INLINE_CONTENT_BANG,                                                                           "INLINE_CONTENT_BANG" },
        { TokeniserState::INLINE_CONTENT_LEFT_SQUARE_BRACKET,                                                            "INLINE_CONTENT_LEFT_SQUARE_BRACKET" },
        { TokeniserState::INLINE_CONTENT_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT,                                          "INLINE_CONTENT_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT" },
        { TokeniserState::INLINE_CONTENT_LEFT_CURLY_BRACKET,                                                             "INLINE_CONTENT_LEFT_CURLY_BRACKET" },
        { TokeniserState::INLINE_CONTENT_LEFT_CURLY_BRACKET_NUMBER_SIGN,                                                 "INLINE_CONTENT_LEFT_CURLY_BRACKET_NUMBER_SIGN" },
        { TokeniserState::INLINE_CONTENT_DOUBLE_LEFT_CURLY_BRACKET,                                                      "INLINE_CONTENT_DOUBLE_LEFT_CURLY_BRACKET" },
        { TokeniserState::INLINE_CONTENT_FORMATTING,                                                                     "INLINE_CONTENT_FORMATTING" },
        { TokeniserState::INLINE_CONTENT_INLINE_CODE_SINGLE_GRAVE_ACCENT,                                                "INLINE_CONTENT_INLINE_CODE_SINGLE_GRAVE_ACCENT" },
        { TokeniserState::INLINE_CONTENT_INLINE_CODE_DOUBLE_GRAVE_ACCENT,                                                "INLINE_CONTENT_INLINE_CODE_DOUBLE_GRAVE_ACCENT" },

        { TokeniserState::ESCAPED_CHARACTER_TO_TOKEN_QUEUE,                                                              "ESCAPED_CHARACTER_TO_TOKEN_QUEUE" },
        { TokeniserState::ESCAPED_CHARACTER_TO_PENDING_CHAR_BUFFER,                                                      "ESCAPED_CHARACTER_TO_PENDING_CHAR_BUFFER" },

        { TokeniserState::HORIZONTAL_RULE_ASTERISK,                                                                      "HORIZONTAL_RULE_ASTERISK" },
        { TokeniserState::HORIZONTAL_RULE_HYPHEN,                                                                        "HORIZONTAL_RULE_HYPHEN" },
        { TokeniserState::HORIZONTAL_RULE_UNDERSCORE,                                                                    "HORIZONTAL_RULE_UNDERSCORE" },
        { TokeniserState::HORIZONTAL_RULE_WHITESPACE,                                                                    "HORIZONTAL_RULE_WHITESPACE" },

        { TokeniserState::IMAGE_ALT_TEXT,                                                                                "IMAGE_ALT_TEXT" },
        { TokeniserState::IMAGE_AFTER_ALT_TEXT,                                                                          "IMAGE_AFTER_ALT_TEXT" },
        { TokeniserState::IMAGE_SOURCE,                                                                                  "IMAGE_SOURCE" },

        { TokeniserState::HYPERLINK_TEXT,                                                                                "HYPERLINK_TEXT" },
        { TokeniserState::HYPERLINK_AFTER_TEXT,                                                                          "HYPERLINK_AFTER_TEXT" },
        { TokeniserState::HYPERLINK_URL,                                                                                 "HYPERLINK_URL" },

        { TokeniserState::HEADING_BLOCK_ATX_DEFINITION,                                                                  "HEADING_BLOCK_ATX_DEFINITION" },
        { TokeniserState::HEADING_BLOCK_ATX_BEFORE_TEXT,                                                                 "HEADING_BLOCK_ATX_BEFORE_TEXT" },
        { TokeniserState::HEADING_BLOCK_ATX_TEXT,                                                                        "HEADING_BLOCK_ATX_TEXT" },
        { TokeniserState::HEADING_BLOCK_ATX_TEXT_CLOSING,                                                                "HEADING_BLOCK_ATX_TEXT_CLOSING" },
        { TokeniserState::HEADING_BLOCK_ATX_TEXT_AFTER_CLOSING,                                                          "HEADING_BLOCK_ATX_TEXT_AFTER_CLOSING" },
        { TokeniserState::HEADING_BLOCK_SETEXT_FMT_EQUAL_SIGN,                                                           "HEADING_BLOCK_SETEXT_FMT_EQUAL_SIGN" },
        { TokeniserState::HEADING_BLOCK_SETEXT_FMT_HYPHEN,                                                               "HEADING_BLOCK_SETEXT_FMT_HYPHEN" },

        { TokeniserState::CODE_BLOCK_DECLARATION,                                                                        "CODE_BLOCK_DECLARATION" },
        { TokeniserState::CODE_BLOCK_BEFORE_LANGUAGE_TAG,                                                                "CODE_BLOCK_BEFORE_LANGUAGE_TAG" },
        { TokeniserState::CODE_BLOCK_INLINED_CONTENT,                                                                    "CODE_BLOCK_INLINED_CONTENT" },
        { TokeniserState::CODE_BLOCK_LANGUAGE_TAG,                                                                       "CODE_BLOCK_LANGUAGE_TAG" },
        { TokeniserState::CODE_BLOCK_WHITESPACE_AFTER_LANGUAGE_TAG,                                                      "CODE_BLOCK_WHITESPACE_AFTER_LANGUAGE_TAG" },
        { TokeniserState::CODE_BLOCK_CONTENT,                                                                            "CODE_BLOCK_CONTENT" },
        { TokeniserState::CODE_BLOCK_CONTENT_NEWLINE,                                                                    "CODE_BLOCK_CONTENT_NEWLINE" },
        { TokeniserState::CODE_BLOCK_CONSUME_WHITESPACE,                                                                 "CODE_BLOCK_CONSUME_WHITESPACE" },
        { TokeniserState::CODE_BLOCK_END,                                                                                "CODE_BLOCK_END" },
        { TokeniserState::CODE_BLOCK_AFTER_END,                                                                          "CODE_BLOCK_AFTER_END" },

        { TokeniserState::BLOCKQUOTE_NEW_BLOCKQUOTE_BLOCK,                                                               "BLOCKQUOTE_NEW_BLOCKQUOTE_BLOCK" },
        { TokeniserState::BLOCKQUOTE_BLOCK_BEGIN,                                                                        "BLOCKQUOTE_BLOCK_BEGIN" },

        { TokeniserState::BLOCK_ID_NAME,                                                                                 "BLOCK_ID_NAME" },
        { TokeniserState::AFTER_BLOCK_ID,                                                                                "AFTER_BLOCK_ID" },

        { TokeniserState::FOOTNOTE_REF_ID,                                                                               "FOOTNOTE_REF_ID" },
        { TokeniserState::AFTER_FOOTNOTE_REF,                                                                            "AFTER_FOOTNOTE_REF" },

        { TokeniserState::TABLE_BEGIN,                                                                                   "TABLE_BEGIN" },
        { TokeniserState::TABLE_COL_HEADING_NEW,                                                                         "TABLE_COL_HEADING_NEW" },
        { TokeniserState::TABLE_COL_HEADING_CONTENT,                                                                     "TABLE_COL_HEADING_CONTENT" },
        { TokeniserState::TABLE_DEFINITION_ROW_BEGIN,                                                                    "TABLE_DEFINITION_ROW_BEGIN" },
        { TokeniserState::TABLE_COL_DEFINITION_NEW,                                                                      "TABLE_COL_DEFINITION_NEW" },
        { TokeniserState::TABLE_COL_DEFINITION,                                                                          "TABLE_COL_DEFINITION" },
        { TokeniserState::TABLE_COL_DEFINITION_END,                                                                      "TABLE_COL_DEFINITION_END" },
        { TokeniserState::TABLE_DEFINITION_ROW_END,                                                                      "TABLE_DEFINITION_ROW_END" },
        { TokeniserState::TABLE_DATA_ROW_BEGIN,                                                                          "TABLE_DATA_ROW_BEGIN" },
        { TokeniserState::TABLE_DATA_CELL_NEW,                                                                           "TABLE_DATA_CELL_NEW" },
        { TokeniserState::TABLE_DATA_CELL_CONTENT,                                                                       "TABLE_DATA_CELL_CONTENT" },
        { TokeniserState::TABLE_DATA_ROW_END,                                                                            "TABLE_DATA_ROW_END" },
        { TokeniserState::TABLE_ABORT,                                                                                   "TABLE_ABORT" },

        { TokeniserState::HTML_BLOCK_BEGIN,                                                                              "HTML_BLOCK_BEGIN" },
        { TokeniserState::HTML_BLOCK_BEGIN_END_TAG_OPEN,                                                                 "HTML_BLOCK_BEGIN_END_TAG_OPEN" },
        { TokeniserState::HTML_BLOCK_BEGIN_MARKUP_DECLARATION,                                                           "HTML_BLOCK_BEGIN_MARKUP_DECLARATION" },
        { TokeniserState::HTML_BLOCK_BEGIN_TAG_NAME,                                                                     "HTML_BLOCK_BEGIN_TAG_NAME" },
        { TokeniserState::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_NAME,                                                        "HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_NAME" },
        { TokeniserState::HTML_BLOCK_BEGIN_ATTRIBUTE_NAME,                                                               "HTML_BLOCK_BEGIN_ATTRIBUTE_NAME" },
        { TokeniserState::HTML_BLOCK_BEGIN_AFTER_ATTRIBUTE_NAME,                                                         "HTML_BLOCK_BEGIN_AFTER_ATTRIBUTE_NAME" },
        { TokeniserState::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_VALUE,                                                       "HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_VALUE" },
        { TokeniserState::HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_DOUBLE_QUOTED,                                                "HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_DOUBLE_QUOTED" },
        { TokeniserState::HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_SINGLE_QUOTED,                                                "HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_SINGLE_QUOTED" },
        { TokeniserState::HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_UNQUOTED,                                                     "HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_UNQUOTED" },
        { TokeniserState::HTML_BLOCK_BEGIN_AFTER_ATTRIBUTE_VALUE_QUOTED,                                                 "HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_SINGLE_QUOTED" },
        { TokeniserState::HTML_BLOCK_BEGIN_SELF_CLOSING_START_TAG,                                                       "HTML_BLOCK_BEGIN_SELF_CLOSING_START_TAG" },
        { TokeniserState::HTML_BLOCK_BEGIN_TAG_RESOLVE,                                                                  "HTML_BLOCK_BEGIN_TAG_RESOLVE" },
        { TokeniserState::HTML_BLOCK_BEGIN_CONCLUSION,                                                                   "HTML_BLOCK_BEGIN_CONCLUSION" },
        { TokeniserState::HTML_BLOCK_CONTENT,                                                                            "HTML_BLOCK_CONTENT" },
        { TokeniserState::HTML_BLOCK_CONTENT_NEWLINE,                                                                    "HTML_BLOCK_CONTENT_NEWLINE" },
        { TokeniserState::HTML_BLOCK_CONTENT_TAG_OPEN,                                                                   "HTML_BLOCK_CONTENT_TAG_OPEN" },
        { TokeniserState::HTML_BLOCK_END_END_TAG_OPEN,                                                                   "HTML_BLOCK_END_END_TAG_OPEN" },
        { TokeniserState::HTML_BLOCK_END_TAG_NAME,                                                                       "HTML_BLOCK_END_TAG_NAME" },
        { TokeniserState::HTML_BLOCK_END_TAG_AFTER_TAG_NAME,                                                             "HTML_BLOCK_END_TAG_AFTER_TAG_NAME" },

        { TokeniserState::END_OF_FILE,                                                                                   "END_OF_FILE" },
    } );

    if( map.contains( state ) ) {
        os << map.at( state );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::markdown::TokeniserState enum to a string representation
 * @param attr TokeniserState enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::markdown::TokeniserState state ) {
    std::stringstream ss;
    ss << state;
    return ss.str();
}
