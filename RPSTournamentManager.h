#ifndef OOP_HW3_RPSTOURNAMENTMANAGER_H
#define OOP_HW3_RPSTOURNAMENTMANAGER_H

#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <set>
#include <map>
#include <functional>
#include <atomic>
#include <thread>
#include <random>
#include <queue>
#include <utility>
#include "RPSGame.h"
#include "AlgorithmRegistration.h"

#define NUM_OF_THREADS_DEFAULT 4
#define PATH_DIR_ALGORITHMS_DEFAULT '.'
#define NUM_OF_GAMES_PER_PLAYER 30
#define WIN_POINTS 3
#define TIE_POINTS 1

using namespace std;

extern mutex queue_lock;
extern condition_variable cond;
extern bool finished;

class RPSTournamentManager {

private:

    static RPSTournamentManager m_the_tournament_manager;
    map<string, function<unique_ptr<PlayerAlgorithm>()>> m_id_factory;
    map<string, atomic <int>> m_id_points;
    queue<pair<string, string>> m_games_queue;
    int m_num_players;
    RPSTournamentManager() {} //private ctor
    bool last_game;
    pair <string, string> last_special_game;



    void runSpecialGame(pair<string, string> game);
    void removeReadyPlayers(vector<pair<string, int>>& queue); //helper function for prepareGames() function
    void threadWork(bool with_lock);

public:

    void fillGamesQueue();
    void launchTournament();
    void printResults();
    static RPSTournamentManager& getTournamentManager();
    void registerAlgorithm(string id, function<unique_ptr<PlayerAlgorithm>()> factoryMethod);

};


#endif //OOP_HW3_RPSTOURNAMENTMANAGER_H
