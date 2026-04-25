#ifdef _WIN32
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0A00
  #endif
  #ifndef WINVER
    #define WINVER 0x0A00
  #endif
#endif
#include "httplib.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

fs::path BASE_DIR;

// ===== MIME =====
std::string get_mime_type(const std::string &ext) {
    static std::unordered_map<std::string, std::string> mime = {
        {".html","text/html"},
        {".txt","text/plain"},
        {".jpg","image/jpeg"},
        {".png","image/png"},
        {".css","text/css"},
        {".js","application/javascript"},
        {".mp4","video/mp4"}
    };

    auto it = mime.find(ext);
    return it != mime.end() ? it->second : "application/octet-stream";
}

// ===== STREAM =====
void stream_file(const fs::path& path, const std::string& mime, httplib::Response& res) {
    auto size = fs::file_size(path);

    res.set_content_provider(size, mime.c_str(),
        [path](size_t offset, size_t length, httplib::DataSink &sink) {

            std::ifstream file(path, std::ios::binary);
            if (!file) return false;

            file.seekg(offset);

            const size_t BUF = 4 * 1024 * 1024;
            static thread_local std::vector<char> buffer(BUF);

            while (length > 0 && file) {
                size_t to_read = std::min(BUF, length);
                file.read(buffer.data(), to_read);
                size_t n = file.gcount();
                if (n == 0) break;

                sink.write(buffer.data(), n);
                length -= n;
            }

            sink.done();
            return true;
        }
    );
}

// ===== CHECK PATH AN TOÀN =====
bool is_safe_path(const fs::path& base, const fs::path& target) {
    auto b = base.lexically_normal();
    auto t = target.lexically_normal();

    return std::mismatch(b.begin(), b.end(), t.begin()).first == b.end();
}

// ===== PARENT PATH =====
std::string parent_path(std::string p) {
    if (p.size() > 1 && p.back() == '/') p.pop_back();

    auto pos = p.find_last_of('/');
    if (pos == std::string::npos || pos == 0) return "/";

    return p.substr(0, pos + 1);
}

// ===== HTML =====
std::string render_dir(const fs::path& dir, std::string url) {
    if (url.back() != '/') url += "/";

    std::stringstream ss;

    ss << "<html><head><meta charset='utf-8'>"

       << "<style>"

// ===== LAYOUT =====
<< "body{font-family:sans-serif;background:#f3f4f6;padding:12px;}"
<< "h2{margin-bottom:10px;}"

<< "ul{list-style:none;padding:0;}"
<< "li{margin:8px 0;display:flex;justify-content:space-between;align-items:center;"
<< "padding:6px 10px;border-radius:8px;}"
<< "li:hover{background:#e5e7eb;}"

// ===== LINK =====
<< "a{text-decoration:none;}"

// file/folder
<< ".folder{color:#2563eb;font-weight:500;}"
<< ".file{color:#111827;}"

// ===== BUTTON REAL =====
<< ".btn{"
<< "display:inline-block;"
<< "padding:6px 14px;"
<< "border-radius:8px;"
<< "font-size:13px;"
<< "font-weight:600;"
<< "cursor:pointer;"
<< "user-select:none;"
<< "transition:all 0.1s ease;"
<< "box-shadow:0 4px 0 rgba(0,0,0,0.25);" // nổi
<< "}"

// nhấn xuống
<< ".btn:active{"
<< "transform:translateY(2px);"
<< "box-shadow:0 2px 0 rgba(0,0,0,0.25);"
<< "}"

// DOWNLOAD
<< ".download{"
<< "background:linear-gradient(#34d399,#10b981);"
<< "color:white;"
<< "}"
<< ".download:hover{"
<< "background:linear-gradient(#6ee7b7,#10b981);"
<< "}"

// BACK
<< ".back{"
<< "background:linear-gradient(#e5e7eb,#d1d5db);"
<< "color:#111827;"
<< "}"
<< ".back:hover{"
<< "background:linear-gradient(#f3f4f6,#d1d5db);"
<< "}"

// ===== RIGHT SIDE =====
<< ".right{display:flex;gap:8px;}"

<< "</style></head><body>";

    ss << "<h2>Index of " << url << "</h2><ul>";

    // ===== BACK =====
    if (url != "/") {
        ss << "<li>"
           << "<a class=\"btn back\" href=\"" << parent_path(url) << "\">⬅ Back</a>"
           << "<span></span>"
           << "</li>";
    }

    for (auto &e : fs::directory_iterator(dir)) {
        std::string name = e.path().filename().string();

        ss << "<li>";

        // LEFT: file/folder
        if (fs::is_directory(e)) {
            ss << "<a class=\"folder\" href=\"" << url << name << "/\">📁 "
               << name << "/</a>";
            ss << "<div class=\"right\"></div>";
        } else {
            ss << "<a class=\"file\" href=\"" << url << name << "\">📄 "
               << name << "</a>";

            // RIGHT: button
            ss << "<div class=\"right\">"
               << "<a class=\"btn download\" href=\"/download" << url << name << "\">⬇ Download</a>"
               << "</div>";
        }

        ss << "</li>";
    }

    ss << "</ul></body></html>";
    return ss.str();
}


std::string render_dir1(const fs::path& dir, std::string url) {
    if (url.back() != '/') url += "/";

    std::stringstream ss;

    ss << "<html><head><meta charset='utf-8'>"
       << "<style>body{font-family:sans-serif;} a{margin-right:10px;}</style>"
       << "</head><body>";

    ss << "<h2>Index of " << url << "</h2><ul>";

    if (url != "/") {
        ss << "<li><a href=\"" << parent_path(url) << "\">[.. Back ]</a></li>";
    }

    for (auto &e : fs::directory_iterator(dir)) {
        std::string name = e.path().filename().string();

        ss << "<li>";

        if (fs::is_directory(e)) {
            ss << "<a href=\"" << url << name << "/\">" << name << "/</a>";
        } else {
            ss << "<a href=\"" << url << name << "\">" << name << "</a>";
            ss << "<a href=\"/download" << url << name << "\">[ DL ]</a>";
        }

        ss << "</li>";
    }

    ss << "</ul></body></html>";
    return ss.str();
}

// ================= MAIN =================
int main(int argc, char* argv[]) {

    fs::path base = (argc > 1) ? argv[1] : "./shared";

    if (!fs::exists(base)) {
        std::cout << "ERROR: folder not exist\n";
        return 1;
    }

    BASE_DIR = fs::canonical(base);

    httplib::Server svr;

    // ===== DOWNLOAD =====
    svr.Get(R"(/download/(.*))", [](const httplib::Request& req, httplib::Response& res) {

        fs::path rel = req.matches[1].str();
        fs::path full = BASE_DIR / rel;

        if (!fs::exists(full)) {
            res.status = 404;
            return;
        }

        full = fs::canonical(full);

        if (!is_safe_path(BASE_DIR, full)) {
            res.status = 403;
            return;
        }

        res.set_header("Content-Disposition",
            "attachment; filename=\"" + full.filename().string() + "\"");

        stream_file(full, "application/octet-stream", res);
    });

    // ===== MAIN =====
    svr.Get(R"(/(.*))", [](const httplib::Request &req, httplib::Response &res) {

        std::string sub = req.matches[1].str();
        std::string url = "/" + sub;

        fs::path full = BASE_DIR / sub;

        if (!fs::exists(full)) {
            res.status = 404;
            res.set_content("Not found", "text/plain");
            return;
        }

        full = fs::canonical(full);

        if (!is_safe_path(BASE_DIR, full)) {
            res.status = 403;
            res.set_content("Forbidden", "text/plain");
            return;
        }

        // ===== DIR =====
        if (fs::is_directory(full)) {

            if (url.back() != '/') {
                res.status = 301;
                res.set_header("Location", url + "/");
                return;
            }

            res.set_content(render_dir(full, url), "text/html; charset=utf-8");
            return;
        }

        // ===== FILE =====
        stream_file(full, get_mime_type(full.extension().string()), res);
    });

    std::cout << "RUN: http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}