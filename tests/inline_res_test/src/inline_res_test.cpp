/*
 * MIT License
 *
 * Copyright (c) 2025 LaffeyNyaa
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <inline_html/exception.h>
#include <inline_html/inline_html.h>

#include <iostream>
#include <map>

#include "resource.h"

static const std::string TEST_SAMPLE = R"delimiter(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>button {
    border-radius: 8px; /* Adjust this value to control the roundness of corners */
    background-color: aqua;
    color: white;
}
</style>
    <script>// For demo

function showAlert() {
    alert("You cliked me!");
}
</script>
    <title>Document</title>
</head>
<body>
    <button onclick="showAlert()">Click Me!</button>
</body>
</html>
)delimiter";

static const inline_html::res_map RESOURCE_MAP = {
    {"index.html", IDR_HTML_INDEX},
    {"style.css", IDR_CSS_STYLE},
    {"script.js", IDR_JS_SCRIPT},
};

int main() {
    try {
        const auto html_data =
            inline_html::inline_html(IDR_HTML_INDEX, RESOURCE_MAP);

        if (html_data != TEST_SAMPLE) {
            return 1;
        }
    } catch (const inline_html::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
