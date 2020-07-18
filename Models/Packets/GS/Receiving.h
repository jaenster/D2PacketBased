//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_RECEIVING_H
#define D2PacketBased_RECEIVING_H

#include "./ServerClient.h"
#include "../../../Tools/Compression.h"
#include "Identifiers.h"

namespace GameServer {
    class Receiving {
        static word getSize(uchar *data, dword size) {
            static char packetLengths[] = {1, 8, 1, 12, 1, 1, 1, 6, 6, 11, 6, 6, 9, 13, 12, 16, 16, 8, 26, 14, 18, 11,
                                           -1, 0, 15, 2, 2, 3, 5, 3, 4, 6, 10, 12, 12, 13, 90, 90, -1, 40, 103, 97, 15,
                                           0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 8, 13, 0, 6, 0,
                                           0, 13, 0, 11, 11, 0, 0, 0, 16, 17, 7, 1, 15, 14, 42, 10, 3, 0, 0, 14, 7, 26,
                                           40, -1, 5, 6, 38, 5, 7, 2, 7, 21, 0, 7, 7, 16, 21, 12, 12, 16, 16, 10, 1, 1,
                                           1, 1, 1, 32, 10, 13, 6, 2, 21, 6, 13, 8, 6, 18, 5, 10, 0, 20, 29, 0, 0, 0, 0,
                                           0, 0, 2, 6, 6, 11, 7, 10, 33, 13, 26, 6, 8, -1, 13, 9, 1, 7, 16, 17, 7, -1,
                                           -1, 7, 8, 10, 7, 8, 24, 3, 8, -1, 7, -1, 7, -1, 7, -1, 0, -1, -1, 1, 0, 53,
                                           -1, 5,};

            const uchar identifier = data[0];
            if (identifier > 0xB5 || size == 0) return 0;

            switch (identifier) {
                case PlayerInGame:
                case UpdateItemStats:
                case Unknown0x16:
                    return size > 1 ? data[1] : 0;
                case GameChat: {
                    const auto *bytes = reinterpret_cast<const unsigned char *>(data);

                    const char *name = reinterpret_cast<const char *>(bytes + 10);
                    int nameLength = static_cast<int>(strlen(name));

                    const char *message = reinterpret_cast<const char *>(bytes + 10 + nameLength + 1);
                    int messageLength = static_cast<int>(strlen(message));
                    return 10 + nameLength + 1 + messageLength + 1;
                }
                case BaseSkillLevels:
                    return size > 1 ? data[1] * 3 + 6 : 0;
                case ItemActionOwned:
                case ItemActionWorld:
                    return size > 2 ? data[2] : 0;

                case WardenRequest:
                case Unknown0x6A:
                    return size > 3 ? *reinterpret_cast<word *>(data + 2) : 0;

                case SetState:
                case AddUnit:
                    return size > 6 ? data[6] : 0;

                case AssignNPC:
                    return size > 12 ? data[12] : 0;
                case ConnectionInfo:
                    return 2;
                default:
                    const uchar defaultSize = packetLengths[identifier];
                    if (defaultSize > 0) return defaultSize;

            }
            // Cant come here as switch always returns a value thanks to the default value, but this shuts up the editor
            return 0;
        }

    protected:
        static void DecompressedRecvData(uchar *buffer, dword size, Models::Game *game) {
            dword offset = 0;
            while (offset < size) {
                int curSize = getSize(buffer + offset, size - offset);

                if (curSize <= 0) {
                    std::cout << offset << std::endl;
                    std::cout << "FAILED" << std::endl;
                    break;
                }

                if (curSize < 0 || size - offset < 0) {
                    std::cout << "Error: Unexpected end of server data encountered" << std::endl;
                    break;
                }

                ServerClient((uchar *) buffer + offset, curSize, game);

                offset += curSize;
            }
        }

    public:
        static void data(uchar *data, dword size, Models::Game *game) {

            if (!game->world->compressed) {
                Receiving::DecompressedRecvData(data, size, game);
            } else {
                int compressedSize = Compression::GetPacketSize(data, size);
                if (compressedSize <= size) {
                    char buffer[2048];

                    int decompressedSize = Compression::DecompressPacket(data, compressedSize, buffer, sizeof(buffer));

                    if (decompressedSize > 0) {
                        Receiving::DecompressedRecvData(data, size, game);

                    }

                }
            }

        };
    };
};

#endif //D2PacketBased_RECEIVING_H
