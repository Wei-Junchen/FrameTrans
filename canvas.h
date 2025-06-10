#ifndef CANVAS_H
#define CANVAS_H

#include "mymath.h"
#include <vector>

//定义画布，将来可以用来绘制图形或显示内容
class Canvas
{
public:
    // 添加点到画布
    void addPoint(const vec2& point) {
        points_.push_back(point);
    }

    // 获取所有点
    const std::vector<vec2>& getPoints() const {
        return points_;
    }

    // 清空画布上的点
    void clear() {
        points_.clear();
    }

    // 获取画布上点的数量
    std::size_t size() const {
        return points_.size();
    }
private:
    std::vector<vec2> points_; // 存储画布上的点
};

#endif