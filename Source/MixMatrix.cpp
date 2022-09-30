/*
  ==============================================================================

    MixMatrix.cpp
    Created: 2 Sep 2022 12:06:49pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#include "MixMatrix.h"

SimpleMixMatrix::SimpleMixMatrix()
{
    mixArr = new float[4]{};
}

SimpleMixMatrix::~SimpleMixMatrix()
{
    delete[] mixArr;
}

std::pair<float, float> SimpleMixMatrix::mix(float* signalArr)
{
    float s1 = signalArr[0] + signalArr[2];
    float s2 = signalArr[1] + signalArr[3];
    mixArr[0] = s1 + s2;
    mixArr[1] = -mixArr[0];
    mixArr[3] = s1 - s2;
    mixArr[2] = -mixArr[3];
    float chanL = mixArr[0] + mixArr[2];
    float chanR = mixArr[1] + mixArr[3];

    return std::pair<float, float>(chanL, chanR);
}

//==============================================================================

Householder::Householder()
{

}

Householder::~Householder()
{

}

//==============================================================================

Hadamard::Hadamard()
{

}

Hadamard::~Hadamard()
{

}