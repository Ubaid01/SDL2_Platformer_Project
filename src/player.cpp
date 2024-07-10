#include "Player.h"
#include "platform.h"

Player::Player(double x, double y, int width, int height)
    : x(x), y(y), width(width), height(height), onPlatform(false), velocity{ 0, 0 } {}

void Player::updatePosition( double gravity ) {
    x += velocity.x;
    y += velocity.y;
    
    if (!isOnPlatform())
        velocity.y += gravity;
    else
        velocity = { 0, 0 };

    if ( ( x < 0 ) || ( x + width > SCREEN_WIDTH ) )
        velocity.x *= -1;
}
