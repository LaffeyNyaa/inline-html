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
#include <regex>
#include <fstream>
#include <vector>

using smatch_vec = std::vector<std::smatch>;
using inline_html::res_map;

static std::string get_dir(const std::string &path) noexcept {
    auto pos = path.find_last_of('/');

    if (pos == std::string::npos) {
        pos = path.find_last_of('\\');
    }

    if (pos == std::string::npos) {
        return "";
    }

    auto dir = path.substr(0, pos + 1);
    return dir;
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

    std::string data(begin, end);
    return data;
}

/*
 * @throws std::system_error If a Windows API error occurs while loading
 *         resources.
 */
static std::string read_res(std::int32_t id, LPCSTR type) {
    auto module = GetModuleHandle(nullptr);
    auto int_res = MAKEINTRESOURCE(id);
    auto handle = FindResource(module, int_res, type);
    auto loaded = LoadResource(module, handle);
    auto locked_data = LockResource(loaded);
    auto data = static_cast<LPSTR>(locked_data);

    auto err = GetLastError();

    if (err != ERROR_SUCCESS) {
        std::error_code ec(err, std::system_category());
        throw std::system_error(ec, "Windows API error");
    }

    return data;
}

static smatch_vec get_smatches(const std::string &data,
                               const std::string &pattern) {
    std::vector<std::smatch> smatches;
    std::regex regex(pattern, std::regex_constants::icase);
    std::sregex_iterator begin(data.begin(), data.end(), regex);
    std::sregex_iterator end;

    for (auto iter = begin; iter != end; ++iter) {
        smatches.emplace_back(*iter);
    }

    return smatches;
}

static std::string inline_style_files(const smatch_vec &smatches,
                                      const std::string &dir,
                                      const std::string &data) {
    std::string current_data = data;
    auto rbegin = smatches.rbegin();
    auto rend = smatches.rend();

    for (auto iter = rbegin; iter != rend; ++iter) {
        auto pos = iter->position();
        auto filename = (*iter)[1].str();
        auto len = (*iter)[0].str().size();
        auto path = dir + filename;
        auto content = read_file(path);
        content = "<style>" + content + "</style>";
        current_data.replace(pos, len, content);
    }

    return current_data;
}

static std::string inline_script_files(const smatch_vec &smatches,
                                       const std::string &dir,
                                       const std::string &data) {
    std::string current_data = data;
    auto rbegin = smatches.rbegin();
    auto rend = smatches.rend();

    for (auto iter = rbegin; iter != rend; ++iter) {
        auto pos = iter->position();
        auto filename = (*iter)[1].str();
        auto len = (*iter)[0].str().size();
        auto path = dir + filename;
        auto content = read_file(path);
        content = "<script>" + content + "</script>";
        current_data.replace(pos, len, content);
    }

    return current_data;
}

/**
 * @throws std::out_of_range If a referenced filename is not found in the
 *         provided resource map.
 */
static std::string inline_style_res(const smatch_vec &smatches,
                                    const std::string &data,
                                    const res_map &res_map) {
    std::string current_data = data;
    auto rbegin = smatches.rbegin();
    auto rend = smatches.rend();

    for (auto iter = rbegin; iter != rend; ++iter) {
        auto pos = iter->position();
        auto filename = (*iter)[1].str();
        auto len = (*iter)[0].str().size();
        auto res_id = res_map.at(filename);
        auto content = read_res(res_id, RT_RCDATA);
        content = "<style>" + content + "</style>";
        current_data.replace(pos, len, content);
    }

    return current_data;
}

/**
 * @throws std::out_of_range If a referenced filename is not found in the
 *         provided resource map.
 */
static std::string inline_script_res(const smatch_vec &smatches,
                                     const std::string &data,
                                     const res_map &res_map) {
    std::string current_data = data;
    auto rbegin = smatches.rbegin();
    auto rend = smatches.rend();

    for (auto iter = rbegin; iter != rend; ++iter) {
        auto pos = iter->position();
        auto filename = (*iter)[1].str();
        auto len = (*iter)[0].str().size();
        auto res_id = res_map.at(filename);
        auto content = read_res(res_id, RT_RCDATA);
        content = "<script>" + content + "</script>";
        current_data.replace(pos, len, content);
    }

    return current_data;
}

static std::string remove_all_cr(std::string data) {
    auto iter = std::remove(data.begin(), data.end(), '\r');
    data.erase(iter, data.end());
    return data;
}

std::string inline_html::inline_html(const std::string &path) {
    auto dir = get_dir(path);
    auto data = read_file(path);

    std::string style_pattern =
        R"(<link[^>]*rel=["']stylesheet["'][^>]*href=["']([^"']*)["'][^>]*>)";
    auto style_smatches = get_smatches(data, style_pattern);
    data = inline_style_files(style_smatches, dir, data);

    std::string script_pattern =
        R"(<script[^>]*src=["']([^"']*)["'][^>]*></script>)";
    auto script_smatches = get_smatches(data, script_pattern);
    data = inline_script_files(script_smatches, dir, data);

    data = remove_all_cr(data);
    return data;
}

#ifdef WIN32
std::string inline_html::inline_html(std::int32_t id, const res_map &res_map) {
    auto data = read_res(id, RT_HTML);

    std::string style_pattern =
        R"(<link[^>]*rel=["']stylesheet["'][^>]*href=["']([^"']*)["'][^>]*>)";
    auto style_smatches = get_smatches(data, style_pattern);
    data = inline_style_res(style_smatches, data, res_map);

    std::string script_pattern =
        R"(<script[^>]*src=["']([^"']*)["'][^>]*></script>)";
    auto script_smatches = get_smatches(data, script_pattern);
    data = inline_script_res(script_smatches, data, res_map);

    data = remove_all_cr(data);
    return data;
}
#endif  // WIN32