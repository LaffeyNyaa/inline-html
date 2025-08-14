#include "html_static_embedder.h"

#include <fstream>
#include <iostream>
#include <vector>

void hse::HTMLStaticEmbedder::load_html_from_file(std::string_view path) {
    auto result = load_file(path);
    if (!result.has_value()) {
        std::cerr << "Error loading file " << path << '\n';
        return;
    }

    raw_html_data = std::move(*result);

    remove_all_cr();
}

#ifdef WIN32
void hse::HTMLStaticEmbedder::load_html_from_res(int id) {
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
    std::string_view path) {
    const char *path_data = path.data();
    std::ifstream file(path_data);

    if (!file.is_open()) {
        return std::nullopt;
    }

    std::string data = std::string(std::istreambuf_iterator<char>(file),
                                   std::istreambuf_iterator<char>());

    return data;
}

#ifdef WIN32
std::optional<std::string> hse::HTMLStaticEmbedder::load_res(int id,
                                                             LPCSTR type) {
    HMODULE module = GetModuleHandle(nullptr);
    LPSTR int_res = MAKEINTRESOURCE(id);
    HRSRC handle = FindResource(module, int_res, type);
    HGLOBAL loaded = LoadResource(module, handle);
    LPVOID data = LockResource(loaded);
    DWORD size = SizeofResource(module, handle);

    if (GetLastError() != ERROR_SUCCESS) {
        return std::nullopt;
    }

    const char *res_data_p = static_cast<const char *>(data);
    std::string res_data = std::string(res_data_p, size);

    return res_data;
}

#endif  // WIN32

void hse::HTMLStaticEmbedder::remove_all_cr() {
    auto iter = std::remove(raw_html_data.begin(), raw_html_data.end(), '\r');
    raw_html_data.erase(iter, raw_html_data.end());
}