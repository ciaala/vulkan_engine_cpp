//
// Created by crypt on 21/07/17.
//

#include "Audio.hpp"

vlk::Audio::Audio(void *data, ALenum alFormat, ALsizei rate, ALsizei size) :
        data(data),
        alFormat(alFormat),
        rate(rate),
        isBufferCreated(false),
        buffer(0), size(size) {

}

ALuint vlk::Audio::getBuffer() {
    if (!isBufferCreated) {
        alGenBuffers((ALuint) 1, &buffer);
        alBufferData(this->buffer, this->alFormat, this->data, this->size, this->rate);
    }
    return buffer;
}

