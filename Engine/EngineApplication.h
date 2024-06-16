#pragma once

class EngineApplication
{
public:
    // Funci�n virtual pura
    virtual void init () = 0;
    virtual void render () = 0;
    

    // Destructores virtuales son importantes en las clases base
    virtual ~EngineApplication () { }
};