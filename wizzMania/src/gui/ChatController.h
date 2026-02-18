#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include <QObject>
#include <QListWidget>
#include "common/Protocol.h"
#include "common/NetworkManager.h"

// Classe pour gérer la logique de chat : pont entre réseau et GUI
class ChatController : public QObject
{
    Q_OBJECT

public:
    explicit ChatController(NetworkManager *networkManager, QListWidget *messageList, QObject *parent = nullptr);

    // Envoi d'un message
    void sendMessage(const QString &message, const QString &sender);

    // Envoi d'un Wizz
    void sendWizz(const QString &sender);

signals:
    // Signal pour notifier la GUI (ex. vibration pour Wizz)
    void wizzReceived();

public slots:
    // Slot appelé quand un paquet est reçu du réseau
    void onPacketReceived(const Packet &packet);

private:
    NetworkManager *m_networkManager;
    QListWidget *m_messageList;

    // Méthode pour ajouter un message à la liste
    void addMessageToList(const QString &message);
};

#endif // CHATCONTROLLER_H