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

#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

#include "inline_html/exception.h"

namespace inline_html {
using smatches = std::vector<std::smatch>;

static const std::string STYLE_PATTERN =
    R"(<link[^>]*rel=["']stylesheet["'][^>]*href=["']([^"']*)["'][^>]*>)";
static const std::string SCRIPT_PATTERN =
    R"(<script[^>]*src=["']([^"']*)["'][^>]*></script>)";
static const std::string STYLE_TAG = "style";
static const std::string SCRIPT_TAG = "script";

static std::string get_dir(const std::string_view path) noexcept {
    const auto pos = path.find_last_of("/\\");

    if (pos == std::string::npos) {
        return "";
    }

    return std::string(path.data(), path.data() + pos + 1);
}

/**
 * @throws std::ios::failure
 */
static std::string read_file(const std::string_view path) {
    std::ifstream file(path.data());
    file.exceptions(std::ios::failbit | std::ios::badbit);

    return std::string(std::istreambuf_iterator<char>(file), {});
}

#ifdef _WIN32
/**
 * @throws std::system_error
 */
static std::string read_res(const int32_t id, LPCSTR type) {
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

static smatches get_smatches(const std::string &data,
                             const std::string_view pattern) noexcept {
    const std::regex regex(pattern.data(), std::regex_constants::icase);

    return smatches(std::sregex_iterator(data.begin(), data.end(), regex), {});
}

/**
 * @throws exception
 */
static std::string inline_files(std::string data, const smatches &smatches,
                                const std::string_view dir,
                                const std::string_view tag) {
    for (auto iter = smatches.rbegin(); iter != smatches.rend(); ++iter) {
        const auto pos = iter->position();
        const auto filename = (*iter)[1].str();
        const auto element_len = (*iter)[0].str().size();
        const auto file_path = dir.data() + filename;

        try {
            auto content = read_file(file_path);
            content = std::string("<") + tag.data() + ">" + content + "</" +
                      tag.data() + ">";
            data.replace(pos, element_len, content);
        } catch (const std::ios::failure) {
            throw exception("Failed to read the file: " + file_path);
        }
    }

    return data;
}

/**
 * @throws exception
 */
static std::string inline_res(std::string data, const smatches &smatches,
                              const res_map &map, const std::string_view tag) {
    for (auto iter = smatches.rbegin(); iter != smatches.rend(); ++iter) {
        const auto pos = iter->position();
        const auto filename = (*iter)[1].str();
        const auto element_len = (*iter)[0].str().size();

        try {
            const auto resource_id = map.at(filename);
            auto content = read_res(resource_id, RT_RCDATA);
            content = std::string("<") + tag.data() + ">" + content + "</" +
                      tag.data() + ">";
            data.replace(pos, element_len, content);
        } catch (const std::out_of_range &e) {
            throw exception("Failed to read the resource: " + filename + "\n" +
                            "Out of range error: " + e.what());
        } catch (const std::system_error &e) {
            throw exception("Failed to read the resource: " + filename + "\n" +
                            "System error: " + e.code().message());
        }
    }

    return data;
}

static std::string remove_all_cr(std::string data) noexcept {
    data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());
    return data;
}

std::string inline_html(const std::string_view path) {
    auto directory = get_dir(path);
    auto data = read_file(path);

    const auto style_smatches = get_smatches(data, STYLE_PATTERN);
    data = inline_files(data, style_smatches, directory, STYLE_TAG);

    const auto script_smatches = get_smatches(data, SCRIPT_PATTERN);
    data = inline_files(data, script_smatches, directory, SCRIPT_TAG);

    return remove_all_cr(data);
}

#ifdef _WIN32
std::string inline_html(const int id, const res_map &map) {
    auto data = read_res(id, RT_HTML);

    const auto style_smatches = get_smatches(data, STYLE_PATTERN);
    data = inline_res(data, style_smatches, map, STYLE_TAG);

    const auto script_smatches = get_smatches(data, SCRIPT_PATTERN);
    data = inline_res(data, script_smatches, map, SCRIPT_TAG);

    return remove_all_cr(data);
}
#endif  // _WIN32
}  // namespace inline_html
