# Usage
```
include(FetchContent)

FetchContent_Declare(
    inline-html
    GIT_REPOSITORY https://github.com/LaffeyNyaa/inline-html.git
    GIT_TAG main)
FetchContent_MakeAvailable(inline-html)
```

# Examples
## Inline files
```
#include <inline_html/exception.h>
#include <inline_html/inline_html.h>

#include <iostream>

int main() {
    try {
        const auto html_data = inline_html::inline_html("res/index.html");
        std::cout << html_data << "\n";
    } catch (const inline_html::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
```
## Inline resources
```
#include <inline_html/exception.h>
#include <inline_html/inline_html.h>

#include <iostream>
#include <map>

#include "resource.h"

static const inline_html::resource_map RESOURCE_MAP = {
    {"index.html", IDR_HTML_INDEX},
    {"style.css", IDR_CSS_STYLE},
    {"script.js", IDR_JS_SCRIPT},
};

int main() {
    try {
        const auto html_data =
            inline_html::inline_html(IDR_HTML_INDEX, RESOURCE_MAP);
        std::cout << html_data << "\n";
    } catch (const inline_html::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
```
