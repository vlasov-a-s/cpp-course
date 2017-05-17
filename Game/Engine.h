/*!
    @file Engine.h
    @brief Класс игрового движка
    @author Власов Александр, Татьяна Мамонтова, Алена Бескровная
    @date Май 2017
*/

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>
#include <limits>

#include "GameObjects\GameObject.h"
#include "GameObjects\Player.h"
#include "GameObjects\Food.h"
#include "Logger.h"
#include "GameLogic.h"
#include "Randomizer.h"
#include "ObjectsFactory.h"
#include "Utility.h"

namespace thirtythree {

class Engine {
public:

    typedef std::vector<std::unique_ptr<GameObject>>::iterator obj_iterator;

    //! Инициализация движка
    //! @param mode Размер окна
    //! @param name Заголовок окна
    //! @param map_size Размер игровой карты
    Engine(sf::VideoMode mode, const sf::String name, sf::Vector2i map_size);

    //! Добавление игрового объекта в движок
    void AddObject(GameObject *object);

    //! Инициализация игрового процесса
    void StartGame();

    //! Возвращает текущее число игровых объектов
    size_t GetObjectsCount() { return objects_.size(); }
    size_t GetObjectsLimit() { return max_object_number_; }

    //! Возвращает размер карты
    sf::Vector2f GetMapSize() { return (sf::Vector2f)map_.getSize(); }

    //! Возвращает размер окна
    sf::Vector2f GetWindowSize() { return (sf::Vector2f)window_.getSize(); }

    //! Возвращает указатель на окно
    sf::RenderWindow* GetWindow() { return &window_; }

private:

    //! Максимальное число объектов, обрабатываемых движком
    static const size_t max_object_number_ = 1000;

    //! Главный игровой цикл
    void GameLoop();
    void RestartGame();

    void HandleEvents();
    void HandleObject(GameObject &obj);
    void HandleBorderCollisions(GameObject &obj);
    void HandleDeadObjects();

    inline float CalculateDistance(obj_iterator obj1, obj_iterator obj2) {
        return length((*obj2)->GetPos() - (*obj1)->GetPos());
    }
    inline bool ObjectsAreAlive(obj_iterator obj1, obj_iterator obj2) {
        return !(*obj1)->IsDead() && !(*obj2)->IsDead();
    }
    inline bool Collision(obj_iterator obj1, obj_iterator obj2, float distance) {
        auto radius1 = (*obj1)->GetRadius();
        auto radius2 = (*obj2)->GetRadius();
        if (distance <= std::max(radius1, radius2)) {
            return true;
        }
        return false;
    }
    inline bool ObjectsAreInteractable(obj_iterator obj1, obj_iterator obj2) {
        return (*obj1)->IsInteractable() && (*obj2)->IsInteractable();
    }
    inline bool ObjectsIsInteractable(obj_iterator obj) {
        return (*obj)->IsInteractable();
    }

    void DrawUI();
    void DrawDebugInfo();
    void DrawText(const std::string &name, int size, const sf::Vector2i &pos,
                  sf::RenderTarget &screen, sf::Color color = sf::Color::Black);

    void DrawTextCentering(const std::string &name, int size, const sf::Vector2i &pos,
                  sf::RenderTarget &screen, sf::Color color = sf::Color::Black);

    //! Хранилище игровых объектов
    std::vector<std::unique_ptr<GameObject>> objects_;

    //! Окно, в которое производится отрисовка
    sf::RenderWindow window_;

    //! Игровая карта
    sf::RenderTexture map_;

    //! Камера для перемещения по игровой карте
    sf::View view_;

    //! Камера для вывода на экран интерфейса
    sf::View default_view_;

    //! Шрифт для отрисовки надписей
    sf::Font font_;

    //! Класс, предоставляющий возможности генерации случайных величин
    Randomizer rand_;

    //! Класс, реализующий игровую логику
    GameLogic logic_;

    //! Время прохождения одного игрового цикла
    float time_;
    sf::Clock clock_;

    //! Вывод отладочной информации
    bool draw_debug_info_ = true;

    bool game_over_ = false;

};

}

#endif // ENGINE_H_INCLUDED
