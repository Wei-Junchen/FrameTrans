#ifndef CANVAS_H
#define CANVAS_H

#include "mymath.h"
#include <vector>
#include "minifbAdapter.h"
#include "point.h"
#include <memory>

//定义画布，将来可以用来绘制图形或显示内容
class Canvas
{
public:
    // 添加点到画布
    void addPoint(std::shared_ptr<Point> point) {
        if (!point) {
            throw std::invalid_argument("Point cannot be null");
        }
        points_.push_back(point);
    }

    // 获取所有点
    const std::vector<std::shared_ptr<Point>>& getPoints() const {
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
    std::vector<std::shared_ptr<Point>> points_; // 存储画布上的点
};

class CanvasAdapter
{
};

#endif