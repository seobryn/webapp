#ifndef WEBAPP_HPP
#define WEBAPP_HPP

#include <webview/webview.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>

struct Screen {
    std::string title;
    std::string content;
};

class WebApp {
    public:
        WebApp() = delete;
        WebApp(int argc, char **argv);
        ~WebApp() = default;

        void addScreen(std::string name, std::string screen_title);
        void setSize(int width, int height);
        void navigateTo(std::string name);
        void setGlobalStyle(std::string styleTag);
        void run();

    private:
        std::string loadFile(std::string path);

        std::map<std::string, Screen> screens;
        
        std::string screensPath;
        std::string globalStyle;

        webview::webview instance;
};

#endif