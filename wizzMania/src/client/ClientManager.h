#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "common/Protocol.h"
#include "common/NetworkManager.h"

class ClientManager : public NetworkManager
{
    Q_OBJECT

public:
    explicit ClientManager(QObject *parent = nullptr);
    ~ClientManager();

    // Connexion au serveur
    bool connectToServer(const QString &host, quint16 port);

    // Implémentation des méthodes abstraites
    bool initialize() override;  // Tentative de connexion
    void shutdown() override;    // Déconnexion propre

    // Envoi d'un paquet
    void sendPacket(const Packet &packet);

signals:
    // Signaux pour l'interface
    void connectedToServer();
    void disconnectedFromServer();
    void packetReceived(const Packet &packet);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void attemptReconnection();  // Reconexion automatique

private:
    QTcpSocket *m_socket;
    QTimer *m_reconnectTimer;
    QString m_host;
    quint16 m_port;
    bool m_autoReconnect = true;  // Activer la reconnexion auto
    bool m_wasConnected = false;  // Pour savoir si on était connecté
};

#endif // CLIENTMANAGER_H