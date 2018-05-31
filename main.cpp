#include <iostream>
#include <string.h>
#include <stdio.h>
#include "RPSTournamentManager.h"


int NUM_OF_THREAD;
string PATH_DIR_ALGORITHMS;

bool handleFlagAndData(char* flag, char* data) {

    string string_data(data);
    //flag 'threads'
    if(strcmp(flag, "-threads") == 0) {
        if(RPSPlayerFromFile::isStringRepInt(string_data) == false)
            return false;

        NUM_OF_THREAD = stoi(data);
        return true;
    }

    //flag 'path'
    if(strcmp(flag, "-path")) {
        PATH_DIR_ALGORITHMS = string_data;
        return true;
    }

    return false;
}

//if the CLA was legal returns true and set params accordingly, false o/w
bool parseCLA(int argc, char *argv[]) {

    if (argc == 1)
        return true;

    if (argc == 3)
        return handleFlagAndData(argv[1], argv[2]);

    if (argc == 5)
        return handleFlagAndData(argv[1], argv[2]) && handleFlagAndData(argv[3], argv[4]);

    return false;

}

int main(int argc, char *argv[]) {

    NUM_OF_THREAD = NUM_OF_THREADS_DEFAULT;
    PATH_DIR_ALGORITHMS = PATH_DIR_ALGORITHMS_DEFAULT;

    if(parseCLA(argc, argv) == false) {
        cout << "USAGE: " << argv[0] << " [-threads <num_threads>] [-path <location_of_algorithms>]" << endl;
        return EXIT_FAILURE;
    }

    //GO TO DIR AND OPEN IT
    //IF FAILS, EXIT FAILURE WITH USAGE

    //LOAD THE SO FILES

    RPSTournamentManager::getTournamentManager().fillGamesQueue(); //prepare games
    RPSTournamentManager::getTournamentManager().launchTournament(); //launch tour'
    RPSTournamentManager::getTournamentManager().printResults(); //print tour' results
    return EXIT_SUCCESS;
    
}