#include "gui/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Créer l'interface dynamiquement
    m_messageList = new QListWidget;
    m_messageInput = new QLineEdit;
    m_messageInput->setPlaceholderText("Tapez votre message...");
    m_sendMessageButton = new QPushButton("Envoyer Message");
    m_sendWizzButton = new QPushButton("Envoyer WIZZ");

    // Layout
    auto *layout = new QVBoxLayout;
    layout->addWidget(m_messageList);
    auto *hlayout = new QHBoxLayout;
    hlayout->addWidget(m_messageInput);
    hlayout->addWidget(m_sendMessageButton);
    hlayout->addWidget(m_sendWizzButton);
    layout->addLayout(hlayout);
    ui->centralwidget->setLayout(layout);

    // Initialiser l'animation de vibration
    m_vibrationAnimation = new QPropertyAnimation(this, "geometry");
    m_vibrationAnimation->setDuration(500);  // 500ms
    m_vibrationAnimation->setLoopCount(3);   // 3 vibrations

    // Initialiser le processus pour l'effet sonore du Wizz
    m_soundProcess = new QProcess(this);

    // Connecter les boutons
    connect(m_sendMessageButton, &QPushButton::clicked, this, &MainWindow::onSendMessageClicked);
    connect(m_sendWizzButton, &QPushButton::clicked, this, &MainWindow::onSendWizzClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setAsServer(int port)
{
    // On crée l'instance du manager serveur
    m_server = new ServerManager(this);

    // CONNEXION : Quand le serveur veut logger un message, on l'affiche
    connect(m_server, &ServerManager::logMessage, this, &MainWindow::onNewLogAvailable);

    // Tentative de démarrage du serveur
    if (!m_server->startServer(port)) {
        QMessageBox::critical(this, "Erreur", QString("Impossible de démarrer le serveur sur le port %1.").arg(port));
        return;
    }

    // Créer le contrôleur de chat
    m_chatController = new ChatController(m_server, m_messageList, this);
    connect(m_server, &ServerManager::packetReceived, m_chatController, &ChatController::onPacketReceived);
    connect(m_chatController, &ChatController::wizzReceived, this, &MainWindow::onWizzReceived);
}

void MainWindow::setAsClient(const QString &ip, int port)
{
    // On crée l'instance du manager client
    m_client = new ClientManager(this);

    // CONNEXION : Quand le client veut logger un message, on l'affiche
    connect(m_client, &ClientManager::logMessage, this, &MainWindow::onNewLogAvailable);
    connect(m_client, &ClientManager::connectedToServer, this, [this]() {
        onNewLogAvailable("Connecté au serveur !");
    });
    connect(m_client, &ClientManager::disconnectedFromServer, this, [this]() {
        onNewLogAvailable("Déconnecté du serveur.");
    });

    // Tentative de connexion au serveur
    if (!m_client->connectToServer(ip, port)) {
        QMessageBox::critical(this, "Erreur", QString("Impossible de se connecter à %1:%2.").arg(ip).arg(port));
        return;
    }

    // Créer le contrôleur de chat
    m_chatController = new ChatController(m_client, m_messageList, this);
    connect(m_client, &ClientManager::packetReceived, m_chatController, &ChatController::onPacketReceived);
    connect(m_chatController, &ChatController::wizzReceived, this, &MainWindow::onWizzReceived);
}

void MainWindow::onWizzReceived()
{
    // Jouer l'effet sonore du Wizz en utilisant un son système
#ifdef Q_OS_MACOS
    m_soundProcess->start("afplay", QStringList() << "/System/Library/Sounds/Bottle.aiff");
#elif defined(Q_OS_WIN)
    m_soundProcess->start("powershell", QStringList() << "-c" << "[console]::beep(800, 500)");
#else
    // Pour Linux ou autres
    m_soundProcess->start("beep", QStringList() << "-f" << "800" << "-l" << "500");
#endif

    // Log le Wizz
    onNewLogAvailable("WIZZ reçu ! Vibration en cours...");

    // Déclencher la vibration
    QRect currentGeometry = geometry();
    QRect startGeometry = currentGeometry;
    QRect endGeometry = currentGeometry.translated(10, 0);  // Décaler de 10px à droite

    // Animation : aller à droite puis revenir
    m_vibrationAnimation->setStartValue(startGeometry);
    m_vibrationAnimation->setKeyValueAt(0.25, endGeometry);
    m_vibrationAnimation->setKeyValueAt(0.5, startGeometry);
    m_vibrationAnimation->setKeyValueAt(0.75, endGeometry.translated(-20, 0));  // À gauche
    m_vibrationAnimation->setEndValue(startGeometry);

    m_vibrationAnimation->start();
}

void MainWindow::onSendMessageClicked()
{
    if (!m_chatController) {
        QMessageBox::information(this, "Info", "Veuillez d'abord choisir un mode (Serveur ou Client).");
        return;
    }
    QString message = m_messageInput->text().trimmed();
    if (message.isEmpty()) return;
    
    QString sender = (m_server ? "Serveur" : "Client");  // Temporaire, à remplacer par un vrai nom
    m_chatController->sendMessage(message, sender);
    m_messageInput->clear();
}

void MainWindow::onSendWizzClicked()
{
    if (!m_chatController) {
        QMessageBox::information(this, "Info", "Veuillez d'abord choisir un mode (Serveur ou Client).");
        return;
    }
    QString sender = (m_server ? "Serveur" : "Client");
    m_chatController->sendWizz(sender);
}

void MainWindow::onNewLogAvailable(const QString &msg)
{
    // Pour l'instant, on affiche dans la console de debug de Qt Creator
    // Plus tard, nous ajouterons un widget (ex: QTextEdit) dans l'UI pour voir les logs
    qDebug() << "[LOG]:" << msg;
}
