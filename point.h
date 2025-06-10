#ifndef POINT_H
#define POINT_H

#include "mymath.h"
#include "frame.h"
#include <memory>

//点类
class Point
{
public:
    Point(double x,double y,double z,std::shared_ptr<const Frame> frame = Frame::getBaseFrame()) : position_{vec3{x, y, z}}, frame_{frame} {}
    Point(const vec3& position, std::shared_ptr<const Frame> frame = Frame::getBaseFrame()) : position_{position}, frame_{frame} {}
    
    //坐标系变换
    Point* transform(std::shared_ptr<const Frame> newFrame)
    {
        // 将点的位置从当前坐标系转换到新的坐标系
        vec3 newPosition;
        for (int i = 0; i < 3; ++i) {
            newPosition[i] = position_.dot(newFrame->bases[i]);
        }
        position_ = newPosition;
        frame_ = newFrame; // 更新坐标系
        return this; // 返回当前点对象的指针
    }

    vec3 position_; // 点的位置
    std::shared_ptr<const Frame> frame_; // 点所在的坐标系
};


#endif