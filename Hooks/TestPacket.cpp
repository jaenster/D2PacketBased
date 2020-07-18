
#include <iostream>
#include <fstream>
#include "Hook.h"

namespace Hooks::TestPacket {

    struct _ : Hook<void> {

        void init(int argc, char *argv[]) {
            std::string packetFile;
            bool found = false;

            for (int i = 1; i < argc; i++) {
                if (i + 1 < argc) {
                    if (strcmp(argv[i], "-testFile") == 0) {
                        packetFile = argv[i + 1];
                        found = true;
                        break;
                    }
                }
            }

            if (found) { // Load test file

                std::ifstream is(packetFile, std::ifstream::binary);

                if (!is) {
                    std::cout << "Failed to load the test file: " << packetFile << std::endl;
                    return;
                };

                std::cout << "Loading test file: " << packetFile << std::endl;

                is.seekg(0, std::ifstream::end);
                dword length = is.tellg();
                is.seekg(0, std::ifstream::beg);

                char *buffer = new char[length];

                std::cout << "Reading " << length << " characters... ";
                // read data as a block:
                is.read(buffer, length);

                Game *game = new Game();

                GameServer::Receiving::data(reinterpret_cast<uchar *>(buffer), length, game);

                delete[] buffer;

                std::cout << "Done Test" << std::endl;

            }
        }

    } self; // NOLINT(cert-err58-cpp) <-- stfu clion
}
