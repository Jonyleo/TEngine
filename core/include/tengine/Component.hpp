#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

namespace tengine
{
    class Component
    {
    public:
        virtual void bind(){};
        virtual void preDraw(){};
        virtual void draw(){};
        virtual void postDraw(){};
        virtual void unbind(){};
    };
}

#endif