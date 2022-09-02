/*
  ==============================================================================

    MixMatrix.h
    Created: 2 Sep 2022 12:06:48pm
    Author:  enric

  ==============================================================================
*/

#pragma once
#include <vector>

// Schroeder mixing matrix
class SimpleMixMatrix
{
public:
    SimpleMixMatrix();
    ~SimpleMixMatrix();
    float mix(float* signalArr);

private:
    float* mixArr;
};

//==============================================================================

class Householder
{
public:
    Householder();
    ~Householder();


private:

};

//==============================================================================

class Hadamard
{
public:
    Hadamard();
    ~Hadamard();


private:

};