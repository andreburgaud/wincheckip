#include "pch.h"
#include "color.h"
#include "version.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace color;

const std::wstring CHECKIP_URL = L"https://checkip.amazonaws.com";
bool debug = false;

auto inline trim(const std::string& s) -> std::string {
    constexpr const char* whitespace{ " \t\r\n\v\f" };
    if (s.empty()) return s;
    const auto first{ s.find_first_not_of(whitespace) };
    if (first == std::string::npos) return {};
    const auto last{ s.find_last_not_of(whitespace) };
    return s.substr(first, (last - first + 1));
}

auto checkIP(const std::wstring& url) {
    Uri checkipUri{ url };
    HttpClient httpClient{};
    auto headers{ httpClient.DefaultRequestHeaders() };

    std::wstring useragent{ std::format(L"checkip/{0}", VER_PRODUCTVERSION_STR) };
    if (url == L"https://ifconfig.co") {
        // https://ifconfig.co requires a specific user agent
        // otherwise it returns HTML to render in a browser
        useragent = std::format(L"curl/{0}", VER_PRODUCTVERSION_STR);
    }
    if (debug) {
        std::wcerr << ">>> url: " << url << std::endl;
        std::wcerr << ">>> user agent: " << useragent << std::endl;
    }
    headers.UserAgent().TryParseAdd(useragent);
    HttpGetStringResult result = httpClient.TryGetStringAsync(checkipUri).get();
    if (result.Succeeded()) {
        return trim(to_string(result.Value()));
    }
    // Error
    std::wcerr << red << result.ToString().c_str() << deftextcol << std::endl;
    return to_string(hstring());
}

auto getPath(const char* path) -> std::wstring {
    auto p = std::filesystem::path{ path };
    return p.stem().wstring();
}

void showUsage(std::wstring path) {
    std::wcerr << yellow << "Usage:\n" << deftextcol
        << "\t" << path << " [OPTIONS] [URL]\n"
        << yellow << "Options:\n" << deftextcol
        << "\t-h,--help\t Display this help and exit\n"
        << "\t-u,--url URL\t Point to a custom check IP address service (default: https://checkip.amazonaws.com)\n"
        << "\t-v,--version\t Output version information and exit\n"
        << "\t-d,--debug\t Display extra information"
        << std::endl;
}

void showVersion(const std::wstring path, const std::wstring version) {
    std::wcerr << yellow << path << " " << version << deftextcol << std::endl;
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return s;
}

auto main(int argc, char* argv[] /*, char* envp[] */) -> int {
    init_apartment();
    std::vector <std::string> args;
    std::wstring checkIpUrl{ CHECKIP_URL.c_str() };

    concolinit();

    for (int i = 1; i < argc; i++) {
        std::string arg{ toLower(argv[i]) };

        if ((arg == "-h") || (arg == "/?") || (arg == "--help")) {
            showUsage(getPath(argv[0]));
            return 0;
        }
        if ((arg == "-v") || (arg == "--version")) {
            showVersion(getPath(argv[0]), VER_PRODUCTVERSION_STR);
            return 0;
        }
        if ((arg == "-d") || (arg == "--debug")) {
            debug = true;
            continue;
        }
        if ((arg == "-u") || (arg == "--url")) {
            std::string u{ argv[i + 1] };
            checkIpUrl = std::wstring(u.begin(), u.end());
        }
    }
    
    auto ipaddress = checkIP(checkIpUrl);
    std::wcout << green << ipaddress.c_str() << deftextcol << std::endl;
}
