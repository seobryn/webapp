#include "WebApp.hpp"

using json = nlohmann::json;

static std::map<std::string, std::string> _storage;

WebApp::WebApp(int argc, char **argv): instance(false, nullptr) {
    screensPath = std::filesystem::canonical(argv[0]).
        parent_path().
        parent_path().
        string() + "/Resources/screens/";

    instance.bind("storageSetItem", [&](const std::string &_jsonStr){
        auto data = json::parse(_jsonStr);
        std::string key = data[0].get<std::string>();
        std::string value = data[1].get<std::string>();
        _storage[key] = value;
        printf("Saving {'%s':'%s'}\n", key.c_str(), value.c_str());

        return "";
    });

    instance.bind("storageGetItem", [&](const std::string &_jsonStr)-> std::string {
        auto data = json::parse(_jsonStr);
        std::string key = data[0].get<std::string>();
        if(_storage.find(key) != _storage.end()){
            printf("Loading {'%s':'%s'}\n", key.c_str(), _storage[key].c_str());
            return "{\"data\":\"" + _storage[key] + "\"}";
        } 
        return "{}";
    });

    instance.bind("navigateTo", [&](std::string _json){
        auto data = json::parse(_json);
        std::string routeName = data[0].get<std::string>();
        printf("Navigating to [%s]\n", routeName.c_str());
        navigateTo(routeName);
        return "";
    });

    instance.bind("log", [&](std::string _json){
        auto data = json::parse(_json);
        std::string cleanMsg = data[0].get<std::string>();
        printf("%s\n", cleanMsg.c_str());
        return "";
    });
}

void WebApp::run(){
    instance.run();
}

void WebApp::setGlobalStyle(std::string styleTag){
    if(styleTag.find_first_of("<style>") == std::string::npos){
        printf("Warning: style tag must start with <style>\n");
        return;
    }
    globalStyle = styleTag;

}

std::string WebApp::loadFile(std::string path){
    std::ifstream t(path);

    if(!t.is_open()){
        printf("Warning: file [%s] not found\n", path.c_str());
        exit(EXIT_FAILURE);
    }

    std::stringstream buffer;
    buffer << t.rdbuf();
    t.close();

    return buffer.str();
}

void WebApp::setSize(int width, int height){
    instance.set_size(width, height, WEBVIEW_HINT_NONE);
}

void WebApp::navigateTo(std::string name){
    if(screens.find(name) == screens.end()){
        printf("Warning: screen [%s] not found\n", name.c_str());
        return;
    }

    instance.set_title(screens[name].title);
    instance.set_html(screens[name].content);
}

void WebApp::addScreen(std::string name, std::string screen_title){
    if(screens.find(name) != screens.end()){
        printf("Warning: screen [%s] already exists\n", name.c_str());
        return;
    }

    Screen screen;
    screen.title = screen_title;
    std::string screenFile = loadFile(screensPath + name + ".html");
    screen.content = screenFile.replace(screenFile.find("${GLOBAL_STYLE}"), std::string("${GLOBAL_STYLE}").length(), globalStyle);
    screens[name] = screen;
}