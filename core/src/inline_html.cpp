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

#include "inline_html.h"

#include <exception>
#include <fstream>
#include <regex>
#include <vector>

namespace inline_html {
using smatch_vector = std::vector<std::smatch>;

static std::string get_directory(const std::string &path) noexcept {
    auto position = path.find_last_of("/\\");

    if (position == std::string::npos) {
        return "";
    }

    return path.substr(0, position + 1);
}

/**
 * @throws std::ios_base::failure If there's an error reading the HTML file or
 *         any of the referenced CSS/JS files.
 */
static std::string read_file(const std::string &path) {
    std::ifstream file(path);
    file.exceptions(std::ios::failbit | std::ios::badbit);

    std::istreambuf_iterator<char> begin(file);
    std::istreambuf_iterator<char> end;

    return std::string(begin, end);
}

/**
 * @throws std::system_error If a Windows API error occurs while loading
 *         resources.
 */
static std::string read_resource(std::int32_t id, LPCSTR type) {
    auto module = GetModuleHandle(nullptr);

    if (module == nullptr) {
        std::error_code ec(GetLastError(), std::system_category());
        throw std::system_error(ec, "GetModuleHandle failed");
    }

    auto handle = FindResource(module, MAKEINTRESOURCE(id), type);

    if (handle == nullptr) {
        std::error_code ec(GetLastError(), std::system_category());
        throw std::system_error(ec, "FindResource failed");
    }

    auto loaded = LoadResource(module, handle);

    if (loaded == nullptr) {
        std::error_code ec(GetLastError(), std::system_category());
        throw std::system_error(ec, "LoadResource failed");
    }

    auto locked = LockResource(loaded);

    if (locked == nullptr) {
        std::error_code ec(GetLastError(), std::system_category());
        throw std::system_error(ec, "LockResource failed");
    }

    auto size = SizeofResource(module, handle);
    return std::string(static_cast<LPSTR>(locked), size);
}

static smatch_vector get_regex_matches(const std::string &data,
                                       const std::string &pattern) noexcept {
    std::regex regex(pattern, std::regex_constants::icase);
    std::sregex_iterator begin(data.begin(), data.end(), regex);
    std::sregex_iterator end;

    return std::vector<std::smatch>(begin, end);
}

/**
 * @throws std::ios_base::failure If there's an error reading the HTML file or
 *         any of the referenced CSS/JS files.
 */
static std::string inline_static_files(const smatch_vector &smatches,
                                       const std::string &directory,
                                       const std::string &data,
                                       const std::string &wrapper_tag) {
    std::string current_data = data;
    auto reverse_begin = smatches.rbegin();
    auto reverse_end = smatches.rend();

    for (auto iterator = reverse_begin; iterator != reverse_end; ++iterator) {
        auto position = iterator->position();
        auto filename = (*iterator)[1].str();
        auto len = (*iterator)[0].str().size();
        auto path = directory + filename;
        auto content = read_file(path);
        content = '<' + wrapper_tag + '>' + content + "</" + wrapper_tag + '>';
        current_data.replace(position, len, content);
    }

    return current_data;
}

/**
 * @throws std::out_of_range If a referenced filename is not found in the
 *         provided resource map.
 * @throws std::system_error If a Windows API error occurs while loading
 *         resources.
 */
static std::string inline_static_resources(const smatch_vector &smatches,
                                           const std::string &data,
                                           const resource_map &res_map,
                                           const std::string &wrapper_tag) {
    std::string current_data = data;
    auto reverse_begin = smatches.rbegin();
    auto reverse_end = smatches.rend();

    for (auto iterator = reverse_begin; iterator != reverse_end; ++iterator) {
        auto position = iterator->position();
        auto filename = (*iterator)[1].str();
        auto len = (*iterator)[0].str().size();
        auto res_id = res_map.at(filename);
        auto content = read_resource(res_id, RT_RCDATA);
        content = '<' + wrapper_tag + '>' + content + "</" + wrapper_tag + '>';
        current_data.replace(position, len, content);
    }

    return current_data;
}

static std::string remove_all_cr(std::string data) noexcept {
    auto iterator = std::remove(data.begin(), data.end(), '\r');
    data.erase(iterator, data.end());
    return data;
}

std::string inline_html(const std::string &path) {
    auto directory = get_directory(path);
    auto data = read_file(path);

    std::string style_pattern =
        R"(<link[^>]*rel=["']stylesheet["'][^>]*href=["']([^"']*)["'][^>]*>)";
    auto style_smatches = get_regex_matches(data, style_pattern);
    data = inline_static_files(style_smatches, directory, data, "style");

    std::string script_pattern =
        R"(<script[^>]*src=["']([^"']*)["'][^>]*></script>)";
    auto script_smatches = get_regex_matches(data, script_pattern);
    data = inline_static_files(script_smatches, directory, data, "script");

    data = remove_all_cr(data);
    return data;
}

#ifdef WIN32
std::string inline_html(std::int32_t id, const resource_map &res_map) {
    auto data = read_resource(id, RT_HTML);

    std::string style_pattern =
        R"(<link[^>]*rel=["']stylesheet["'][^>]*href=["']([^"']*)["'][^>]*>)";
    auto style_smatches = get_regex_matches(data, style_pattern);
    data = inline_static_resources(style_smatches, data, res_map, "style");

    std::string script_pattern =
        R"(<script[^>]*src=["']([^"']*)["'][^>]*></script>)";
    auto script_smatches = get_regex_matches(data, script_pattern);
    data = inline_static_resources(script_smatches, data, res_map, "script");

    data = remove_all_cr(data);
    return data;
}
#endif  // WIN32
}  // namespace inline_html
