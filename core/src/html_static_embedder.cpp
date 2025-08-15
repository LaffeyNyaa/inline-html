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

void hse::HTMLStaticEmbedder::load_html_from_file(std::string_view path) noexcept {
    auto result = load_file(path);
    if (!result.has_value()) {
        std::cerr << "Error loading file " << path << '\n';
        return;
    }

    raw_html_data = std::move(*result);

    remove_all_cr();
}

#ifdef WIN32
void hse::HTMLStaticEmbedder::load_html_from_res(int id) noexcept {
    auto result = load_res(id, RT_HTML);
    if (!result.has_value()) {
        std::cerr << "Error loading resource " << id << '\n';
        return;
    }

    raw_html_data = std::move(*result);
    remove_all_cr();
}
#endif  // WIN32

std::optional<std::string> hse::HTMLStaticEmbedder::load_file(
    std::string_view path) noexcept {
    auto path_data = path.data();
    std::ifstream file(path_data);

    if (!file.is_open()) {
        return std::nullopt;
    }

    auto data = std::string(std::istreambuf_iterator<char>(file),
                            std::istreambuf_iterator<char>());

    return data;
}

#ifdef WIN32
std::optional<std::string> hse::HTMLStaticEmbedder::load_res(int id,
                                                             LPCSTR type) noexcept {
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

void hse::HTMLStaticEmbedder::remove_all_cr() noexcept {
    auto iter = std::remove(raw_html_data.begin(), raw_html_data.end(), '\r');
    raw_html_data.erase(iter, raw_html_data.end());
}