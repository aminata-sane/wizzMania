cmake_minimum_required(VERSION 3.19)
project(wizzMania_tests VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Multimedia)

qt_standard_project_setup()

# Créer l'exécutable de test
qt_add_executable(test_clients
    test_clients.cpp
    # Inclure les sources du projet principal pour les tests
    ../wizzMania/src/common/Protocol.h
    ../wizzMania/src/common/NetworkManager.h
    ../wizzMania/src/common/NetworkManager.cpp
    ../wizzMania/src/client/ClientManager.h
    ../wizzMania/src/client/ClientManager.cpp
)

# Lier les bibliothèques Qt
target_link_libraries(test_clients
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
        Qt6::Multimedia
)

# Inclure les headers du projet principal
target_include_directories(test_clients PRIVATE
    ../wizzMania/src
)

# Créer aussi un exécutable pour tester le serveur
qt_add_executable(test_server
    test_server.cpp
    ../wizzMania/src/common/Protocol.h
    ../wizzMania/src/common/NetworkManager.h
    ../wizzMania/src/common/NetworkManager.cpp
    ../wizzMania/src/server/ServerManager.h
    ../wizzMania/src/server/ServerManager.cpp
)

target_link_libraries(test_server
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
        Qt6::Multimedia
)

target_include_directories(test_server PRIVATE
    ../wizzMania/src
)