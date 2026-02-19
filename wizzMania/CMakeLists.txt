cmake_minimum_required(VERSION 3.19)
project(wizzMania VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. On ajoute "Network" à la liste des composants
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Multimedia LinguistTools)

qt_standard_project_setup()

# 2. On liste les fichiers par module
qt_add_executable(wizzMania
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/gui/mainwindow.ui
    src/gui/ChatController.cpp
    src/gui/ChatController.h
    src/server/ServerManager.cpp
    src/server/ServerManager.h
    src/client/ClientManager.cpp
    src/client/ClientManager.h
    src/common/Protocol.h
    src/common/NetworkManager.h
    src/common/NetworkManager.cpp
)

qt_add_translations(wizzMania TS_FILES wizzMania_fr_FR.ts)

# 3. On lie Qt6::Network ici
target_link_libraries(wizzMania
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
        Qt6::Multimedia
)

# On indique à CMake où trouver les headers
target_include_directories(wizzMania PRIVATE
    src
)

# Le reste (install et deploy) peut rester tel quel
include(GNUInstallDirs)
install(TARGETS wizzMania
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
