#include <html_static_embedder.h>

#include "resource.h"

std::string target_data =
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, "
    "initial-scale=1.0\">\n"
    "    <link rel=\"stylesheet\" href=\"style.css\">\n"
    "    <script src=\"script.js\"></script>\n"
    "    <title>Document</title>\n"
    "</head>\n"
    "<body>\n"
    "    <button onclick=\"showAlert()\">Click Me!</button>\n"
    "</body>\n"
    "</html>\n";

int main() {
    hse::HTMLStaticEmbedder embedder;
    embedder.load_html_from_file("res/index.html");
    std::string raw_html_data = embedder.get_raw_html_data();

    if (raw_html_data != target_data) {
        return 1;
    }

    return 0;
}