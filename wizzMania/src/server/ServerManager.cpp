#include "server/ServerManager.h"
#include <QDataStream>

ServerManager::ServerManager(QObject *parent) : NetworkManager(parent)
{
    m_server = new QTcpServer(this);

    // Quand quelqu'un se connecte, le serveur émet le signal newConnection
    connect(m_server, &QTcpServer::newConnection, this, &ServerManager::onNewConnection);
}

bool ServerManager::initialize()
{
    // Initialisation réseau de base + démarrage du serveur sur port par défaut
    if (!initNetwork()) return false;
    return startServer(1234);  // Port par défaut, peut être configurable plus tard
}

void ServerManager::shutdown()
{
    // Fermer le serveur et nettoyer
    if (m_server) {
        m_server->close();
        emit logMessage("Serveur arrêté.");
    }
    cleanupNetwork();
}

bool ServerManager::startServer(quint16 port)
{
    // On écoute sur toutes les interfaces réseau (IPv4 et IPv6) sur le port choisi
    if (!m_server->listen(QHostAddress::Any, port)) {
        emit logMessage("Erreur : Le serveur n'a pas pu démarrer.");
        return false;
    }
    emit logMessage(QString("Serveur démarré sur le port %1").arg(port));
    return true;
}

void ServerManager::onNewConnection()
{
    // On récupère la socket du nouveau client
    while (m_server->hasPendingConnections()) {
        QTcpSocket *clientSocket = m_server->nextPendingConnection();
        
        // On l'ajoute à notre liste
        m_clients << clientSocket;

        // On connecte les signaux de cette socket spécifique
        connect(clientSocket, &QTcpSocket::readyRead, this, &ServerManager::onReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &ServerManager::onClientDisconnected);

        emit logMessage(QString("Nouveau client connecté : %1").arg(clientSocket->peerAddress().toString()));
        
        // Broadcast JOIN
        Packet joinPacket(PacketType::JOIN, clientSocket->peerAddress().toString(), "");
        broadcast(joinPacket);
    }
}

void ServerManager::onReadyRead()
{
    // On récupère la socket qui a envoyé les données
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    // On utilise QDataStream pour lire les données selon notre protocole
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_6_0);

    while (!in.atEnd()) {
        Packet packet;
        in >> packet; // Utilise l'opérateur >> défini dans Protocol.h

        emit logMessage(QString("Paquet reçu de %1 : %2").arg(packet.sender, (packet.type == PacketType::WIZZ ? "WIZZ!" : packet.content)));
        
        // On renvoie le paquet à tout le monde (broadcast)
        broadcast(packet);
        
        emit packetReceived(packet);
    }
}

void ServerManager::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    emit logMessage("Un client s'est déconnecté.");
    
    // Broadcast LEAVE
    Packet leavePacket(PacketType::LEAVE, clientSocket->peerAddress().toString(), "");
    broadcast(leavePacket);
    
    m_clients.removeAll(clientSocket);
    clientSocket->deleteLater();
}

void ServerManager::broadcast(const Packet &p)
{
    // On envoie le paquet à tous les clients connectés
    for (QTcpSocket *client : m_clients) {
        QDataStream out(client);
        out.setVersion(QDataStream::Qt_6_0);
        out << p; // Utilise l'opérateur << défini dans Protocol.h
    }
}