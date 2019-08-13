#ifndef BLOGATOR_HTML_EDITOR_CODE_H
#define BLOGATOR_HTML_EDITOR_CODE_H

namespace blogator::html::editor::code { //TODO syntax highlighter
    //Need to lift any snippet (between tags like <code class="code-snippet"></code> - let's assume non-html for the moment)
    //All lifted (cached) snippets need to be cached along with their article ref and insertion point/range in the source post
    //on output, any cached code need to be processed by the highlighter (essentially putting formatting tags around keywords) and written out as a block
    //formatted block can then be stored in system's dir as files that can then be opened when a snippet point is reached and copied verbatim
    //don't forget to delete cached file after

    //regex replace: https://www.geeksforgeeks.org/regex_replace-in-cpp-replace-the-match-of-a-string-using-regex_replace/
    //               https://en.cppreference.com/w/cpp/regex/regex_replace
    //idea: https://www.freeformatter.com/html-escape.html
    //how-to: https://www.taniarascia.com/adding-syntax-highlighting-to-code-snippets/
    //        https://idiallo.com/blog/javascript-syntax-highlighter
}

#endif //BLOGATOR_HTML_EDITOR_CODE_H
