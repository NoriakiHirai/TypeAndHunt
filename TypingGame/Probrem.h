#pragma once
#include <string>
#include "render.h"

class Probrem : public Object
{
public:
    //char probrems[100][25];
    std::string probrems[100];

public:
    Probrem();
    ~Probrem();

    void Update();
    void Draw();
};

