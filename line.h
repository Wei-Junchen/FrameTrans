#ifndef LINE_H
#define LINE_H

#include "point.h"
#include <memory>
#include <algorithm>
#include <iostream>

//定义线类
class Line : public std::enable_shared_from_this<Line>
{
public:
    Line() = default;
    // 注意：不能在构造函数中调用 shared_from_this()
    Line(std::shared_ptr<Point> start, std::shared_ptr<Point> end): start_(start), end_(end) {}
    //工厂方法函数
    static std::shared_ptr<Line> create(std::shared_ptr<Point> start, std::shared_ptr<Point> end)
    {
        auto line = std::make_shared<Line>(start, end);
        // 添加线段到起点和终点的线段集合中
        if (start) start->lines_.push_back(line->get_const_shared_ptr());
        if (end) end->lines_.push_back(line->get_const_shared_ptr());
        return line;
    }

    ~Line()
    {
        // 在析构函数中从点的线段集合中移除自己
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

    double length() const
    {
        if (!start_ || !end_) return 0.0; // 如果起点或终点为空，返回长度为0
        Point start = start_->copy(); // 复制起点

        // std::cout<<start_->frame_->bases[0][0]<<" "<<start_->frame_->bases[0][1]<<" "<<start_->frame_->bases[0][2]<<std::endl;
        // std::cout<<start_->frame_->bases[1][0]<<" "<<start_->frame_->bases[1][1]<<" "<<start_->frame_->bases[1][2]<<std::endl;
        // std::cout<<start_->frame_->bases[2][0]<<" "<<start_->frame_->bases[2][1]<<" "<<start_->frame_->bases[2][2]<<std::endl;


        std::cout<<"start trans before"<<start.position_[0]<<" "<<start.position_[1]<<" "<<start.position_[2]<<std::endl;

        std::cout<<end_->frame_->bases[0][0]<<" "<<end_->frame_->bases[0][1]<<" "<<end_->frame_->bases[0][2]<<std::endl;
        std::cout<<end_->frame_->bases[1][0]<<" "<<end_->frame_->bases[1][1]<<" "<<end_->frame_->bases[1][2]<<std::endl;
        std::cout<<end_->frame_->bases[2][0]<<" "<<end_->frame_->bases[2][1]<<" "<<end_->frame_->bases[2][2]<<std::endl;

        start.transform(end_->frame_); // 将起点转换到终点的坐标系

        std::cout<<start.position_[0]<<" "<<start.position_[1]<<" "<<start.position_[2]<<std::endl;
        vec3 diff = end_->position_ - start.position_;
        return std::sqrt(diff.dot(diff)); // 计算长度
    }

    // 如果你想返回一个 const shared_ptr
    std::shared_ptr<const Line> get_const_shared_ptr() const {
        return std::const_pointer_cast<const Line>(shared_from_this());
    }

private:
    std::shared_ptr<Point> start_; // 起点
    std::shared_ptr<Point> end_;   // 终点
};


#endif