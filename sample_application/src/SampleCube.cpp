//
// Created by crypt on 19/07/17.
//

#include "../include/SampleCube.hpp"

void SampleCube::rotateLeft() {
    this->spinningAngle -= spinningStep;
}

void SampleCube::rotateRight() {
    this->spinningAngle += spinningStep;
}
