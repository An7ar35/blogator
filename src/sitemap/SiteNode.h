#include <utility>

#ifndef BLOGATOR_SITENODE_H
#define BLOGATOR_SITENODE_H

#include <iostream>
#include <filesystem>

namespace blogator::sitemap {
    class SiteNode {
      public:
        SiteNode( size_t id, std::filesystem::path path ) :
            _id( id ),
            _path( std::move( path ) )
        {};

        friend std::ostream &operator <<( std::ostream &s, const SiteNode &node ) {
            s << node._id << ": " << node._path.string();
            return s;
        }

        bool operator ==( const SiteNode &rhs ) const { return _id == rhs._id; };
        bool operator !=( const SiteNode &rhs ) const { return !( *this == rhs ); };
        bool operator <( const SiteNode &rhs ) const  { return rhs < *this; };
        bool operator >( const SiteNode &rhs ) const  { return _id > rhs._id; };
        bool operator <=( const SiteNode &rhs ) const { return !( *this > rhs ); };
        bool operator >=( const SiteNode &rhs ) const { return !( *this < rhs ); };

        bool isFile() const { return _path.has_filename(); } //TODO check that shit
        bool isFolder() const { return !_path.has_filename(); } //TODO check that shit

        int compare( const SiteNode &rhs ) {

        }

      private:
        size_t                _id;
        std::filesystem::path _path;
    };
}

#endif //BLOGATOR_SITENODE_H
