#ifndef HTML_STATIC_EMBEDDER_H
#define HTML_STATIC_EMBEDDER_H

#include <string>

#ifdef WIN32
#include <Windows.h>
#endif  // WIN32

namespace hse {
class HTMLStaticEmbedder {
   public:
    HTMLStaticEmbedder() = default;

    void load_html_from_file(const std::string &path);

#ifdef WIN32
    void load_html_from_res(int id);
#endif  // WINDOWS

    std::string get_raw_html_data() const;
    std::string get_processed_html_data() const;

   private:
    std::string raw_html_data;
    std::string processed_html_data;

#ifdef WIN32
    std::string load_res(int id, LPCSTR type);
#endif  // WIN32
};
}  // namespace hse

#endif  // HTML_STATIC_EMBEDDER_H