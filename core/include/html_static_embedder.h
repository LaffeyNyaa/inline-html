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

#ifndef HSE_HTML_STATIC_EMBEDDER_H
#define HSE_HTML_STATIC_EMBEDDER_H

#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <vector>

#ifdef WIN32
#include <Windows.h>
#endif  // WIN32

namespace hse {
class html_static_embedder {
   public:
    html_static_embedder() = default;

    // Load html data from file.
    void load_html_from_file(const std::string &path);

#ifdef WIN32
    // Load html data from the .rc file.
    void load_html_from_res(int id);
#endif  // WIN32

    // Embed static files by filenames in the html file.
    void embed_static_from_files();

#ifdef WIN32
    // Embed static files by filenames in the html file and a res map.
    void embed_static_from_res(const std::map<std::string, int> &res_map);
#endif  // WIN32

    // Get html data const reference.
    const std::string &html_data() const {
        if (!html_data_.has_value()) {
            std::cerr << "[Error] HTML data is not loaded" << '\n';
            std::exit(1);
        }

        return *html_data_;
    }

   private:
    std::optional<std::string> path_prefix_;
    std::optional<std::string> html_data_;
    std::optional<std::map<std::string, int>> res_map_;

    std::optional<std::string> load_file(const std::string &path);

#ifdef WIN32
    std::optional<std::string> load_res(int id, LPCSTR type);
#endif  // WIN32

    void get_path_prefix(const std::string &path);

    void remove_all_cr();
    std::optional<std::vector<std::smatch>> read_matches(
        const std::string &pattern);

    void embed_css_files_with_matches(const std::vector<std::smatch> &matches);
    void embed_js_files_with_matches(const std::vector<std::smatch> &matches);
    void embed_css_res_with_matches(const std::vector<std::smatch> &matches);
    void embed_js_res_with_matches(const std::vector<std::smatch> &matches);
};
}  // namespace hse

#endif  // HSE_HTML_STATIC_EMBEDDER_H