#ifndef OOP_HW3_ALGORITHMREGISTRATION_H
#define OOP_HW3_ALGORITHMREGISTRATION_H

#include <functional>
#include <memory>
#include <iostream>
#include "RPSTournamentManager.h"

#include "PlayerAlgorithm.h"


#define REGISTER_ALGORITHM(ID) \
AlgorithmRegistration register_me_##ID \
	(#ID, []{return std::make_unique<RSPPlayer_##ID>();} );

class AlgorithmRegistration {
public:
    AlgorithmRegistration(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
        RPSTournamentManager::getTournamentManager().registerAlgorithm(id, factoryMethod);
    }
};


#endif //OOP_HW3_ALGORITHMREGISTRATION_H
