#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__

#include <vector>

#include "Color.hpp"
#include "Point.hpp"



class Shape {
    struct Vertex {
        struct tengine::Color color;
        struct tengine::Point point;
    };

    private:
        struct Vertex *vertices;


    public:
        static Shape& buildSquare(struct tengine::Color);
        static Shape& buildTriangle(struct tengine::Color);
        static Shape& buildTrapezoid(struct tengine::Color);

};

#endif