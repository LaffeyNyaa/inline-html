# Usage
```
include(FetchContent)

FetchContent_Declare(
  html-static-embedder
  GIT_REPOSITORY https://github.com/LaffeyNyaa/html-static-embedder.git
  GIT_TAG main)
FetchContent_MakeAvailable(html-static-embedder)
```

# Examples
## Load from files
```
#include <inline_htmlder.h>
#include <iostream>

int main() {
    hse::inline_htmlder embedder;
    embedder.load_html_from_file("res/index.html");
    embedder.embed_static_from_files();

    auto html_data = embedder.html_data();
    std::cout << *html_data << '\n';

    return 0;
}
```
## Load from the .rc file
```
#include "resource.h"
#include <inline_htmlder.h>
#include <iostream>
#include <map>

int main() {
    std::map<std::string, int> res_map = {
        {"index.html", IDR_HTML_INDEX},
        {"style.css", IDR_CSS_STYLE},
        {"script.js", IDR_JS_SCRIPT},
    };

    hse::inline_htmlder embedder;
    embedder.load_html_from_res(IDR_HTML_INDEX);
    embedder.embed_static_from_res(res_map);

    auto html_data = embedder.html_data();
    std::cout << *html_data << '\n';

    return 0;
}
```