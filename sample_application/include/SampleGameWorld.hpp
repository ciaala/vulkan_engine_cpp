//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEGAMEWORLD_HPP
#define VULKAN_ENGINE_CPP_SAMPLEGAMEWORLD_HPP

#include "game/GameWorld.hpp"
#include "Engine.hpp"

class SampleGameWorld : public vlk::GameWorld {
private:
    vlk::Camera camera;

    std::vector<vlk::GameObject *> objects;

    vlk::Engine *engine;

public:
    explicit SampleGameWorld(vlk::Engine *engine);

    vlk::Camera *getCamera() override;

    void updateWorld() override;

    std::vector<vlk::GameObject *> getGameObjects() override;

};


#endif //VULKAN_ENGINE_CPP_SAMPLEGAMEWORLD_HPP
