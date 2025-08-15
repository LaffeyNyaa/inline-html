#include <html_static_embedder.h>

#include "resource.h"

const std::string target_data = R"html(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
    <script src="script.js"></script>
    <title>Document</title>
</head>
<body>
    <button onclick="showAlert()">Click Me!</button>
</body>
</html>
)html";
int main() {
    hse::HTMLStaticEmbedder embedder;
    embedder.load_html_from_res(IDR_HTML_INDEX);
    std::string raw_html_data = embedder.get_raw_html_data();

    if (raw_html_data != target_data) {
        return 1;
    }

    return 0;
}