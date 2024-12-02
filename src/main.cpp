#include "core/WebApp.hpp"

int main(int argc, char *argv[]) {
    WebApp app(argc, argv);
    app.setSize(230, 320);
    app.setGlobalStyle("<style>"
      "html {"
        "--background-light: #f5f5f6;"
        "--background-dark: #1d1d1f;"
      "}"
      ":root {"
        "color-scheme: light dark;"
        "--background: var(--background-light);"
      "}"
      "@media (prefers-color-scheme: dark) {"
        ":root {"
          "color-scheme: dark;"
          "--background: var(--background-dark);"
        "}"
      "}"
      "body {"
        "margin: 0;"
        "padding: 0;"
        "display: flex;"
        "flex-direction: column;"
        "font-family: \"Segoe UI\", Tahoma, Geneva, Verdana, sans-serif;"
      "}"
    "</style>");
    app.addScreen("main", "Calculator");
    app.navigateTo("main");
    app.run();
    return 0;
}