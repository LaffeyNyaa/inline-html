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

#include "html_static_embedder.h"

#include <fstream>
#include <iostream>
#include <vector>

void hse::HTMLStaticEmbedder::load_html_from_file(
    const std::string &path) noexcept {
    auto result = load_file(path);
    if (!result.has_value()) {
        std::cerr << "Error loading file " << path << '\n';
        return;
    }

    get_path_prefix(path);
    html_data = std::move(*result);

    remove_all_cr();
}

#ifdef WIN32
void hse::HTMLStaticEmbedder::load_html_from_res(int id) noexcept {
    auto result = load_res(id, RT_HTML);
    if (!result.has_value()) {
        std::cerr << "Error loading resource " << id << '\n';
        return;
    }

    html_data = std::move(*result);
    remove_all_cr();
}
#endif  // WIN32

void hse::HTMLStaticEmbedder::embed_static_from_files() noexcept {
    std::string css_pattern =
        R"(<link[^>]*rel=["']stylesheet["'][^>]*href=["']([^"']*)["'][^>]*>)";
    auto css_matches_result = read_matches(css_pattern);

    if (css_matches_result.has_value()) {
        embed_css_files_with_matches(*css_matches_result);
    }

    std::string js_pattern = R"(<script[^>]*src=["']([^"']*)["'][^>]*></script>)";
    auto js_matches_result = read_matches(js_pattern);

    if (js_matches_result.has_value()) {
        embed_js_files_with_matches(*js_matches_result);
    }

    remove_all_cr();
}

#ifdef WIN32
void hse::HTMLStaticEmbedder::embed_static_from_res() noexcept {}
#endif  // WIN32
std::optional<std::string> hse::HTMLStaticEmbedder::load_file(
    const std::string &path) noexcept {
    std::ifstream file(path);

    if (!file.is_open()) {
        return std::nullopt;
    }

    auto data = std::string(std::istreambuf_iterator<char>(file),
                            std::istreambuf_iterator<char>());

    return data;
}

#ifdef WIN32
std::optional<std::string> hse::HTMLStaticEmbedder::load_res(
    int id, LPCSTR type) noexcept {
    HMODULE module = GetModuleHandle(nullptr);
    LPSTR int_res = MAKEINTRESOURCE(id);
    HRSRC handle = FindResource(module, int_res, type);
    HGLOBAL loaded = LoadResource(module, handle);
    LPVOID data = LockResource(loaded);
    DWORD size = SizeofResource(module, handle);

    if (GetLastError() != ERROR_SUCCESS) {
        return std::nullopt;
    }

    auto res_data_p = static_cast<const char *>(data);
    auto res_data = std::string(res_data_p, size);

    return res_data;
}
#endif  // WIN32

void hse::HTMLStaticEmbedder::get_path_prefix(
    const std::string &path) noexcept {
    auto pos = path.find_last_of('/');

    if (pos == std::string::npos) {
        pos = path.find_last_of('\\');
    }

    if (pos == std::string::npos) {
        path_prefix = "";
        return;
    }

    path_prefix = path.substr(0, pos + 1);
}

void hse::HTMLStaticEmbedder::remove_all_cr() noexcept {
    auto iter = std::remove(html_data.begin(), html_data.end(), '\r');
    html_data.erase(iter, html_data.end());
}

std::optional<std::vector<std::smatch>> hse::HTMLStaticEmbedder::read_matches(
    const std::string &pattern) noexcept {
    std::vector<std::smatch> matches;
    std::regex regex(pattern, std::regex_constants::icase);
    std::sregex_iterator begin(html_data.begin(), html_data.end(), regex);
    std::sregex_iterator end;

    for (auto iter = begin; iter != end; ++iter) {
        matches.emplace_back(*iter);
    }

    if (matches.empty()) {
        return std::nullopt;
    }

    return std::move(matches);
}

void hse::HTMLStaticEmbedder::embed_css_files_with_matches(
    const std::vector<std::smatch> &matches) noexcept {
    auto rbegin = matches.rbegin();
    auto rend = matches.rend();

    for (auto iter = rbegin; iter != rend; ++iter) {
        auto pos = iter->position();
        auto filename = (*iter)[1].str();
        auto full = (*iter)[0].str();
        auto path = path_prefix + filename;
        auto data_result = load_file(path);

        if (!data_result.has_value()) {
            std::cerr << "Error loading file " << path << '\n';
            continue;
        }

        auto data = "<style>" + *data_result + "</style>";

        auto len = full.size();
        html_data.replace(pos, len, data);
    }
}

void hse::HTMLStaticEmbedder::embed_js_files_with_matches(
    const std::vector<std::smatch> &matches) noexcept {
    auto rbegin = matches.rbegin();
    auto rend = matches.rend();

    for (auto iter = rbegin; iter != rend; ++iter) {
        auto pos = iter->position();
        auto filename = (*iter)[1].str();
        auto full = (*iter)[0].str();
        auto path = path_prefix + filename;
        auto data_result = load_file(path);

        if (!data_result.has_value()) {
            std::cerr << "Error loading file " << path << '\n';
            continue;
        }

        auto data = "<script>" + *data_result + "</script>";

        auto len = full.size();
        html_data.replace(pos, len, data);
    }
}