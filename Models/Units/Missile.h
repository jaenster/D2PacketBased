//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_MISSILE_H
#define D2PacketBased_MISSILE_H
namespace Models {
    class Missile : public Unit<Missile>, public Locatable {


        Missile() {
            this->base = this;
            this->type = UnitType::Tile;
        }
    };
}
#endif //D2PacketBased_MISSILE_H
