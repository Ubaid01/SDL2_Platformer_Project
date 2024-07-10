#include "Entity.h"

Entity::Entity(float p_x, float p_y, std::vector<SDL_Texture*> p_tex)
    : x(p_x), y(p_y), tex(p_tex)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    SDL_QueryTexture(tex.at(0), nullptr, nullptr, &currentFrame.w, &currentFrame.h);
    for (int i = 0; i < getSize(); i++)
    {
        animOffsetsX.push_back(0);
        animOffsetsY.push_back(0);
    }
}

Entity::Entity(float p_x, float p_y, SDL_Texture* p_tex)
    : x(p_x), y(p_y)
{
    tex.push_back(p_tex);
    currentFrame.x = 0;
    currentFrame.y = 0;
    SDL_QueryTexture(p_tex, nullptr, nullptr, &currentFrame.w, &currentFrame.h);
    for (size_t i = 0; i < getSize(); i++)
    {
        animOffsetsX.push_back(0);
        animOffsetsY.push_back(0);
    }
}

//void Entity::setTex(SDL_Texture* p_tex)
//{
//    tex[0] = p_tex;
//}
