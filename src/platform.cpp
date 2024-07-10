#include "Platform.h"
#include <cstdlib>
#include <cmath>
#include "player.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288 // To convert from radians to degrees M_PI / 180
#endif // M_PI

Platform::Platform( int index , int level_mode ) 

{
    width = 100;
    height = 32;
    if ( level_mode == LM_EasyMode )
        currentMotion = MT_Stationary ;
	else 
    {
        currentMotion = static_cast<MotionTypes>( rand() % MT_MaxLimit ) ;
		currentMotion = ( currentMotion > MT_Parabolic ) ? MT_Stationary : currentMotion ;
    }
    	
    dx = dy = dt = 0.0 ;
    vx = (rand() % 2 == 0) ? 1.25 : -1.25 ; 
    vy = 0.75 ;

    x = rand() % (SCREEN_WIDTH - width - 15) + 15 ;
    y = 0 - height - (index * 100);

    int coinInt = rand() % 4;

    hasCoin = ( coinInt > 0 ) && ( index != 0 );

    coinX = static_cast<int>(x + width / 2.0 - 12 );
    coinY = static_cast<int>(y - 24 - 5);
}

void Platform::updatePosition( int level_mode ) 
{
    y += 1 ;
    double previousX = x;
	double previousY = y - 1 ;

    dt += 0.05; 
    if ( currentMotion == MT_Swaying ) 
    {
    	x += sin(dt) * vx ; 
    	if (x < 0 || x + width > SCREEN_WIDTH)
        {
            vx = -vx;
            x += vx * sin(dt);
        }
        dx = x - previousX;
	}
	else if ( currentMotion == MT_Parabolic )
	{
		x += sin( dt ) * vx ;
		if (x < 0 || x + width > SCREEN_WIDTH)
        {
            vx = -vx;
            x += vx * sin(dt);
        }
		y += cos( dt ) * vy ;
		dx = x - previousX ;
        dy = y - previousY;
	}
	else if (currentMotion == MT_Diagonal)
    {
        x += vx;
        if ( x < 0 || x + width > SCREEN_WIDTH )
        {
            vx = -vx;
            x += vx; // move back within bounds
        }

        dx = x - previousX;
    }

    coinX = static_cast<int>(x + width / 2.0 - 12);
    coinY = static_cast<int>(y - 24 - 5);

    if ( y > SCREEN_HEIGHT ) 
	{
        if ( level_mode == LM_AdventureMode )
        {
            currentMotion = static_cast<MotionTypes>(rand() % MT_MaxLimit);
            currentMotion = (currentMotion > MT_Parabolic) ? MT_Stationary : currentMotion;
        }
        x = rand() % (SCREEN_WIDTH - width - 15) + 15 ;
        y = 0 - height;

        int coinInt = rand() % 4;
        hasCoin = (coinInt > 0);
    }
}
