//
// Created by crypt on 19/07/17.
//

#include <game/GameObject.hpp>
#include <vector>
#include <iostream>
#include <SampleObject.hpp>
#include <Engine.hpp>
#include "SampleGameWorld.hpp"
#include "resource/ResourceModel.hpp"

std::vector<vlk::GameObject *> SampleGameWorld::getGameObjects() {
    return this->objects;
}

void SampleGameWorld::updateWorld() {
    static bool once = false;
    if (!once) {
        std::cout << "Update the world" << std::endl;
    }
    once = true;
}

vlk::Camera *SampleGameWorld::getCamera() {
    return &this->camera;
}

SampleGameWorld::SampleGameWorld(vlk::Engine *engine) {
    this->engine = engine;
    vlk::ResourceModel *resourceModel = this->engine->getResourceManager()->loadModel("cube.json");
    this->objects.emplace_back(new SampleObject(resourceModel));
}
