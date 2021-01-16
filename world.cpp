#include "world.hpp"
#include "game.hpp"
#include <cstdlib>
#include <iostream>
#include <algorithm>

//finite state machine 
world::world(sf::RenderWindow& w, game *g_):
    window(w), 
    g(g_),
    queue(),
    network()
{
    srand(time(0));
    window.setFramerateLimit(60);
    setup();
    buildersSoFar = 0;
}

void world::setup()
{
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            tiles[i][j].pos = sf::Vector2i(i,j);
            tiles[i][j].level = 0;
        }
    }
    turn = 0;
}

world::~world()
{
}

bool world::noBuilder(const sf::Vector2i& pos)
{
    //iterating over the builders and checking if there are any
    for (builder& builders)
    {
        if (b.pos == pos)
            return false;
    }
    return true;
}

bool world::noDome(const sf::Vector2i& pos)
{
    return tiles[pos.x][pos.y].level < 4;
}

bool world::nearSelectedBuilder(const sf::Vector2i& pos)
{
    //calculate the difference between the selected builder and the position
    sf::Vector2i diff = pos - builders[selectedBuilder].pos;

    //if diff.x and diff.y are between -1 and 1 and not 0 at the same time then we're near the selected builder
    return (diff.x >= -1 && diff.y >= -1 
         && diff.x <= 1 && <= 1 &&
            !(diff.x ==0 && diff.y == 0));
}

int world::levelDifference(const sf::Vector2i& pos)
{
    sf::Vector2i bpos = builders[selectedBuilder].pos;
    return tiles[pos.x][pos.y].level - tiles[bpos.x][bpos.y].level;

}

void world::build(const sf::Vector2i& pos)
{
    //Check if there is no building, near the builder and no dome

    if (noBuilder(pos) && nearSelectedBuilder(pos) && noDome(pos))
    {
        MsgPos msg(MsgType::Build, id, pos);
        //tiles[pos.x][pos.y].build();
        //state = WorldState::Select;
        //turn++;
    }
   
}

void world::move(const sf::Vector2i& pos)
{
    //Check if on a builder and is one of the same colour we select and return
    if (!noBuilder(pos))
    {
        for (size_t i = 0; i < builders.size(); i++)
        {
            // if the builders position is the same as the one that was clicked and the turn is one of their players
            if (builders[i].pos == pos && turn % 2 == builder[i].player)
            {
                //changes the clicked builder
                //selectedBuilderIndex = i
                MsgSelect msg(id, i);
                network.send(msg);
                return;
            }
        }
    }
    
    //Check if there isn't a builder, not a dome, near a selected builder and can't climb more than one floor
    if (noBuilder(pos) && nearSelectedBuilder(pos) && noDome(pos) && levelDifference(pos) <= 1)
    {
        sf::Vector2i prevPos = builders[selectedBuilderIndex].pos;

        if (tiles[prevPos.x][prevPos.y].level == 2 && tiles[pos.x][pos.y].level == 3)
        {
            //we check if the local player has won the game
            if (turn % 2 == 0)
                g->state = gamestate::victory;
            else
                g->state = gamestate::defeat;
            
        }
        sf::Vector2i m = pos - builders[selectedBuilderIndex].pos;
        MsgPos msg(MsgType::Move, id, m);
        network.send(msg);
        //builders[selectedBuilder].move(m);
        //state = WorldState::Build;
    }
}

void world::place(const sf::Vector2i& pos)
{
    //Check if there is no builder
    if (noBuilder(pos))
    {
        network.send(MsgPos msg(MsgType::Place, id, pos));
        //the first player will have an ID of 0, the second will have 1 etc
        //builders.push_back(builder(pos.x, pos.y, buildersSoFar / 2));
        //builderSoFar++;

        //if (buildersSoFar == 4)
        //    state = WorldState::Select;
    }
    
}

void world::select(const sf::Vector2i& pos)
{
    //Check if the player is correct 
    for (size_t i = 0; i < builders.size(); i++)
    {
        if ((builders[i].pos == pos) && (turn % 2 == builders[i].player))
        {
                network.send(MsgSelect(id, i));
                //selectedBuilder = i;
                //state = WorldState::Move;
                return;     
        }
    }
}

void world::processPlace(const Msg& msg)
{
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    sf::Vector2i pos(m_pos.x, m_pos.y);
    builders, push_back(builder(pos.x, pos.y, buildersSoFar / 2));
    buildersSoFar++;
    if (buildersSoFar == 4)
        state = WorldState::Select;
}

void world::processSelect(const Msg& msg)
{
    MsgSelect m_select;
    std::memcpy(&m_select, &msg, sizeof(MsgSelect));
    selectedBuilderIndex = static_cast<int>(m_select.builderID);
    state = WorldState::Move;
}

void world::processMove(const Msg& msg)
{
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    sf::Vector2i pos(m_pos.x, m_pos.y);
    builders[selectedBuilderIndex].move(pos);
    state = WorldState::Build;
}

void world::processBuild(const Msg& msg)
{
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    tiles[m_pos.x][m_pos.y].build();
    state = WorldState::Select;
    turn++;
}

void world::update()
{
    static bool clicked = false;
   if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicked)
   {
        clicked = true;

        //size of the tiles are 120 so divided by it to get the correct tile
        sf::Vector2i position = sf::Mouse::getPosition(window);
        sf::Vector2i pos(position.x/120, position.y/120);
       
        switch (state)
        {
        case WorldState::build:
            build(pos);
            break;
        case WorldState::Move:
            move(pos);
            break;  
        case WorldState::Place:
            place(pos);
            break;
        case WorldState::Select:
            select(pos);
            break;
        default:
            break;
        } 
   } 
   else
   {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
             clicked = false;
   }
       
   //Input has been processed
   //Now for incoming messages
   Message m;
   queue.pop(m); //non blocking pop
   
   //If this is an empty packet the there is no need to process the message
   if (m.first.endOfPacket())
       return;

   //otherwise we need to process message
   Msg msgl
   readMsg(m, msg);
   //msg now contains the received message
   switch (msg.msgtype) {
       case MsgType::Register:
           std::cout << "MsgType::Register\n";
           break;
       case MsgType::Select:
           std::cout << "MsgType::Select\n";
           processSelect(msg);
           break;
       case MsgType::Move:
           std::cout << "MsgType::Move\n";
           processMove(msg);
           break;
       case MsgType::Build:
           std::cout << "MsgType::Build\n";
           processBuild(msg);
           break;
       case MsgType::Rand:
           std::cout << "MsgType::Rand\n";
           break;
       case MsgType::Place:
           std::cout << "MsgType::Place\n";
           processPlace(msg);
           break;
       default:
           std::cout << "MsgType::Undefined\n";
           break;
   }
}

void world::draw()
{
    window.clear();
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            tiles[i][j].draw(window);
        }
    }
    //for all of the builders will be placed in the world so for however many builders will be placed in the world
    //eg if there are no builders then none will be drawn
    for (builder& b : builders) {
        b.draw(window);
    }
}
