#include "rmvkh.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void RMVKH::_bind_methods() {
    ClassDB::bind_method(D_METHOD("encoder", "position", "rotation_degres", "etat", "arme"), &RMVKH::encoder);
    ClassDB::bind_method(D_METHOD("decoder", "paquet"), &RMVKH::decoder);
}

RMVKH::RMVKH() {}
RMVKH::~RMVKH() {}

// --- ENCODER ---
PackedByteArray RMVKH::encoder(Vector3 position, float rotation_degres, int etat, bool arme) {
    PackedByteArray paquet;
    paquet.resize(7);

    // Position X et Z : 10 unites par metre, sur 2 octets (0 a 65535)
    int x_encode = (int)((position.x) * 10.0);
    int z_encode = (int)((position.z) * 10.0);
    // Position Y : 10 unites par metre, sur 1 octet (0 a 255 -> jusqu'a 25.5m)
    int y_encode = (int)((position.y) * 10.0);
    if (y_encode < 0) y_encode = 0;
    if (y_encode > 255) y_encode = 255;

    paquet[0] = (x_encode >> 8) & 0xFF;  // X octet haut
    paquet[1] = x_encode & 0xFF;         // X octet bas
    paquet[2] = (z_encode >> 8) & 0xFF;  // Z octet haut
    paquet[3] = z_encode & 0xFF;         // Z octet bas
    paquet[4] = y_encode & 0xFF;         // Y

    // Rotation : pas de 5 degres -> 0 a 71
    int rotation_encode = (int)(rotation_degres / 5.0) % 72;
    paquet[5] = rotation_encode & 0xFF;

    // Etat (0-8) sur les 4 premiers bits, arme sur le 5e bit
    int octet_etat = (etat & 0x0F) | ((arme ? 1 : 0) << 4);
    paquet[6] = octet_etat & 0xFF;

    return paquet;
}

// --- DECODER ---
Dictionary RMVKH::decoder(PackedByteArray paquet) {
    Dictionary resultat;
    if (paquet.size() < 7) {
        return resultat; // paquet invalide
    }

    int x_encode = (paquet[0] << 8) | paquet[1];
    int z_encode = (paquet[2] << 8) | paquet[3];
    int y_encode = paquet[4];

    Vector3 position;
    position.x = x_encode / 10.0;
    position.z = z_encode / 10.0;
    position.y = y_encode / 10.0;

    float rotation_degres = paquet[5] * 5.0;

    int octet_etat = paquet[6];
    int etat = octet_etat & 0x0F;
    bool arme = (octet_etat & 0x10) != 0;

    resultat["position"] = position;
    resultat["rotation"] = rotation_degres;
    resultat["etat"] = etat;
    resultat["arme"] = arme;

    return resultat;
}