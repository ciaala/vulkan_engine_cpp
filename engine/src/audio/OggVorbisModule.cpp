//
// Created by crypt on 21/07/17.
//

#include "OggVorbisModule.hpp"
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <iostream>
#include "AL/al.h"
#include "AL/alc.h"
#include "Audio.hpp"

int isLittleEndian() {
    // TODO rework the endianess recognition
    if (__BYTE_ORDER == __BIG_ENDIAN) {
        return 1;
    } else {
        return 0;
    }
}

vlk::Audio *vlk::OggVorbisModule::load(const char *filename) {
    FILE *file = fopen(filename, "rb");

    OggVorbis_File vorbis{0};
    if (ov_open(file, &vorbis, nullptr, 0) != 0) {
        std::cerr << "Could not read " << filename << std::endl;
        return nullptr;
    };

    vorbis_info *info = ov_info(&vorbis, -1);
    ogg_int64_t samples = ov_pcm_total(&vorbis, -1);
    long rate = info->rate;
    int channels = info->channels;


    ALenum alFormat = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    // TODO discuss size_t VS ogg_vorbis_64_t
    size_t bytes = 2 * (size_t) samples * channels;

    auto data = new char[bytes];

    size_t bytes_read = 0;
    while (bytes_read < bytes) {
        size_t remain = bytes - bytes_read;
        char *cursor = data + bytes_read;

        long value = ov_read(&vorbis, cursor, (int) remain, isLittleEndian(), 2, 1, NULL);
        if (value < 0) {
            std::cerr << "Could not read " << filename << std::endl;
        }

        bytes_read += value;
    }


    ov_clear(&vorbis);
    return new Audio(data, alFormat, (ALsizei) rate, (ALsizei) bytes);
}
