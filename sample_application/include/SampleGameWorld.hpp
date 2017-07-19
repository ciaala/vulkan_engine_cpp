//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEGAMEWORLD_HPP
#define VULKAN_ENGINE_CPP_SAMPLEGAMEWORLD_HPP

#include "game/GameWorld.hpp"

class SampleGameWorld : public vlk::GameWorld {
private:
public:
    SampleGameWorld();

private:
    vlk::Camera camera;
    std::vector<vlk::GameObject *> objects;
public:

    vlk::Camera *getCamera() override;

    void updateWorld() override;

    std::vector<vlk::GameObject *> getGameObjects() override;

};


#endif //VULKAN_ENGINE_CPP_SAMPLEGAMEWORLD_HPP
