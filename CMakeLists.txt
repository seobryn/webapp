cmake_minimum_required(VERSION 3.8)

project("MyApp" VERSION 0.1.0 LANGUAGES CXX)

set(SRC_FILES "src/main.cpp" "src/core/WebApp.cpp")
set(HEADERS "src/core/WebApp.hpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# add CPM
include(cmake/CPM.cmake)

# Add CPM Webview dependency
CPMAddPackage("gh:webview/webview#0.12.0")
CPMAddPackage("gh:nlohmann/json@3.11.3")

# Set Configs for macos
if(APPLE)
  set(MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}")
  set(MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT}")
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.${PROJECT_NAME}.${PROJECT_NAME}")
  set(MACOSX_BUNDLE_ICON_FILE "app_icon.icns")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
  set(APP_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${MACOSX_BUNDLE_ICON_FILE}")

  set(SCREENS_PATH "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources/screens")

  set_source_files_properties(${APP_ICON_FILE} PROPERTIES
       MACOSX_PACKAGE_LOCATION "Resources")

endif()

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${APP_ICON_FILE} ${SRC_FILES} ${HEADERS})

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Info.plist.in)
endif()

# Link Webview Library
target_link_libraries(${PROJECT_NAME} PRIVATE webview::core)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Add EVN Var to know the environment of the app
target_compile_definitions(${PROJECT_NAME} PRIVATE ENV="Dev")

# Set CPP Standard to 17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

# Copy assets to assets path
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/screens/" "${SCREENS_PATH}")