#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include "Entity.hpp"

namespace tengine
{
    class Entity;

    class Component
    {    
    protected:
        tengine::Entity &parent;
    
    public:
        Component(tengine::Entity &parent) : parent(parent) {}

        virtual void bind(){};
        virtual void preDraw(){};
        virtual void draw(){};
        virtual void postDraw(){};
        virtual void unbind(){};

        virtual void update(double timeElapsed){};
    };
}

#endif