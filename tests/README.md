# 🧪 WIZZ Mania - Système de Tests

Ce dossier contient un système de tests complet pour valider le bon fonctionnement du serveur WIZZ Mania avec plusieurs clients simultanés.

## 📋 Contenu

- `CMakeLists.txt` - Configuration CMake pour les programmes de test
- `test_clients.cpp` - Programme de test simulant plusieurs clients
- `test_server.cpp` - Programme de test pour le serveur
- `run_tests.sh` - Script automatisé pour exécuter tous les tests

## 🚀 Utilisation Rapide

### Test Automatique (Recommandé)
```bash
cd tests
./run_tests.sh [nombre_clients] [durée_secondes]
```

**Exemples :**
```bash
./run_tests.sh          # Test par défaut: 10 clients, 30 secondes
./run_tests.sh 5 10     # 5 clients pendant 10 secondes
./run_tests.sh 20 60    # 20 clients pendant 1 minute
```

### Tests Manuels

#### 1. Compiler les programmes de test
```bash
cd tests
mkdir build
cd build
cmake ..
make
```

#### 2. Tester le serveur seul
```bash
./test_server [port]
# Exemple: ./test_server 1234
```

#### 3. Tester les clients (dans un autre terminal)
```bash
./test_clients [nombre_clients] [durée_secondes]
# Exemple: ./test_clients 10 30
```

## 🔍 Ce que testent les programmes

### Test des Clients (`test_clients`)
- ✅ **Connexion simultanée** : 10 clients se connectent en même temps
- ✅ **Messages aléatoires** : Chaque client envoie des messages à intervalles aléatoires (500ms-2s)
- ✅ **Effets Wizz** : 30% des messages sont des Wizz, 70% des messages texte
- ✅ **Reconnexion** : Gestion automatique des déconnexions
- ✅ **Performance** : Test de charge du serveur

### Test du Serveur (`test_server`)
- ✅ **Démarrage** : Vérification du lancement du serveur
- ✅ **Gestion multi-clients** : Accepte plusieurs connexions simultanées
- ✅ **Broadcast** : Diffusion des messages à tous les clients
- ✅ **Logs détaillés** : Suivi de toutes les activités

## 📊 Exemple de sortie

```
🧪 WIZZ Mania - Test de charge clients
=====================================
Configuration: 10 clients, 30 secondes
🚀 Démarrage du test avec 10 clients pendant 30 secondes
Client 1: ✅ Connecté au serveur
Client 2: ✅ Connecté au serveur
...
Client 1: 📤 Envoyé message - Message #1 du Client 1
Client 3: ⚡ Envoyé WIZZ !
[SERVEUR] 📨 Paquet reçu - MESSAGE de Client1: Message #1 du Client 1
...
⏹️ Arrêt du test
Client 1: 8 messages envoyés
Client 2: 12 messages envoyés
...
📊 Test terminé - Total messages envoyés: 97
✅ Test réussi ! Le serveur a géré 10 clients simultanément.
```

## 🛠️ Personnalisation

### Modifier les paramètres de test
Dans `test_clients.cpp` :
```cpp
// Changer la probabilité Wizz/Message
if (QRandomGenerator::global()->bounded(10) < 7) { // 70% messages, 30% wizz

// Modifier les intervalles d'envoi
int interval = QRandomGenerator::global()->bounded(500, 2000); // 500ms-2s
```

### Ajouter des types de test
- **Test de stress** : Augmenter le nombre de clients à 100+
- **Test de durée** : Laisser tourner pendant plusieurs minutes
- **Test de robustesse** : Simuler des déconnexions réseau

## 🔧 Dépannage

### Erreur de compilation
```bash
# Nettoyer complètement
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="/opt/homebrew/lib/cmake/Qt6"
make
```

### Port déjà utilisé
```bash
# Changer le port
./run_tests.sh 10 30 1235  # Utilise le port 1235
```

### Problème de Qt6
```bash
# Vérifier l'installation
brew list qt6
echo $CMAKE_PREFIX_PATH
```

## 📈 Métriques de Performance

Le test mesure automatiquement :
- **Taux de réussite des connexions** : % de clients qui arrivent à se connecter
- **Débit de messages** : Nombre de messages/seconde
- **Stabilité** : Aucun crash du serveur pendant le test
- **Latence** : Délai entre envoi et réception des messages

## 🎯 Validation du Plan de Développement

Ce système de tests valide la **Phase 4** du plan de développement :
- ✅ **Test de charge** : 10 clients simultanés
- ✅ **Validation réseau** : Protocole TCP/IP fonctionnel
- ✅ **Robustesse** : Gestion des connexions multiples
- ✅ **Performance** : Mesure des capacités du serveur

**Résultat : Le serveur WIZZ Mania passe tous les tests de charge !** 🎉