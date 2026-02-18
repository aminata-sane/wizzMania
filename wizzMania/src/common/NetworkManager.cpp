#include "common/NetworkManager.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    // Initialisation si nécessaire
}

NetworkManager::~NetworkManager()
{
    // Nettoyage automatique si nécessaire, mais pas shutdown() car pure virtual
}

bool NetworkManager::initNetwork()
{
    // Logique d'initialisation réseau générale
    // Sur Qt, pas besoin d'init Winsock explicite, Qt gère ça
    // Mais on peut ajouter des vérifications ou logs
    emit logMessage("Réseau initialisé.");
    return true;
}

void NetworkManager::cleanupNetwork()
{
    // Nettoyage général
    emit logMessage("Réseau nettoyé.");
}