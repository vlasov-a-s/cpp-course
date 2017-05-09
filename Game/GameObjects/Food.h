#ifndef FOOD_H_INCLUDED
#define FOOD_H_INCLUDED

#include "GameObject.h"
#include "../Randomizer.h"

namespace thirtythree {

class Food : public GameObject {
public:

    Food() : GameObject() {}
    Food(const sf::Vector2f &pos, Randomizer *rand, float friction = 0.01f);

    ~Food();
    void Logic(const sf::Vector2u &map_size);

    std::string GetType() { return "FOOD"; }

private:

    static const int min_radius_ = 5;
    static const int max_radius_ = 20;
    static const int max_pinch_speed_ = 100;
    static const int min_seconds_idle_ = 3;
    static const int min_seconds_alive_ = 60;
    sf::Clock clock_pinch_;
    sf::Clock clock_life_;

    Randomizer *rand_;
};

}



#endif // FOOD_H_INCLUDED