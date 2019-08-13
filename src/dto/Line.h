#ifndef BLOGATOR_LINE_H
#define BLOGATOR_LINE_H

#include <string>

namespace blogator::dto {
    struct Line {
        std::vector<std::string>::const_iterator _it;
        size_t                                   _num;

        friend std::ostream &operator <<( std::ostream &s, const Line &line ) {
            s << line._num << ": \"" << *line._it << "\"";
            return s;
        }

        const Line & operator ++() {
            ++_it; ++_num;
            return *this;
        }

        const Line operator ++( int ) {
            Line temp = *this;
            ++*this;
            return temp;
        }
    };
}

#endif //BLOGATOR_LINE_H
