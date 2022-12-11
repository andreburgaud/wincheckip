#include "pch.h"
#include "color.h"
#include "version.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;

const std::wstring CHECKIP_URL = L"https://checkip.amazonaws.com";
//const std::wstring VERSION = L"0.1.0";
bool debug = false;

hstring CheckIP(std::wstring url) {
    Uri checkipUri{ url };
    HttpClient httpClient{};
    auto headers{ httpClient.DefaultRequestHeaders() };

    std::wstring useragent = std::format(L"checkip/1.2.3", VER_PRODUCTVERSION_STR);
    if (url == L"https://ifconfig.co") {
        // https://ifconfig.co requires a specific user agent otherwise 
        // it expects displaying HTML for a browser
        useragent = std::format(L"curl/{0}", VER_PRODUCTVERSION_STR);
    }
    if (debug) {
        std::wcerr << ">>> url: " << url << std::endl;
        std::wcerr << ">>> user agent: " << useragent << std::endl;
    }
    headers.UserAgent().TryParseAdd(useragent);
    HttpGetStringResult result = httpClient.TryGetStringAsync(checkipUri).get();
    if (result.Succeeded()) {
        return result.Value();
    }
    std::wcerr << result.ToString().c_str() << std::endl;
    return hstring();
}

static std::wstring getPath(char* path) {
    auto p = std::filesystem::path{ path };
    return p.stem().wstring();
}

static void showUsage(std::wstring path) {
    std::wcerr << yellow << "Usage:\n" << deftextcol
        << "\t" << path << " [OPTIONS] [URL]\n"
        << yellow << "Options:\n" << deftextcol
        << "\t-h,--help\t Display this help and exit\n"
        << "\t-u,--url URL\t Point to a custom check IP address service (default: https://checkip.amazonaws.com)\n"
        << "\t-v,--version\t Output version information and exit\n"
        << "\t-d,--debug\t Display extra information"
        << std::endl;
}

static void showVersion(std::wstring path, std::wstring version) {
    std::wcerr << yellow << path << " " << version << deftextcol << std::endl;
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return s;
}

int main(int argc, char* argv[] /*, char* envp[] */) {
    init_apartment();
    std::vector <std::string> args;
    std::wstring checkIpUrl = CHECKIP_URL.c_str();

    concolinit();

    for (int i = 1; i < argc; i++) {
        std::string arg = toLower(argv[i]);

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
            std::string u = argv[i + 1];
            checkIpUrl = std::wstring(u.begin(), u.end());
        }
    }
    
    hstring ipaddress = CheckIP(checkIpUrl);
    std::wcout << green << ipaddress.c_str() << deftextcol << std::endl;
}
