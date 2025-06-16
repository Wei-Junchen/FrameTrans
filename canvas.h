#ifndef CANVAS_H
#define CANVAS_H

#include "mymath.h"
#include <vector>
#include "minifbAdapter.h"
#include <memory>
#include "line.h"
#include <set>

//定义画布，将来可以用来绘制图形或显示内容
class Canvas
{
    friend class Camera;
public:
    Canvas(uint16_t width = 800, uint16_t height = 600)
        : width_(width), height_(height) {}
    
        // 添加点到画布
    std::shared_ptr<Point2D> addPoint(vec2 position = vec2{0.0, 0.0}) {
        originPositions_.push_back(position); // 存储原始位置
        position = vec2{-position[0], -position[1]}; // 翻转y轴坐标

        vec2 adjustedPosition = position + vec2{xOffset_, yOffset_}; // 调整点的位置
        points_.push_back(std::make_shared<Point2D>(adjustedPosition)); // 将点添加到画布上
        return points_.back(); // 返回添加的点的智能指针
    }

    // 获取所有点
    const std::vector<std::shared_ptr<Point2D>>& getPoints() const {
        return points_;
    }

    // 清空画布上的点
    void clear() { 
        points_.clear();
        originPositions_.clear(); // 清空原始位置
    }

    // 获取画布上点的数量
    std::size_t size() const {
        return points_.size();
    }

    void printPoints() const {
        for (const auto& point : points_) {
            point->print(); // 打印每个点的信息
        }
    }
protected:
    std::vector<std::shared_ptr<Point2D>> points_; // 存储画布上的点
    std::vector<vec2> originPositions_; // 存储原始位置
    uint16_t width_;  // 画布宽度
    uint16_t height_; // 画布高度
    double xOffset_ {400}; // 画布左上角x坐标偏移
    double yOffset_ {300}; // 画布左上角y坐标偏移
};

class CanvasAdapter : public MiniFB
{
public:
    // 构造函数，创建一个MiniFB窗口和画布
    CanvasAdapter(const char* title, int width, int height): MiniFB(title, width, height), canvas_(std::make_shared<Canvas>()) {}

    void bindCanvas(std::shared_ptr<Canvas> canvas) {
        if (!canvas) {
            throw std::invalid_argument("Canvas cannot be null");
        }
        canvas_ = canvas; // 绑定新的画布
    }

    void drawPoint(uint32_t color = COLOR_WHITE)
    {
        if (!canvas_) {
            throw std::runtime_error("Canvas is not bound");
        }
        for (const auto& point : canvas_->getPoints()) 
        {
            if (auto p2d = std::dynamic_pointer_cast<const Point2D>(point))
                setPixel(static_cast<uint32_t>(p2d->position_[0]), static_cast<uint32_t>(p2d->position_[1]), color); // 绘制白色点
        }
    }

    void drawLine(uint32_t color = COLOR_WHITE)
    {
        
        if (!canvas_) {
            throw std::runtime_error("Canvas is not bound");
        }
        std::set<std::shared_ptr<const Line>> lines;
        for (const auto& point : canvas_->getPoints())
        {
            // point->print(); // 打印点信息
            //遍历所有点的线段指针,添加到lines中，如果重复则不添加
            lines.insert(point->lines_.begin(), point->lines_.end());
        }
        for(const auto& line : lines)
        {
            auto p1 = std::dynamic_pointer_cast<Point2D>(line->start_);
            auto p2 = std::dynamic_pointer_cast<Point2D>(line->end_);
            MiniFB::drawLine(
                static_cast<uint32_t>(p1->position_[0]),
                static_cast<uint32_t>(p1->position_[1]),
                static_cast<uint32_t>(p2->position_[0]),
                static_cast<uint32_t>(p2->position_[1]),
                line-> color // 绘制白色线段
            );
        } 
    }
private:
    std::shared_ptr<Canvas> canvas_; // 画布的智能指针
};

#endif