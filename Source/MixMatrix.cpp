/*
  ==============================================================================

    MixMatrix.cpp
    Created: 2 Sep 2022 12:06:49pm
    Author:  enric

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

float SimpleMixMatrix::mix(float* signalArr)
{
    float s1 = signalArr[0] + signalArr[2];
    float s2 = signalArr[1] + signalArr[3];
    mixArr[0] = s1 + s2;
    mixArr[1] = -mixArr[0];
    mixArr[3] = s1 - s2;
    mixArr[2] = -mixArr[3];

    return *mixArr;
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