#ifndef BLOGATOR_SITEMAP_H
#define BLOGATOR_SITEMAP_H

#include <list>
#include <filesystem>
#include <unordered_map>

namespace blogator::sitemap {
    class Sitemap {
      public:
        typedef size_t Key_t;

        struct SiteNode {
            Key_t                 id;
            std::filesystem::path path;

            bool isLeaf() const { return path.has_filename(); }
        };
        struct NodeAdjacency {
            std::list<Key_t> children;
            Key_t            parent;
        };

        typedef std::unordered_map<SiteNode, NodeAdjacency> SiteMap_t;

        Sitemap();
        Sitemap( const Sitemap &sitemap );
        Sitemap( Sitemap &&sitemap ) noexcept;
        virtual ~Sitemap() = default;

        SiteMap_t::const_iterator find( const Key_t &node ) const;
        const NodeAdjacency & at( const Key_t &node ) const;
        bool createDirectedEdge( const SiteNode &from, const SiteNode &to );
        //Graph state
        bool isReachable( const T &from, const T &to ) const;
        bool nodeExists( const T &node ) const;
        bool isEmpty() const;
        bool hasParent( const Key_t)
        size_t getInDegree( const T &node ) const;
        size_t getOutDegree( const T &node ) const;

        std::ostream & printAdjacencyList( std::ostream &out ) const;
      protected:
        bool checkNodesExist( const T& from, const T &to ) const;
        SiteMap_t   _sitemap;
    };
}

#endif //BLOGATOR_SITEMAP_H
