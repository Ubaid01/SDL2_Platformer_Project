#ifndef PLATFORM_H
#define PLATFORM_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define STARTING_GRAVITY 1.0

enum MotionTypes
{
	MT_Stationary , MT_Swaying , MT_Diagonal , MT_Parabolic , MT_MaxLimit = 5
};

enum LevelMode
{
    LM_EasyMode = 1, LM_AdventureMode
};
	
class Platform {
public:
    Platform() {}
    Platform( int index , int level_mode = LM_EasyMode );

    double getX() const { return x; }
    double getChangedX( ) const { return dx ; }
    double getChangedY() const { return dy; }
    double getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    bool getHasCoin() const { return hasCoin; }
    void setHasCoin(bool value) { hasCoin = value; }
    int getCoinX() const { return coinX; }
    int getCoinY() const { return coinY; }
    
	bool checkOverlap(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) 
		{	return (x1 < x2 + w2) && (x1 + w1 > x2) && (y1 < y2 + h2) && (y1 + h1 > y2);  }
    void block() { x -= 3; y -= 3 ; }
    bool isPlatformHaveMotion( MotionTypes type ) const { return currentMotion == type ; }
    
    void updatePosition( int level_mode );
    
private:
    double x;
    double y;
    double dx;
    double dy;
    double dt;
    float vx ;
    float vy ;
    MotionTypes currentMotion ;
    int width;
    int height;
    bool hasCoin;
    int coinX;
    int coinY;


};

#endif // PLATFORM_H
