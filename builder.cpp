#include "builder.hpp"
#include <iostream>

builder::builder(unsigned short x_, unsigned short y_, unsigned short p_):
    pos(x_,y_),
    player(p_)
{
}

void builder::draw(sf::RenderWindow &window)
{
    sf::CircleShape circle(40, 40);
    sf::Color fColor = sf::Color(0xa0, 0x50, 0x50, 0xff);
    if (player == 1) {
        fColor = sf::Color(0x50, 0x50, 0xa0, 0xff);
    }
    circle.setFillColor(fColor);
    circle.setPosition(pos.x * 120 + 20, pos.y * 120 + 20);
    window.draw(circle);
}

void builder::move(sf::Vector2i m)
{
    pos += m;
}
