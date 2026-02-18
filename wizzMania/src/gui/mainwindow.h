#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QProcess>
#include "server/ServerManager.h"
#include "client/ClientManager.h"
#include "ChatController.h" 

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr); // Ajout de explicit pour la sécurité
    ~MainWindow();

    // Configuration des modes
    void setAsServer(int port = 1234);
    void setAsClient(const QString &ip = "127.0.0.1", int port = 1234);

private slots:
    // Slot pour mettre à jour l'interface avec les évènements réseau
    void onNewLogAvailable(const QString &msg);
    
    // Slot pour gérer la réception d'un Wizz
    void onWizzReceived();
    
    // Slots pour les boutons d'envoi
    void onSendMessageClicked();
    void onSendWizzClicked();
    
    // Ajoutons un slot pour traiter les paquets reçus (Messages/Wizz)
    // void onPacketReceived(const Packet &p); 

private:
    Ui::MainWindow *ui;
    QListWidget *m_messageList;
    QLineEdit *m_messageInput;
    QPushButton *m_sendMessageButton;
    QPushButton *m_sendWizzButton;
    QPropertyAnimation *m_vibrationAnimation;
    QProcess *m_soundProcess;
    
    // Pointeurs vers nos modules de gestion
    ServerManager *m_server = nullptr; 
    ClientManager *m_client = nullptr;
    ChatController *m_chatController = nullptr; 
};

#endif // MAINWINDOW_H