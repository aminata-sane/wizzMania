#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

// Classe abstraite pour gérer la logique réseau de base
// Héritée par ServerManager et ClientManager
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    virtual ~NetworkManager();

    // Méthodes abstraites à implémenter par les sous-classes
    virtual bool initialize() = 0;  // Initialiser le réseau (ex. démarrer serveur ou connecter client)
    virtual void shutdown() = 0;    // Fermer proprement les sockets

signals:
    // Signaux communs
    void logMessage(const QString &msg);  // Pour logger les événements réseau
    void errorOccurred(const QString &error);  // En cas d'erreur réseau

protected:
    // Méthodes utilitaires pour les sous-classes
    bool initNetwork();  // Initialisation générale (si besoin, pour compatibilité)
    void cleanupNetwork();  // Nettoyage général
};

#endif // NETWORKMANAGER_H