#include "client/ClientManager.h"
#include <QDataStream>
#include <QHostAddress>

ClientManager::ClientManager(QObject *parent) : NetworkManager(parent)
{
    m_socket = new QTcpSocket(this);
    m_reconnectTimer = new QTimer(this);

    // Connexions des signaux
    connect(m_socket, &QTcpSocket::connected, this, &ClientManager::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientManager::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientManager::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &ClientManager::onErrorOccurred);

    // Timer pour reconnexion
    connect(m_reconnectTimer, &QTimer::timeout, this, &ClientManager::attemptReconnection);
    m_reconnectTimer->setInterval(5000);  // Essayer toutes les 5 secondes
}

ClientManager::~ClientManager()
{
    shutdown();
}

bool ClientManager::connectToServer(const QString &host, quint16 port)
{
    m_host = host;
    m_port = port;

    emit logMessage(QString("Tentative de connexion à %1:%2").arg(host).arg(port));
    m_socket->connectToHost(QHostAddress(host), port);

    // Attendre la connexion (timeout 5s)
    if (!m_socket->waitForConnected(5000)) {
        emit logMessage("Échec de connexion : " + m_socket->errorString());
        return false;
    }
    return true;
}

bool ClientManager::initialize()
{
    // Initialisation réseau + connexion par défaut (localhost:1234)
    if (!initNetwork()) return false;
    return connectToServer("127.0.0.1", 1234);
}

void ClientManager::shutdown()
{
    m_autoReconnect = false;
    m_reconnectTimer->stop();
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }
    cleanupNetwork();
}

void ClientManager::sendPacket(const Packet &packet)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        emit logMessage("Impossible d'envoyer : socket non connectée");
        return;
    }

    QDataStream out(m_socket);
    out.setVersion(QDataStream::Qt_6_0);
    out << packet;

    emit logMessage("Paquet envoyé : " + packet.content);
}

void ClientManager::onConnected()
{
    emit logMessage("Connecté au serveur !");
    emit connectedToServer();
    m_reconnectTimer->stop();  // Arrêter les tentatives de reconnexion
    m_wasConnected = true;
    
    // Envoyer un paquet JOIN
    sendPacket(Packet(PacketType::JOIN, "Client", ""));
}

void ClientManager::onDisconnected()
{
    emit logMessage("Déconnecté du serveur.");
    emit disconnectedFromServer();

    // Envoyer LEAVE si on était connecté
    if (m_wasConnected) {
        sendPacket(Packet(PacketType::LEAVE, "Client", ""));
        m_wasConnected = false;
    }

    // Reconexion automatique si activée
    if (m_autoReconnect) {
        emit logMessage("Tentative de reconnexion dans 5 secondes...");
        m_reconnectTimer->start();
    }
}

void ClientManager::onReadyRead()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_6_0);

    Packet packet;
    in >> packet;

    emit logMessage("Paquet reçu : " + packet.content);
    emit packetReceived(packet);
}

void ClientManager::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    emit logMessage("Erreur socket : " + m_socket->errorString());
}

void ClientManager::attemptReconnection()
{
    if (m_socket->state() == QAbstractSocket::UnconnectedState) {
        connectToServer(m_host, m_port);
    }
}