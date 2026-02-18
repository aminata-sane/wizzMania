# 🗺️ Guide de Développement - WIZZ Mania

## 🛠️ Phase 1 : Cœur Réseau & Modularité
- [ ] **Définition du Protocole** : Créer un fichier `Protocol.h` partagé contenant une `enum PacketType { MESSAGE, WIZZ, JOIN, LEAVE };`.
- [ ] **Classe NetworkManager (Abstract)** : Gérer l'initialisation de Winsock et la fermeture des sockets.
- [ ] **Le Serveur** :
    - Utiliser `std::vector<SOCKET>` pour gérer plusieurs clients.
    - Créer un thread ou utiliser `select()` pour écouter les messages sans bloquer l'appli.
- [ ] **Le Client** :
    - Gérer la reconnexion automatique en cas de perte de serveur.

## 🎨 Phase 2 : Interface Qt (Signal/Slot)
- [ ] **Modularité GUI** : Créer une classe `ChatController` qui fait le pont entre `NetworkManager` et les fenêtres.
- [ ] **Signaux** : `emit dataReceived(Packet p);`.
- [ ] **Slots** : `void onDataReceived(Packet p);` qui met à jour la `QListWidget`.

## ⚡ Phase 3 : Le WIZZ Mania !
- [ ] **Effet Visuel** : Utiliser `QPropertyAnimation` sur la propriété `geometry` de la fenêtre principale pour créer la vibration.
- [ ] **Effet Sonore** : Jouer le célèbre son `.wav` du Wizz avec `QSoundEffect`.

## ✅ Phase 4 : Tests & Déploiement
- [ ] **Plan de Test** : Créer un script (ou un petit projet C++) qui simule 10 clients envoyant des messages simultanément.