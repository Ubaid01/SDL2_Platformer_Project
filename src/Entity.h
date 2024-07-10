#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

class Entity {
public:
    Entity(float p_x, float p_y, std::vector<SDL_Texture*> p_tex);
    Entity(float p_x, float p_y, SDL_Texture* p_tex);
    
    float getX() const { return x; }
    float getY() const { return y; }
    int getWidth() const { return currentFrame.w; }
    int getHeight() const { return currentFrame.h; }
    int getSize() const { return tex.size(); }
    float getAnimOffsetX(int p_index) const { return animOffsetsX.at(p_index); }
    float getAnimOffsetY(int p_index) const { return animOffsetsY.at(p_index); }
    SDL_Texture* getTex(int p_index) const { return tex.at(p_index); }
    SDL_Rect getCurrentFrame() const { return currentFrame; }

    void setX(float p_x) { x = p_x; }
    void setY(float p_y) { y = p_y; }
    void setAnimOffsetX(int p_index, int p_value) { animOffsetsX[p_index] = p_value; }
    void setAnimOffsetY(int p_index, int p_value) { animOffsetsY[p_index] = p_value; }
    
private:
    float x, y;
    std::vector<float> animOffsetsX;
    std::vector<float> animOffsetsY;
    SDL_Rect currentFrame;
    std::vector<SDL_Texture*> tex;

};

#endif // ENTITY_H
