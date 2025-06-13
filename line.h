#ifndef LINE_H
#define LINE_H

#include "point.h"
#include <memory>
#include <algorithm>
#include <iostream>

//定义线类(描述的是抽象的关系，不是具体的线段)
class Line : public std::enable_shared_from_this<Line>
{
    friend class CanvasAdapter; // 允许CanvasAdapter类访问Line的私有成员
public:
    // 注意：不能在构造函数中调用 shared_from_this()
    Line(std::shared_ptr<Point> start, std::shared_ptr<Point> end): start_(start), end_(end) {}
    
    static std::shared_ptr<Line> create(std::shared_ptr<Point> start, std::shared_ptr<Point> end)
    {
        auto line = std::make_shared<Line>(start, end);
        // 添加线段到起点和终点的线段集合中
        if (start) start->lines_.push_back(line->get_const_shared_ptr());
        if (end) end->lines_.push_back(line->get_const_shared_ptr());
        return line;
    }

    // 计算线段长度
    double length() const
    {
        if (!start_ || !end_) return 0.0; // 如果起点或终点为空，返回长度为0
        return start_->distanceTo(*end_); // 使用点的距离计算方法
    }

    ~Line()
    {
        deleteLine(); // 删除线段时从起点和终点的线段集合中移除自己
    }
    //移除线段
    void remove()
    {
        deleteLine(); // 从起点和终点的线段集合中移除自己
        start_.reset(); // 清空起点
        end_.reset();   // 清空终点
    }

    //返回一个 const shared_ptr
    std::shared_ptr<const Line> get_const_shared_ptr() const {
        return std::const_pointer_cast<const Line>(shared_from_this());
    }

protected:
    void deleteLine()
    {
        // 从起点和终点的线段集合中移除自己
        if (start_) {
            start_->lines_.erase(
                std::remove_if(start_->lines_.begin(), start_->lines_.end(),
                               [this](const std::shared_ptr<const Line>& line) {
                                   return line.get() == this;
                               }),
                start_->lines_.end());
        }
        if (end_) {
            end_->lines_.erase(
                std::remove_if(end_->lines_.begin(), end_->lines_.end(),
                               [this](const std::shared_ptr<const Line>& line) {
                                   return line.get() == this;
                               }),
                end_->lines_.end());
        }
    }
    std::shared_ptr<Point> start_; // 起点
    std::shared_ptr<Point> end_;   // 终点
};

#endif