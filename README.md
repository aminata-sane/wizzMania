# WIZZ Mania ⚡
**"Parce qu'un message ne suffit pas toujours, envoyez un Wizz !"**

WIZZ Mania est une réimplémentation moderne du système de chat MSN Messenger. Ce projet met l'accent sur la programmation réseau bas niveau (TCP/IP) et une interface utilisateur réactive.

## 🚀 Fonctionnalités
- 🗨️ **Chat temps réel** : Communication fluide via sockets TCP.
- 🔊 **Système de Wizz** : Faites vibrer l'écran de vos interlocuteurs.
- 👥 **Multi-clients** : Serveur capable de gérer une file de connexions simultanées.
- 🏗️ **Architecture Modulaire** : Séparation stricte entre la logique métier et la vue (Qt).

## 🛠️ Stack Technique
- **Langage** : C++ (Standard 17+)
- **Framework GUI** : Qt 6 (Core, Widgets, Network, Multimedia)
- **API Réseau** : Qt Network (TCP/IP)
- **Build System** : CMake
- **Plateforme** : macOS (avec Homebrew Qt6)

## 📦 Installation
1. Assurez-vous d'avoir **Qt 6** installé (via Homebrew sur macOS : `brew install qt6`)
2. Clonez le projet : `git clone https://github.com/Mehdichoucha/wizz_mania.git`
3. Accédez au dossier du projet : `cd wizzMania`

## 🔨 Compilation et Exécution

### 📋 Prérequis

#### macOS
- **Qt 6** : `brew install qt6`
- **CMake** : `brew install cmake`
- **Xcode Command Line Tools** : `xcode-select --install` (pour le compilateur)

#### Windows  
- **Qt 6** : Télécharger et installer depuis [qt.io/download](https://www.qt.io/download). Assurez-vous d'installer les composants Core, Widgets, Network, Multimedia.
- **CMake** : Télécharger depuis [cmake.org/download](https://cmake.org/download/) ou utiliser la version incluse dans Qt Creator.
- **Compilateur** : 
  - MinGW : Inclus avec Qt, recommandé pour compatibilité.
  - MSVC : Visual Studio Community (gratuit) pour un environnement professionnel.
- **Git** : Pour cloner le projet.

### 🏗️ Compilation

#### macOS
```bash
cd wizzMania
mkdir build
cd build
cmake ..
make
```

#### Windows (avec Qt Creator)
1. Ouvrir `CMakeLists.txt` dans Qt Creator
2. Configurer le kit (MinGW ou MSVC)
3. Compiler en mode Release

#### Windows (en ligne de commande avec MinGW)
```bash
cd wizzMania
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\mingw_64"  # Ajuster le chemin Qt
cmake --build . --config Release
```

#### Windows (en ligne de commande avec MSVC)
```bash
cd wizzMania
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc2019_64"  # Ajuster versions
cmake --build . --config Release
```

### 🚀 Lancement

#### macOS
```bash
./wizzMania.app/Contents/MacOS/wizzMania
```

#### Windows
```bash
wizzMania.exe
```

### 🎮 Utilisation
1. **Mode Serveur** : Choisissez "Serveur" et spécifiez un port (ex: 1234)
2. **Mode Client** : Choisissez "Client", saisissez l'IP du serveur et le port
3. Envoyez des messages ou des Wizz !

### 🌿 Branches Git
- `main` : Version complète
- `server` : Code serveur uniquement
- `client` : Code client uniquement  
- `common` : Composants partagés

## 📖 Licence
Projet réalisé dans le cadre d'un exercice académique sur les communications réseaux.