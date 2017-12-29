//
// Created by crypt on 04/12/17.
//

#include "AudioObject.hpp"

vlk::Audio *vlk::AudioObject::getAudio() {
    return audio;
}

void vlk::AudioObject::setAudio(vlk::Audio *audio) {
    this->audio = audio;
}

const std::string &vlk::AudioObject::getResouceIdentifier() {
    return identifier;
}
vlk::AudioObject::AudioObject(std::string identifier) : identifier(identifier){

}
