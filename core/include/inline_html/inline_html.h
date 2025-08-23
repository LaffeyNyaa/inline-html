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

#pragma once

#include <map>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif  // _WIN32

namespace inline_html {

using resource_map = std::map<std::string, int>;

/**
 * @brief Inlines external CSS and JS files into an HTML document.
 *
 * This function reads an HTML file and replaces all `<link rel="stylesheet">`
 * and `<script src="">` tags with the actual content of the referenced files,
 * embedding them directly into the HTML document as `<style>` and `<script>`
 * blocks respectively.
 *
 * @param file_path The file file_path to the HTML document to process.
 *
 * @return std::string The processed HTML document with CSS and JS inlined.
 *
 * @throws std::ios_base::failure If there's an error reading the HTML file or
 *         any of the referenced CSS/JS files.
 */
std::string inline_html(const std::string &file_path);

#ifdef _WIN32
/**
 * @brief Inlines CSS and JS resources into an HTML resource.
 *
 * This function loads an HTML resource from the Windows executable and replaces
 * all `<link rel="stylesheet">` and `<script src="">` tags with the actual
 * content of the referenced resources, embedding them directly into the HTML
 * document as `<style>` and `<script>` blocks respectively.
 *
 * @param id The resource ID of the HTML document to process.
 * @param resource_map A mapping of resource filenames to their corresponding
 *                resource IDs for CSS and JS files.
 *
 * @return std::string The processed HTML document with CSS and JS inlined.
 *
 * @throws std::system_error If a Windows API error occurs while loading
 *         resources.
 * @throws std::out_of_range If a referenced filename is not found in the
 *         provided resource map.
 */
std::string inline_html(const int id, const resource_map &resource_map);
#endif  // _WIN32
}  // namespace inline_html
