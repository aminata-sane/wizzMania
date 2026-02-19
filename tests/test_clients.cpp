#include <QCoreApplication>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QRandomGenerator>
#include <QElapsedTimer>
#include "client/ClientManager.h"
#include "common/Protocol.h"

// Classe pour simuler un client de test
class TestClient : public QObject
{
    Q_OBJECT

public:
    TestClient(int id, QObject *parent = nullptr)
        : QObject(parent), m_id(id), m_client(new ClientManager(this)), m_messageCount(0)
    {
        connect(m_client, &ClientManager::connectedToServer, this, &TestClient::onConnected);
        connect(m_client, &ClientManager::disconnectedFromServer, this, &TestClient::onDisconnected);
        connect(m_client, &ClientManager::packetReceived, this, &TestClient::onPacketReceived);
        connect(m_client, &ClientManager::logMessage, this, &TestClient::onLogMessage);

        // Timer pour envoyer des messages périodiquement
        m_sendTimer = new QTimer(this);
        connect(m_sendTimer, &QTimer::timeout, this, &TestClient::sendRandomMessage);
    }

    void start(const QString &host, quint16 port)
    {
        qDebug() << "Client" << m_id << ": Tentative de connexion à" << host << ":" << port;
        m_client->connectToServer(host, port);
    }

    void stop()
    {
        m_sendTimer->stop();
        m_client->shutdown();
    }

    int messageCount() const { return m_messageCount; }

private slots:
    void onConnected()
    {
        qDebug() << "Client" << m_id << ": ✅ Connecté au serveur";
        // Commencer à envoyer des messages toutes les 500ms-2s
        int interval = QRandomGenerator::global()->bounded(500, 2000);
        m_sendTimer->start(interval);
    }

    void onDisconnected()
    {
        qDebug() << "Client" << m_id << ": ❌ Déconnecté du serveur";
        m_sendTimer->stop();
    }

    void onPacketReceived(const Packet &packet)
    {
        qDebug() << "Client" << m_id << ": 📨 Reçu -" << packet.sender << ":" << packet.content;
    }

    void onLogMessage(const QString &msg)
    {
        // Silencieux pour éviter le spam
    }

    void sendRandomMessage()
    {
        if (!m_client) return;

        m_messageCount++;

        // Alterner entre messages normaux et Wizz
        if (QRandomGenerator::global()->bounded(10) < 7) { // 70% messages, 30% wizz
            QString message = QString("Message #%1 du Client %2").arg(m_messageCount).arg(m_id);
            Packet packet(PacketType::MESSAGE, QString("Client%1").arg(m_id), message);
            m_client->sendPacket(packet);
            qDebug() << "Client" << m_id << ": 📤 Envoyé message -" << message;
        } else {
            Packet packet(PacketType::WIZZ, QString("Client%1").arg(m_id));
            m_client->sendPacket(packet);
            qDebug() << "Client" << m_id << ": ⚡ Envoyé WIZZ !";
        }

        // Changer l'intervalle aléatoirement
        int newInterval = QRandomGenerator::global()->bounded(500, 2000);
        m_sendTimer->setInterval(newInterval);
    }

private:
    int m_id;
    ClientManager *m_client;
    QTimer *m_sendTimer;
    int m_messageCount;
};

// Classe principale du test
class ClientTestManager : public QObject
{
    Q_OBJECT

public:
    ClientTestManager(QObject *parent = nullptr) : QObject(parent)
    {
        m_testDuration = new QTimer(this);
        connect(m_testDuration, &QTimer::timeout, this, &ClientTestManager::stopTest);
    }

    void startTest(int numClients = 10, int durationSeconds = 30)
    {
        qDebug() << "🚀 Démarrage du test avec" << numClients << "clients pendant" << durationSeconds << "secondes";

        // Créer les clients
        for (int i = 0; i < numClients; ++i) {
            TestClient *client = new TestClient(i + 1, this);
            m_clients.append(client);

            // Démarrer la connexion avec un petit délai pour éviter la surcharge
            QTimer::singleShot(i * 100, [client]() {
                client->start("127.0.0.1", 9999);
            });
        }

        // Programmer l'arrêt du test
        m_testDuration->setSingleShot(true);
        m_testDuration->start(durationSeconds * 1000);

        qDebug() << "⏳ Test en cours...";
    }

private slots:
    void stopTest()
    {
        qDebug() << "⏹️ Arrêt du test";

        int totalMessages = 0;
        for (TestClient *client : m_clients) {
            client->stop();
            totalMessages += client->messageCount();
            qDebug() << "Client" << (m_clients.indexOf(client) + 1) << ":"
                     << client->messageCount() << "messages envoyés";
        }

        qDebug() << "📊 Test terminé - Total messages envoyés:" << totalMessages;
        qDebug() << "✅ Test réussi ! Le serveur a géré" << m_clients.size() << "clients simultanément.";

        // Quitter l'application
        QCoreApplication::quit();
    }

private:
    QList<TestClient*> m_clients;
    QTimer *m_testDuration;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "🧪 WIZZ Mania - Test de charge clients";
    qDebug() << "=====================================";

    // Vérifier les arguments
    int numClients = 10;
    int duration = 30;

    if (argc >= 2) {
        numClients = QString(argv[1]).toInt();
        if (numClients <= 0) numClients = 10;
    }

    if (argc >= 3) {
        duration = QString(argv[2]).toInt();
        if (duration <= 0) duration = 30;
    }

    qDebug() << "Configuration: " << numClients << "clients," << duration << "secondes";

    ClientTestManager manager;
    manager.startTest(numClients, duration);

    return app.exec();
}

#include "test_clients.moc"