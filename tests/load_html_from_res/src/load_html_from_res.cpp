#include <html_static_embedder.h>
#include <iostream>

#include "resource.h"

std::string target_data =
    "<!DOCTYPE html>\r\n"
    "<html lang=\"en\">\r\n"
    "<head>\r\n"
    "    <meta charset=\"UTF-8\">\r\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
    "    <link rel=\"stylesheet\" href=\"style.css\">\r\n"
    "    <script src=\"script.js\"></script>\r\n"
    "    <title>Document</title>\r\n"
    "</head>\r\n"
    "<body>\r\n"
    "    <button onclick=\"showAlert()\">Click Me!</button>\r\n"
    "</body>\r\n"
    "</html>\r\n";

int main() {
    hse::HTMLStaticEmbedder embedder;
    embedder.load_html_from_res(IDR_HTML_INDEX);
    std::string raw_html_data = embedder.get_raw_html_data();

    if (raw_html_data != target_data) {
        return 1;
    }
    
    return 0;
}