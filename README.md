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
    try {
        const auto html_data = inline_html::inline_html("res/index.html");
        std::cout << html_data << '\n';
    } catch (const std::ios_base::failure &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
```
## Load from .rc file
```
int main() {
    try {
        const auto html_data = inline_html::inline_html(IDR_HTML_INDEX, RESOURCE_MAP);
        std::cout << html_data << '\n';
    } catch (const std::system_error &e) {
        std::cerr << e.code().message() << '\n';
        return 1;
    } catch (const std::out_of_range &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
```