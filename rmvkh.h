#ifndef RMVKH_H
#define RMVKH_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot {

class RMVKH : public RefCounted {
    GDCLASS(RMVKH, RefCounted)

protected:
    static void _bind_methods();

public:
    RMVKH();
    ~RMVKH();

    // Encode : position + rotation (degrés) + etat + arme -> paquet d'octets
    PackedByteArray encoder(Vector3 position, float rotation_degres, int etat, bool arme);

    // Decode : paquet d'octets -> Dictionary avec position, rotation, etat, arme
    Dictionary decoder(PackedByteArray paquet);
};

}

#endif