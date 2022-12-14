#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include "Entity.hpp"

namespace tengine
{
    class Entity;

    class Component
    {
    protected:
        tengine::Entity &entity;

    public:
        Component(tengine::Entity &entity) : entity(entity) {}

        virtual void bind(){};
        virtual void preDraw(){};
        virtual void draw(){};
        virtual void postDraw(){};
        virtual void unbind(){};

        virtual void init(){};
        virtual void update(double timeElapsed){};
    };
}

#endif