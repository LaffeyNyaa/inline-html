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

#include <optional>
#include <string>

#ifdef WIN32
#include <Windows.h>
#endif  // WIN32

namespace hse {
class HTMLStaticEmbedder {
   public:
    HTMLStaticEmbedder() noexcept = default;

    void load_html_from_file(std::string_view) noexcept;

#ifdef WIN32
    void load_html_from_res(int id) noexcept;
#endif  // WIN32

    const std::string &get_html_data() const noexcept {
        return html_data;
    }

   private:
    std::string html_data;

    std::optional<std::string> load_file(std::string_view path) noexcept;

#ifdef WIN32
    std::optional<std::string> load_res(int id, LPCSTR type) noexcept;
#endif  // WIN32

    void remove_all_cr() noexcept;

    void embed_static() noexcept;
};
}  // namespace hse

#endif  // HSE_HTML_STATIC_EMBEDDER_H