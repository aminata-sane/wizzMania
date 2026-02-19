#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include "server/ServerManager.h"

// Classe pour gérer le serveur de test
class ServerTestManager : public QObject
{
    Q_OBJECT

public:
    ServerTestManager(QObject *parent = nullptr) : QObject(parent)
    {
        m_server = new ServerManager(this);

        connect(m_server, &ServerManager::logMessage, this, &ServerTestManager::onServerLog);
        connect(m_server, &ServerManager::packetReceived, this, &ServerTestManager::onPacketReceived);
    }

    bool startServer(quint16 port = 1234)
    {
        qDebug() << "🚀 Démarrage du serveur de test sur le port" << port;
        return m_server->startServer(port);
    }

    void stopServer()
    {
        qDebug() << "⏹️ Arrêt du serveur de test";
        m_server->shutdown();
    }

private slots:
    void onServerLog(const QString &msg)
    {
        qDebug() << "[SERVEUR]" << msg;
    }

    void onPacketReceived(const Packet &packet)
    {
        QString typeStr;
        switch (packet.type) {
        case PacketType::MESSAGE: typeStr = "MESSAGE"; break;
        case PacketType::WIZZ: typeStr = "WIZZ"; break;
        case PacketType::JOIN: typeStr = "JOIN"; break;
        case PacketType::LEAVE: typeStr = "LEAVE"; break;
        }

        qDebug() << "[SERVEUR] 📨 Paquet reçu -" << typeStr << "de" << packet.sender << ":" << packet.content;
    }

private:
    ServerManager *m_server;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "🧪 WIZZ Mania - Serveur de Test";
    qDebug() << "===============================";

    quint16 port = 1234;
    if (argc >= 2) {
        port = QString(argv[1]).toInt();
        if (port == 0) port = 1234;
    }

    ServerTestManager manager;

    if (!manager.startServer(port)) {
        qDebug() << "❌ Échec du démarrage du serveur sur le port" << port;
        return 1;
    }

    qDebug() << "✅ Serveur démarré avec succès. En attente de connexions...";
    qDebug() << "💡 Lancez maintenant le test des clients dans un autre terminal";

    // Garder le serveur en marche
    return app.exec();
}

#include "test_server.moc"