#include "httplib.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace fs = std::filesystem;

fs::path BASE_DIR;

// MIME type cơ bản
std::string get_mime_type(const std::string &ext) {
    static std::unordered_map<std::string, std::string> mime = {
        {".html", "text/html"},
        {".htm", "text/html"},
        {".txt", "text/plain"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        {".pdf", "application/pdf"},
        {".mp4", "video/mp4"}
    };

    auto it = mime.find(ext);
    return it != mime.end() ? it->second : "application/octet-stream";
}

// đọc file binary
std::string read_file(const fs::path &path) {
    std::ifstream file(path, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
}

// HTML list folder
std::string render_directory(const fs::path &dir, const std::string &url_path) {
    std::stringstream html;

    html << "<html><head><meta charset='utf-8'>"
         << "<style>"
         << "body{font-family:sans-serif;}"
         << "a{text-decoration:none;margin-right:8px;}"
         << ".icon{width:16px;vertical-align:middle;}"
         << "</style></head><body>";

    html << "<h2>Index of " << url_path << "</h2><ul>";

    if (url_path != "/") {
        html << "<li>"
			 << "<a href=\"../\">"
			 << "<img src=\"/back.png\" class=\"icon\"/> .. Back"
			 << "</a></li>";
    }

    for (auto &entry : fs::directory_iterator(dir)) {
        std::string name = entry.path().filename().string();

        std::string link = url_path;
        if (link.back() != '/') link += "/";
        link += name;

        html << "<li>";

        if (fs::is_directory(entry)) {
            html << "<a href=\"" << link << "/\">" << name << "/</a>";
        } else {
            // link xem file
            html << "<a href=\"" << link << "\">" << name << "</a>";

            // link download (icon)
            std::string download_link = "/download/";
            if (url_path != "/") {
                download_link += url_path.substr(1) + "/";
            }
            download_link += name;

            html << "<a href=\"" << download_link << "\">"
                 << "<img src=\"/download.png\" class=\"icon\"/>"
                 << "</a>";
        }

        html << "</li>";
    }

    html << "</ul></body></html>";
    return html.str();
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
        BASE_DIR = fs::canonical(argv[1]);
    } else {
        BASE_DIR = fs::canonical("./shared");
    }
    httplib::Server svr;

	// route download file
	svr.Get(R"(/download/(.*))", [](const httplib::Request& req, httplib::Response& res) {
		fs::path requested = BASE_DIR / req.matches[1].str();
		fs::path canonical = fs::weakly_canonical(requested);

		if (canonical.string().find(BASE_DIR.string()) != 0 || !fs::exists(canonical)) {
			res.status = 404;
			return;
		}

		std::ifstream file(canonical, std::ios::binary);
		std::stringstream buffer;
		buffer << file.rdbuf();

		std::string filename = canonical.filename().string();

		res.set_header("Content-Disposition", "attachment; filename=\"" + filename + "\"");
		res.set_content(buffer.str(), "application/octet-stream");
	});

	// serve icon back.png
	svr.Get("/back.png", [](const httplib::Request&, httplib::Response& res) {
		std::ifstream file("res/back.png", std::ios::binary);
		std::stringstream buffer;
		buffer << file.rdbuf();
		res.set_content(buffer.str(), "image/png");
	});

	// serve icon download.png
	svr.Get("/download.png", [](const httplib::Request&, httplib::Response& res) {
		std::ifstream file("res/download.png", std::ios::binary);
		std::stringstream buffer;
		buffer << file.rdbuf();
		res.set_content(buffer.str(), "image/png");
	});

    svr.Get(R"(/(.*))", [](const httplib::Request &req, httplib::Response &res) {
        std::string url_path = "/" + req.matches[1].str();

        // normalize path
        fs::path requested = BASE_DIR / fs::path(req.matches[1].str());
        fs::path canonical_path = fs::weakly_canonical(requested);

        // 🔒 chặn path traversal
        if (canonical_path.string().find(BASE_DIR.string()) != 0) {
            res.status = 403;
            res.set_content("Forbidden", "text/plain");
            return;
        }

        if (!fs::exists(canonical_path)) {
            res.status = 404;
            res.set_content("Not found", "text/plain");
            return;
        }

        // nếu là folder
        if (fs::is_directory(canonical_path)) {
            std::string html = render_directory(canonical_path, url_path);
            res.set_content(html, "text/html; charset=utf-8");
            return;
        }

        // nếu là file
        std::ifstream file(canonical_path, std::ios::binary);
        if (!file) {
            res.status = 500;
            res.set_content("Cannot open file", "text/plain");
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        std::string ext = canonical_path.extension().string();
        res.set_content(buffer.str(), get_mime_type(ext));
    });

    printf("Server running at http://0.0.0.0:8080\n");
    svr.listen("0.0.0.0", 8080);
}