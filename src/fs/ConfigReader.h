#ifndef BLOGATOR_CONFIGREADER_H
#define BLOGATOR_CONFIGREADER_H

#include <regex>
#include <string>
#include "../dto/Options.h"
#include "../cli/MsgInterface.h"

namespace blogator::fs {
    class ConfigReader {
      public:
        [[nodiscard]] std::shared_ptr<blogator::dto::Options> init( const std::filesystem::path &file_path ) const;

        static void generateBlankConfigFile( const std::filesystem::path &file_path );

      private:
        enum class Type { BOOLEAN, INTEGER, STRING, STRING_ARRAY };

        struct Value {
            Value( size_t l, Type t, std::string v ) :
                line( l ), type( t ), value( std::move( v ) )
            {};

            size_t      line ;
            bool        validated { false };
            Type        type;
            std::string value;
        };

        cli::MsgInterface &_display = cli::MsgInterface::getInstance();

        const std::regex COMMENT_RX       = std::regex( R"(^(?:\s*//.*?|\s*)$)" );
        const std::regex KV_BOOL_VAL_RX   = std::regex( R"(\s*([a-zA-Z-]+)\s*=\s*(true|false)\s*;)" );
        const std::regex KV_STR_VAL_RX    = std::regex( "^\\s*([a-zA-Z-]+)\\s*=\\s*\"(.*)\"\\s*;" );
        const std::regex KV_INT_VAL_RX    = std::regex( R"(\s*([a-zA-Z-]+)\s*=\s*(\d+)\s*;)" );
        const std::regex KV_STR_ARR_RX    = std::regex( R"(\s*([a-zA-Z-]+)\s*=\s*\[(.*)\]\s*;)" );
        const std::regex QUOTED_STR_RX    = std::regex( R"((?:(?:")([^"]*)(?:"(?:\s*))))" );
        const std::regex HTML_FILENAME_RX = std::regex( R"(^.+\.(?:htm|html)$)" );
        const std::regex JSON_FILENAME_RX = std::regex( R"(^.+\.(?:json)$)" );
        const std::regex WEB_URL_RX       = std::regex( R"((https:[/][/]|http:[/][/]|www.)[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(:[a-zA-Z0-9]*)?/?([a-zA-Z0-9\-\._?\,'/\+&amp;%\$#\=~])*$)" );
        const std::regex CSS_DIMENSION_RX = std::regex( R"((\d+\.{0,1}\d*\s*(?:cm|mm|in|px|pt|pc|em|ex|ch|rem|vw|vh|vmin|vmax|%)))" );
        const std::regex IMG_FILENAME     = std::regex( R"(([a-zA-Z-_\d]+\.(?:jpg|jpeg|jpe|jif|jfif|jfi|gif|png|apng|svg|svgz|bmp|dib|webp|ico)))" );
        const std::regex FILE_PATH        = std::regex( R"(^[^/]{1}[a-zA-Z\-_\d\/]+\.[a-zA-Z\d_]{2,4}$)" );

        void loadConfigurationFile( const std::filesystem::path &path, std::unordered_map<std::string, Value> &map ) const;
        void processTemplateOptions( std::unordered_map<std::string, Value> &map, dto::Options &options ) const;
        void processPostsOptions( std::unordered_map<std::string, Value> &map, dto::Options &options ) const;
        void processMonthsOptions( std::unordered_map<std::string, Value> &map, dto::Options &options ) const;
        void processIndexOptions( std::unordered_map<std::string, Value> &map, dto::Options &options ) const;
        void processLandingPageOptions( std::unordered_map<std::string, Value> &map, dto::Options &options ) const;
        void processPageNavOptions( std::unordered_map<std::string, Value> &map, dto::Options &options ) const;
        void processBreadcrumbOptions( std::unordered_map<std::string, Value> &map, dto::Options &options ) const;
        void processRssOptions( std::unordered_map<std::string, Value> &map, dto::Options &options ) const;
        void processJsonOptions( std::unordered_map<std::string, Value> &map, dto::Options &options ) const;
    };
}

#endif //BLOGATOR_CONFIGREADER_H