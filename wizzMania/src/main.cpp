#include "gui/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QInputDialog> // Ajouté pour la boîte de dialogue de choix

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // --- Logique de traduction (générée par Qt) ---
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "wizzMania_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // --- Choix du mode : Serveur ou Client ---
    QStringList options;
    options << "Lancer le Serveur" << "Lancer le Client";
    
    bool ok;
    QString item = QInputDialog::getItem(nullptr, 
                                         "Wizz Mania - Mode", 
                                         "Comment souhaitez-vous lancer l'application ?", 
                                         options, 0, false, &ok);

    // Si l'utilisateur clique sur "Annuler" ou ferme la petite fenêtre
    if (!ok || item.isEmpty()) {
        return 0;
    }

    // --- Initialisation de la fenêtre principale ---
    MainWindow w;

    if (item == "Lancer le Serveur") {
        w.setWindowTitle("Wizz Mania [SERVEUR]");
        // Demander le port pour le serveur
        bool ok;
        int port = QInputDialog::getInt(nullptr, "Port du Serveur", "Entrez le port pour le serveur :", 1234, 1024, 65535, 1, &ok);
        if (!ok) return 0;
        w.setAsServer(port);
    } else {
        w.setWindowTitle("Wizz Mania [CLIENT]");
        // Demander l'IP et le port du serveur
        bool ok;
        QString ip = QInputDialog::getText(nullptr, "Connexion au Serveur", "Entrez l'adresse IP du serveur :", QLineEdit::Normal, "127.0.0.1", &ok);
        if (!ok || ip.isEmpty()) return 0;
        int port = QInputDialog::getInt(nullptr, "Connexion au Serveur", "Entrez le port du serveur :", 1234, 1024, 65535, 1, &ok);
        if (!ok) return 0;
        w.setAsClient(ip, port);
    }

    w.show();
    return a.exec();
}
