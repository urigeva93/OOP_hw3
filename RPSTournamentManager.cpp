#include "RPSTournamentManager.h"

bool comp (pair<string, atomic<int>> elem1, pair<string, atomic<int>> elem2) {
    return (elem1.second > elem2.second);
}

RPSTournamentManager &RPSTournamentManager::getTournamentManager() {
    return m_the_tournament_manager;
}

void RPSTournamentManager::registerAlgorithm(string id, function<unique_ptr<PlayerAlgorithm>()> factoryMethod) {

    //check if the id already exists
    if(m_id_factory.count(id) == 1) {
        cout << "WARNING: id " << id << " already registered to tournament. " << endl;
        return;
    }

    m_id_factory[id] = factoryMethod;
    m_num_players++;

}

void RPSTournamentManager::fillGamesQueue() {

    vector<pair<string, int>> pool;
    for(auto player : m_id_factory)
        pool.push_back(make_pair(player.first, 0));

    random_device rand_dev;
    mt19937 generator(rand_dev());
    int lower = 0, upper, player_1_index, player_2_index;

    while(pool.size() > 1) {

        upper = pool.size() - 1;
        uniform_int_distribution<int> distr(lower, upper);

        //random two indices to play
        player_1_index = distr(generator);
        while((player_2_index = distr(generator)) == player_1_index); //make sure the indices are different!

        this->m_games_queue.push(make_pair(pool.at(player_1_index).first, pool.at(player_2_index).first)); //enter to res games

        //update the players' num of games
        pool.at(player_1_index).second++;
        pool.at(player_2_index).second++;

        removeReadyPlayers(pool);
    }

    //in case one player left match him with the first player
    if(pool.size() == 1)
        this->m_games_queue.push(make_pair(pool.at(0).first, this->m_games_queue.front().first));

}

void RPSTournamentManager::removeReadyPlayers(vector<pair<string, int>>& pool) {
    auto elems_ro_remove = remove_if(pool.begin(), pool.end(), [&](pair<string, int>& curr){return curr.second >= NUM_OF_GAMES_PER_PLAYER;});
    pool.erase(elems_ro_remove, pool.end());
}

void RPSTournamentManager::launchTournament() {

    vector<thread> threads_pool;
    for(int i =0; i < NUM_OF_THREAD; i++)
        threads_pool.push_back(thread(threadWork));


    for(thread& t : threads_pool)
        t.join();

}

void RPSTournamentManager::threadWork() {


    int winner;
    while(this->m_games_queue.empty() == false) {

        pair<string, string> game;
        {
            lock_guard<mutex> lock(queue_lock);
            game = this->m_games_queue.pop();
        }

        auto algo_player1 = this->m_id_factory[game.first];
        auto algo_player2 = this->m_id_factory[game.second];

        unique_ptr<RPSGame> game_manager = make_unique<RPSGame>(algo_player1, algo_player2);
        game_manager->initGame();
        game_manager->playGame();
        game_manager->endGame();


        //TODO: what if game metzik

        //update points according to winner
        winner = game_manager->getWinner();
        if(winner == PLAYER_1)
            this->m_id_points[game.first] += WIN_POINTS;
        else if(winner == PLAYER_2)
            this->m_id_points[game.second] += WIN_POINTS;
        else { //winner == TIE
            this->m_id_points[game.first] += TIE_POINTS;
            this->m_id_points[game.second] += TIE_POINTS;
        }
    }
}

void RPSTournamentManager::printResults() {

    vector<pair<string, int>> vec_results(this->m_id_points.begin(), this->m_id_points.end());
    //sort the id_2_points by the second value (the player's points)
    sort(vec_results.begin(), vec_results.end(), comp);

    cout << "The results of this tournaments are: " << endl;
    for (int i = 0 ; i < vec_results.size(); i++) {
        cout << vec_results[i].first << " " << vec_results[i].second << endl;
    }

}
