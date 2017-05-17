/*!
    @file GameLogic.cpp
    @brief Класс игровой логики
    @author Власов Александр, Татьяна Мамонтова, Алена Бескровная
    @date Май 2017
*/

#include "Engine.h"
#include "GameLogic.h"

namespace thirtythree {

GameLogic::GameLogic(Engine *engine, Randomizer *rand)
    : engine_ (engine),
      rand_ (rand),
      factory_ (engine, rand, this),
      score_ (0) {
    clock_food_create_.restart();
    clock_enemy_create_.restart();
    LOG_INFO("Game logic unit initialized");
}

GameLogic::~GameLogic(){
    LOG_INFO("Game logic unit destroyed");
}

void GameLogic::StartGame() {
    auto map_size = engine_->GetMapSize();
    score_ = 0;
    int num_food = rand_->UniformInt(30 * map_size.x / 2500, 60 * map_size.y / 2500);
    for (int i = 0; i < num_food; i++) {
        engine_->AddObject(factory_.CreateFood());
    }
    int num_enemies = rand_->UniformInt(2 * map_size.x / 2500, 5 * map_size.y / 2500);
    for (int i = 0; i < num_enemies; i++) {
        engine_->AddObject(factory_.CreateEnemy());
    }
    engine_->AddObject(factory_.CreatePlayer(player_initial_radius_));
}

void GameLogic::DoLogic() {
    auto map_size = engine_->GetMapSize();
    if (clock_food_create_.getElapsedTime().asSeconds() > min_food_create_interval_) {
        int num_obj = rand_->UniformInt(1 * map_size.x / 2500, 5 * map_size.y / 2500);
        for (int i = 0; i < num_obj; i++) {
            engine_->AddObject(factory_.CreateFood());
        }
        clock_food_create_.restart();
    }

    if (clock_enemy_create_.getElapsedTime().asSeconds() > min_enemy_create_interval_) {
        int num_obj = rand_->UniformInt(0 * map_size.x / 2500, 5 * map_size.y / 2500);
        for (int i = 0; i < num_obj; i++) {
            engine_->AddObject(factory_.CreateEnemy());
        }
        clock_enemy_create_.restart();
    }

}

void GameLogic::CollideBoth(GameObject &obj1, GameObject &obj2) {
    if (!OnCollide(obj1, obj2)) {
        OnCollide(obj2, obj1);
    }
}

void GameLogic::InteractBoth(GameObject &obj1, GameObject &obj2) {
    if (!Interact(obj1, obj2)) {
        Interact(obj2, obj1);
    }
}

bool GameLogic::OnCollide(GameObject &obj1, GameObject &obj2) {
    auto obj1_type = obj1.GetType();
    auto obj2_type = obj2.GetType();
    auto obj1_radius = obj1.GetRadius();
    auto obj2_radius = obj2.GetRadius();
    bool obj1_is_player_or_enemy = (obj1_type == PLAYER || obj1_type == ENEMY);

    if (obj1_is_player_or_enemy && obj2_type == FOOD) {
        obj2.Kill();
        obj1.SetRadius(CalcNewRadius(obj1_radius, obj2_radius));
        if (obj1_type == PLAYER) UpdateScore(obj1_radius); // side effect
        return true;
    }

    if (obj1_is_player_or_enemy && obj2_type == ENEMY) {
        if (obj1_radius > obj2_radius) {
            obj2.Kill();
            obj1.SetRadius(CalcNewRadius(obj1_radius, obj2_radius));
            if (obj1_type == PLAYER) UpdateScore(obj1_radius);
        } else {
            obj1.Kill();
            obj2.SetRadius(CalcNewRadius(obj1_radius, obj2_radius));
        }
        return true;
    }

    return false;
}

bool GameLogic::Interact(GameObject &obj1, GameObject &obj2) {
    auto obj1_type = obj1.GetType();
    auto obj2_type = obj2.GetType();
    auto obj1_radius = obj1.GetRadius();
    auto obj2_radius = obj2.GetRadius();
    auto obj1_pos = obj1.GetPos();
    auto obj2_pos = obj2.GetPos();
    float obj1_speed = obj1.GetMaxSpeed();
    float obj2_speed = obj2.GetMaxSpeed();

    bool obj1_is_player_or_enemy = (obj1_type == PLAYER || obj1_type == ENEMY);

    if (obj1_is_player_or_enemy && obj2_type == ENEMY) {
        auto distance = length(obj2_pos - obj1_pos) - std::max(obj1_radius, obj2_radius);
        auto direction = normalize(obj2_pos - obj1_pos);
        if (distance <= 0) distance = 1;
        float reaction = rand_->UniformInt(1, 10);
        auto speed_factor = direction /(float)sqrt(distance) / reaction;
        if (obj1_radius > obj2_radius) {
            if (obj1_type != PLAYER) {
                obj1.AddSpeed(obj1_speed * speed_factor);
            }
            obj2.AddSpeed(obj2_speed * speed_factor);
        } else {
            if (obj1_type != PLAYER) {
                obj1.AddSpeed(- obj1_speed * speed_factor);
            }
            obj2.AddSpeed(- obj2_speed * speed_factor);
        }
        return true;
    }

    return false;
}

}
