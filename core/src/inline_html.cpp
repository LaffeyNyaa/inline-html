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

#include "inline_html/inline_html.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

namespace inline_html {
using regex_match_vector = std::vector<std::smatch>;

static const std::string STYLE_PATTERN =
    R"(<link[^>]*rel=["']stylesheet["'][^>]*href=["']([^"']*)["'][^>]*>)";
static const std::string SCRIPT_PATTERN =
    R"(<script[^>]*src=["']([^"']*)["'][^>]*></script>)";
static const std::string STYLE_TAG = "style";
static const std::string SCRIPT_TAG = "script";

static std::string get_directory(const std::string_view file_path) noexcept {
    const auto position = file_path.find_last_of("/\\");

    if (position == std::string::npos) {
        return "";
    }

    return std::string(file_path.data(), file_path.data() + position + 1);
}

/**
 * @throws std::ios_base::failure If there's an error reading the HTML file or
 *         any of the referenced CSS/JS files.
 */
static std::string read_file(const std::string_view file_path) {
    std::ifstream file(file_path.data());
    file.exceptions(std::ios::failbit | std::ios::badbit);

    return std::string(std::istreambuf_iterator<char>(file), {});
}

#ifdef _WIN32
/**
 * @throws std::system_error If a Windows API error occurs while loading
 *         resources.
 */
static std::string read_resource(const int id, LPCSTR type) {
    const auto module = GetModuleHandle(nullptr);

    if (module == nullptr) {
        std::error_code ec(GetLastError(), std::system_category());
        throw std::system_error(ec);
    }

    const auto handle = FindResource(module, MAKEINTRESOURCE(id), type);

    if (handle == nullptr) {
        std::error_code ec(GetLastError(), std::system_category());
        throw std::system_error(ec);
    }

    const auto loaded = LoadResource(module, handle);

    if (loaded == nullptr) {
        std::error_code ec(GetLastError(), std::system_category());
        throw std::system_error(ec);
    }

    const auto locked = LockResource(loaded);

    if (locked == nullptr) {
        std::error_code ec(GetLastError(), std::system_category());
        throw std::system_error(ec);
    }

    const auto size = SizeofResource(module, handle);
    return std::string(static_cast<LPSTR>(locked), size);
}
#endif  // _WIN32

static regex_match_vector get_regex_matches(
    const std::string &data, const std::string_view pattern) noexcept {
    const std::regex regex(pattern.data(), std::regex_constants::icase);

    return std::vector<std::smatch>(
        std::sregex_iterator(data.begin(), data.end(), regex), {});
}

/**
 * @throws std::ios_base::failure If there's an error reading the HTML file or
 *         any of the referenced CSS/JS files.
 */
static std::string inline_static_files(std::string data,
                                       const regex_match_vector &smatches,
                                       const std::string_view directory,
                                       const std::string_view wrapper_tag) {
    for (auto match_iterator = smatches.rbegin();
         match_iterator != smatches.rend(); ++match_iterator) {
        const auto position = match_iterator->position();
        const auto filename = (*match_iterator)[1].str();
        const auto len = (*match_iterator)[0].str().size();
        const auto path = directory.data() + filename;

        auto content = read_file(path);
        content = std::string("<") + wrapper_tag.data() + ">" + content + "</" +
                  wrapper_tag.data() + ">";
        data.replace(position, len, content);
    }

    return data;
}

/**
 * @throws std::out_of_range If a referenced filename is not found in the
 *         provided resource map.
 * @throws std::system_error If a Windows API error occurs while loading
 *         resources.
 */
static std::string inline_static_resources(std::string data,
                                           const regex_match_vector &smatches,
                                           const resource_map &resource_map,
                                           const std::string_view wrapper_tag) {
    const auto reverse_begin = smatches.rbegin();
    const auto reverse_end = smatches.rend();

    for (auto match = reverse_begin; match != reverse_end; ++match) {
        const auto position = match->position();
        const auto filename = (*match)[1].str();
        const auto length = (*match)[0].str().size();
        const auto res_id = resource_map.at(filename);
        auto content = read_resource(res_id, RT_RCDATA);
        content = std::string("<") + wrapper_tag.data() + ">" + content + "</" +
                  wrapper_tag.data() + ">";
        data.replace(position, length, content);
    }

    return data;
}

static std::string remove_all_cr(std::string data) noexcept {
    data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());
    return data;
}

std::string inline_html(const std::string_view file_path) {
    auto directory = get_directory(file_path);
    auto data = read_file(file_path);

    const auto style_smatches = get_regex_matches(data, STYLE_PATTERN);
    data = inline_static_files(data, style_smatches, directory, STYLE_TAG);

    const auto script_smatches = get_regex_matches(data, SCRIPT_PATTERN);
    data = inline_static_files(data, script_smatches, directory, SCRIPT_TAG);

    return remove_all_cr(data);
}

#ifdef _WIN32
std::string inline_html(const int id, const resource_map &resource_map) {
    auto data = read_resource(id, RT_HTML);

    const auto style_smatches = get_regex_matches(data, STYLE_PATTERN);
    data =
        inline_static_resources(data, style_smatches, resource_map, STYLE_TAG);

    const auto script_smatches = get_regex_matches(data, SCRIPT_PATTERN);
    data = inline_static_resources(data, script_smatches, resource_map,
                                   SCRIPT_TAG);

    return remove_all_cr(data);
}
#endif  // _WIN32
}  // namespace inline_html
