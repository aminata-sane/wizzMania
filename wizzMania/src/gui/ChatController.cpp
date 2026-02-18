#include "ChatController.h"
#include "server/ServerManager.h"
#include "client/ClientManager.h"
#include <QListWidgetItem>

ChatController::ChatController(NetworkManager *networkManager, QListWidget *messageList, QObject *parent)
    : QObject(parent), m_networkManager(networkManager), m_messageList(messageList)
{
    // La connexion au signal packetReceived se fait dans MainWindow, car c'est spécifique à Server/Client
}

void ChatController::sendMessage(const QString &message, const QString &sender)
{
    Packet packet(PacketType::MESSAGE, sender, message);

    // Si c'est un serveur, broadcast
    if (auto server = qobject_cast<ServerManager*>(m_networkManager)) {
        server->broadcast(packet);
        // Ajouter aussi localement
        addMessageToList(QString("[%1] %2").arg(sender, message));
    }
    // Si c'est un client, envoyer au serveur
    else if (auto client = qobject_cast<ClientManager*>(m_networkManager)) {
        client->sendPacket(packet);
    }
}

void ChatController::sendWizz(const QString &sender)
{
    Packet packet(PacketType::WIZZ, sender);

    // Même logique que sendMessage
    if (auto server = qobject_cast<ServerManager*>(m_networkManager)) {
        server->broadcast(packet);
        addMessageToList(QString("[%1] envoie un WIZZ !").arg(sender));
        emit wizzReceived();  // Pour vibration
    }
    else if (auto client = qobject_cast<ClientManager*>(m_networkManager)) {
        client->sendPacket(packet);
    }
}

void ChatController::onPacketReceived(const Packet &packet)
{
    QString displayMessage;

    switch (packet.type) {
    case PacketType::MESSAGE:
        displayMessage = QString("[%1] %2").arg(packet.sender, packet.content);
        break;
    case PacketType::WIZZ:
        displayMessage = QString("[%1] envoie un WIZZ !").arg(packet.sender);
        emit wizzReceived();  // Déclencher vibration
        break;
    case PacketType::JOIN:
        displayMessage = QString("[%1] a rejoint le chat").arg(packet.sender);
        break;
    case PacketType::LEAVE:
        displayMessage = QString("[%1] a quitté le chat").arg(packet.sender);
        break;
    }

    addMessageToList(displayMessage);
}

void ChatController::addMessageToList(const QString &message)
{
    if (m_messageList) {
        m_messageList->addItem(new QListWidgetItem(message));
        // Scroll to bottom
        m_messageList->scrollToBottom();
    }
}