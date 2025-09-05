cmake_minimum_required(VERSION 3.16)
project(BusinessScraper VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
if(NOT WIN32)
    find_package(PkgConfig REQUIRED)
endif()


# Find libcurl and jsoncpp
if(NOT WIN32)
    pkg_check_modules(CURL REQUIRED libcurl)
    pkg_check_modules(JSONCPP REQUIRED jsoncpp)
    find_package(CURL CONFIG REQUIRED)
    find_package(jsoncpp CONFIG REQUIRED)
    find_package(ZLIB CONFIG REQUIRED)
endif()

# Set Qt to auto-generate MOC files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(include)

# Source files for core library
set(CORE_SOURCES
    src/core/Business.cpp
    src/core/BusinessScraperEngine.cpp
    src/scrapers/MapScraper.cpp
    src/scrapers/WebScraper.cpp
    src/output/Formatter.cpp
    src/utils/ConfigManager.cpp
    src/utils/FileUtils.cpp
)


# Create core library
add_library(business_scraper_core STATIC ${CORE_SOURCES})
target_include_directories(business_scraper_core PUBLIC include)

# Add curl/jsoncpp include dirs for Windows/vcpkg
if(WIN32)
    find_package(CURL CONFIG REQUIRED)
    find_package(jsoncpp CONFIG REQUIRED)
    find_package(ZLIB REQUIRED)
    target_include_directories(business_scraper_core PRIVATE
        $<TARGET_PROPERTY:CURL::libcurl,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:jsoncpp_lib,INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_link_libraries(business_scraper_core CURL::libcurl jsoncpp_lib ZLIB::ZLIB)
else()
    target_link_libraries(business_scraper_core
        ${CURL_LIBRARIES}
        ${JSONCPP_LIBRARIES}
    )
    target_compile_options(business_scraper_core PRIVATE
        ${CURL_CFLAGS_OTHER}
        ${JSONCPP_CFLAGS_OTHER}
    )
endif()



# GUI executable
add_executable(business_scraper_gui
    src/gui/main_gui.cpp
    src/gui/MainWindow.cpp
    src/gui/SearchWidget.cpp
    src/gui/ResultsWidget.cpp
    src/gui/ConfigDialog.cpp
    src/gui/StyleManager.cpp
    src/gui/TableItemDelegate.cpp
    include/gui/MainWindow.h
    include/gui/SearchWidget.h
    include/gui/ResultsWidget.h
    include/gui/ConfigDialog.h
    include/gui/StyleManager.h
    include/gui/TableItemDelegate.h
)

target_link_libraries(business_scraper_gui
    Qt6::Core
    Qt6::Widgets
    business_scraper_core
)

# Set output directory
set_target_properties(business_scraper_gui
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
