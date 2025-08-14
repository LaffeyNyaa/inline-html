#include "html_static_embedder.h"

#include <fstream>
#include <iostream>
#include <vector>

void hse::HTMLStaticEmbedder::load_html_from_file(const std::string &path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Error: Can't open file: " << path << '\n';
        return;
    }

    std::string data = std::string(std::istreambuf_iterator<char>(file),
                                   std::istreambuf_iterator<char>());

    raw_html_data = std::move(data);

    remove_all_cr();
}

#ifdef WIN32
void hse::HTMLStaticEmbedder::load_html_from_res(int id) {
    raw_html_data = std::move(load_res(id, RT_HTML));

    remove_all_cr();
}
#endif  // WIN32

std::string hse::HTMLStaticEmbedder::get_raw_html_data() const {
    return raw_html_data;
}

std::string hse::HTMLStaticEmbedder::get_processed_html_data() const {
    return processed_html_data;
}

#ifdef WIN32
std::string hse::HTMLStaticEmbedder::load_res(int id, LPCSTR type) {
    HMODULE module = GetModuleHandle(nullptr);
    LPSTR int_res = MAKEINTRESOURCE(id);
    HRSRC handle = FindResource(module, int_res, type);
    HGLOBAL loaded = LoadResource(module, handle);
    LPVOID data = LockResource(loaded);
    DWORD size = SizeofResource(module, handle);

    const char *res_data_p = static_cast<const char *>(data);
    std::string res_data = std::string(res_data_p, size);

    return res_data;
}

#endif  // WIN32

void hse::HTMLStaticEmbedder::remove_all_cr() {
    auto iter = std::remove(raw_html_data.begin(), raw_html_data.end(), '\r');
    raw_html_data.erase(iter, raw_html_data.end());
}