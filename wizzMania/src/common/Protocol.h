#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <QDataStream>

// Enum pour les types de paquets (selon le plan Phase 1)
enum class PacketType {
    MESSAGE,  // Message texte normal
    WIZZ,     // Effet Wizz (vibration)
    JOIN,     // Un client rejoint
    LEAVE     // Un client part
};

// Struct pour représenter un paquet réseau
struct Packet {
    PacketType type;
    QString sender;    // Nom ou ID de l'expéditeur
    QString content;   // Contenu du message (vide pour WIZZ/JOIN/LEAVE si besoin)

    // Constructeurs
    Packet() = default;
    Packet(PacketType t, const QString &s, const QString &c = QString())
        : type(t), sender(s), content(c) {}

    // Sérialisation pour QDataStream (utilisé dans ServerManager)
    friend QDataStream &operator<<(QDataStream &out, const Packet &p) {
        out << static_cast<int>(p.type) << p.sender << p.content;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Packet &p) {
        int typeInt;
        in >> typeInt >> p.sender >> p.content;
        p.type = static_cast<PacketType>(typeInt);
        return in;
    }
};

#endif // PROTOCOL_H