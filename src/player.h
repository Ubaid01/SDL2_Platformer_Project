#ifndef PLAYER_H
#define PLAYER_H

struct Vector2D {
    int x;
    int y;
};

class Player {
public:
    Player(double x, double y, int width, int height);

    double getX() const { return x ; }
    double getY() const { return y ; }
    void setX(double x) { this->x = x; }
    void setY(double y) { this->y = y; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isOnPlatform() const { return onPlatform; }
    void setOnPlatform(bool result) { onPlatform = result; }
    
    void setVelocity(double x, double y) 
        { velocity = { static_cast<int>(x), static_cast<int>(y) }; }

    Vector2D getVelocity() const { return velocity; }

    void updatePosition( double gravity );
    
private:
    double x;
    double y;
    int width;
    int height;
    bool onPlatform;
    Vector2D velocity;

};

#endif // PLAYER_H
