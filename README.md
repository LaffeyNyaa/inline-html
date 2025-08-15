# Using html-static-embedder
```include(FetchContent)
FetchContent_Declare(
  html-static-embedder
  GIT_REPOSITORY https://github.com/LaffeyNyaa/html-static-embedder.git
  GIT_TAG main)
FetchContent_MakeAvailable(html-static-embedder)
```

# Examples
## Load from files
`#include <html_static_embedder.h>

int main() {
    hse::HTMLStaticEmbedder embedder;
    embedder.load_html_from_file("res/index.html");
    embedder.embed_static_from_files();
    auto html_data = embedder.get_html_data();
}`
## Load according to the .rc file
`#include <html_static_embedder.h>
#include <map>
#include "resource.h"

int main() {
    std::map<std::string, int> res_map = {
        {"index.html", IDR_HTML_INDEX},
        {"style.css", IDR_CSS_STYLE},
        {"script.js", IDR_JS_SCRIPT},
    };

    hse::HTMLStaticEmbedder embedder;
    embedder.load_html_from_res(IDR_HTML_INDEX);
    embedder.embed_static_from_res(res_map);
    auto html_data = embedder.get_html_data();
}`