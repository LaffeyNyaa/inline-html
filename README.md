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
## Load from HTML file
```
#include <inline_html.h>

#include <iostream>

int main() {
    std::string html_data;

    try {
        html_data = inline_html::inline_html("res/index.html");
    } catch (std::ios_base::failure &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << html_data << '\n';

    return 0;
}
```
## Load from .rc file
```
#include <inline_html.h>

#include <iostream>
#include <map>

#include "resource.h"

int main() {
    inline_html::resource_map resource_map = {
        {"index.html", IDR_HTML_INDEX},
        {"style.css", IDR_CSS_STYLE},
        {"script.js", IDR_JS_SCRIPT},
    };

    std::string html_data;

    try {
        html_data = inline_html::inline_html(IDR_HTML_INDEX, resource_map);
    } catch (const std::system_error &e) {
        std::cerr << e.what() << '\n';
        return 1;
    } catch (const std::out_of_range &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << html_data << '\n';

    return 0;
}
```