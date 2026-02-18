#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "common/Protocol.h"
#include "common/NetworkManager.h"

class ServerManager : public NetworkManager
{
    Q_OBJECT
public:
    explicit ServerManager(QObject *parent = nullptr);
    bool startServer(quint16 port); // Lance l'écoute sur le port choisi

    // Implémentation des méthodes abstraites
    bool initialize() override;
    void shutdown() override;

    void broadcast(const Packet &p); // Envoyer un paquet à tout le monde

signals:
    // Signaux pour prévenir l'interface graphique (GUI)
    void logMessage(const QString &msg);      // Pour afficher l'activité dans une console
    void packetReceived(const Packet &packet); // Quand un message arrive vraiment

private slots:
    void onNewConnection();    // Quand un client se connecte
    void onReadyRead();        // Quand des données arrivent sur une socket
    void onClientDisconnected(); // Quand un client s'en va

private:
    QTcpServer *m_server;            // Le "maître" qui écoute
    QList<QTcpSocket*> m_clients;    // La liste de tous les clients connectés
};

#endif // SERVERMANAGER_H