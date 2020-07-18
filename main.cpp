#include <iostream>
#include "Models/Models.h"
#include "Test/ItemPacket.h"
#include "Hooks/Hook.h"

void StartGame(const char *dir);

int main(int argc, char *argv[]) {


    std::string D2Directory;

    for(int i=1;i<argc;i++) {
        if (i+1<argc) {
            if (strcmp(argv[i],"-d") == 0) {
                D2Directory = argv[i+1];
            }
        }
    }

    StartGame(D2Directory.c_str());

    Hooks::init(argc, argv);


    return 0;
}
