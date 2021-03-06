#include <SFML/Graphics.hpp>
#include <array>
#include "tile.hpp"
#include "builder.hpp"

#ifndef WORLD_HPP
#define WORLD_HPP

class game;

//enum class for game states
enum class WorldState {

    Build,
    Move,
    Place,
    Select
};

class world {
    public:
        world(sf::RenderWindow& w, game *g);
        ~world();
        void update();
        void draw();
        void setup();
    private:
        game *g;
        sf::Uint32 id;
        sf::RenderWindow& window;
        std::array<std::array<tile, 5>, 5> tiles{{{{tile(), tile(), tile()}}, {{tile(), tile(), tile()}}, {{tile(), tile(), tile()}}}} ;
        std::vector<builder> builders;
        unsigned short turn;
        unsigned short buildersSoFar;
        std::size_t selectedBuilderIndex;
        WorldState state = WorldState::Place; 
        void build(const sf::Vector2i& pos);
        void move(const sf::Vector2i& pos);
        void place(const sf::Vector2i& pos);
        void select(const sf::Vector2i& pos);
        bool noBuilder(const sf::Vector2i& pos);
        bool noDome(const sf::Vector2i& pos);
        bool nearSelectedBuilder(const sf::Vector2i& pos);
        int levelDifference(const sf::Vector2i& pos);
        void processPlace(const Msg& msg);
        void processSelect(const Msg& msg);
        void processMove(const Msg& msg);
        void processBuild(const Msg& msg);
};
#endif
