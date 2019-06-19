#include "Generator.h"

#include <utility>
#include <iostream>
#include <fstream>

#include "../fs/fs.h"
#include "../html/html.h"

/**
 * Constructor
 * @param global_options Global blogator options
 */
blogator::Generator::Generator( blogator::dto::Options global_options ) :
    _options( std::move( global_options ) )
{}

/**
 *
 * @param index
 * @return
 */
bool blogator::Generator::init( const blogator::dto::Index &index ) {
    _header = fs::importHTML( index._paths.header );
    _footer = fs::importHTML( index._paths.footer );

    const auto header_insert_index = html::findLineOfTag( "</head>", *_header );

    for( const auto &article : index._file_index ) {
        const auto html_out = ( _options._posts_dir / article._html_filepath.filename() );
        const auto css_rel  = ( article._css_filepath.parent_path().lexically_relative( html_out.parent_path() ) / article._css_filepath.filename() );

        auto article_in  = std::ifstream( article._html_filepath );
        auto article_out = std::ofstream( html_out );

        if( article_out.is_open() ) {
            //HEADER
            size_t counter = 0;
            for( const auto & line : _header->_lines ) {
                if( !article._css_filepath.empty() && counter == header_insert_index ) {
                    auto css_ref = "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"" + css_rel.string() + "\"/>";
                    article_out << css_ref << std::endl;
                }
                article_out << line << std::endl;
                ++counter;
            }

            //CONTENT
            if( article_in.is_open() ) {
                std::string line;

                while( getline( article_in, line ) )
                    article_out << line << std::endl;

                article_in.close();
            }

            //FOOTER
            for( const auto &line : _footer->_lines )
                article_out << line << std::endl;

            article_out.close();
        } else {
            std::cerr << "File '" << html_out << "' could not be opened for writing." << std::endl;
        }
    }
    return false;
}
