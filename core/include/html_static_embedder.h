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

    const std::string &get_raw_html_data() const noexcept {
        return raw_html_data;
    }

    const std::string &get_processed_html_data() const noexcept {
        return processed_html_data;
    }

   private:
    std::string raw_html_data;
    std::string processed_html_data;

    std::optional<std::string> load_file(std::string_view path) noexcept;

#ifdef WIN32
    std::optional<std::string> load_res(int id, LPCSTR type) noexcept;
#endif  // WIN32

    void remove_all_cr() noexcept;
};
}  // namespace hse

#endif  // HSE_HTML_STATIC_EMBEDDER_H