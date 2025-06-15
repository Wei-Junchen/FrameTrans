#ifndef POINT_H
#define POINT_H

#include "mymath.h"
#include "frame.h"
#include <memory>
#include <vector>
#include <initializer_list>

class Line; // 前向声明，避免循环依赖

//点类
class Point
{
    friend class Line; // 允许Line类访问Point的私有成员
    friend class CanvasAdapter;
    friend class Camera;
public:
    // virtual Point& transform(std::shared_ptr<const Frame> newFrame) = 0; // 坐标系变换，纯虚函数
    virtual void print() const = 0; // 打印点信息，纯虚函数
    virtual double distanceTo(const Point& other) const = 0; // 计算到另一个点的距离，纯虚函数
protected:
    std::vector<std::shared_ptr<const Line>> lines_; // 点所在的线段集合
};

class Point2D : public Point
{
public:
    Point2D(double x, double y) : position_{vec2{x, y}} {}
    Point2D(const vec2& position) : position_{position} {}
    
    Point2D& setPosition(vec2 position) 
    {
        position_ = position; // 设置点的位置
        return *this; // 返回当前点对象的指针
    }
    vec2 getPosition() const
    {
        return position_; // 返回点的位置
    }

    void print() const override
    {
        std::cout << "Point2D Position: (" << position_[0] << ", " << position_[1] << ")" << std::endl; // 打印点的位置
    }

    Point2D copy() const
    {
        return Point2D(position_); // 返回一个新的Point对象，位置相同
    }

    double distanceTo(const Point& other) const override
    {
        if (auto otherPoint = dynamic_cast<const Point2D*>(&other)) {
            return (position_ - otherPoint->position_).norm(); // 使用欧几里得距离计算
        }
        std::cerr << "Error: Distance calculation only supports Point2D." << std::endl;
            return 0.0; // 如果类型不匹配，返回0
    }

    vec2 position_; // 点的位置
};

class Point3D : public Point
{
public:
    Point3D(double x,double y,double z,std::shared_ptr<const Frame> frame = Frame::getBaseFrame()) : position_{vec3{x, y, z}}, frame_{frame} {}
    Point3D(const vec3& position, std::shared_ptr<const Frame> frame = Frame::getBaseFrame()) : position_{position}, frame_{frame} {}
    
    //坐标系变换
    Point3D& transform(std::shared_ptr<const Frame> newFrame) //override
    {
        Matrix3x3 newBases = Matrix3x3{newFrame->bases[0], newFrame->bases[1], newFrame->bases[2]}; // 获取新坐标系的基向量矩阵
        if(newBases.reverse() == std::nullopt) {
            std::cerr << "Error: The new frame is not invertible." << std::endl;
            return *this; // 返回当前点对象的指针
        }
        newBases = *newBases.reverse(); // 解包可选值，获取逆矩阵
        // 将点的位置从当前坐标系转换到新的坐标系
        Matrix3x3 transMatrix = newBases * Matrix3x3{frame_->bases[0], frame_->bases[1], frame_->bases[2]}; // 获取新坐标系的逆矩阵

        position_ = transMatrix[0] * position_[0] + transMatrix[1] * position_[1] + transMatrix[2] * position_[2];
        frame_ = newFrame; // 更新坐标系
        return *this; // 返回当前点对象的指针
    }

    void print() const override
    {
        std::cout << "Point3D Position: (" << position_[0] << ", " << position_[1] << ", " << position_[2] << ") in Frame: " 
                  << frame_->bases[0][0] << ", " << frame_->bases[0][1] << ", " << frame_->bases[0][2]<<
                    " | " << frame_->bases[1][0] << ", " << frame_->bases[1][1] << ", " << frame_->bases[1][2] <<
                    " | " << frame_->bases[2][0] << ", " << frame_->bases[2][1] << ", " << frame_->bases[2][2] << std::endl; // 打印点的位置和坐标系 

    }

    Point3D copy() const
    {
        return Point3D(position_, frame_); // 返回一个新的Point对象，位置和坐标系相同
    }

    double distanceTo(const Point& other) const override
    {
        if (auto otherPoint = dynamic_cast<const Point3D*>(&other)) {
            return (position_ - otherPoint->copy().transform(frame_).position_).norm(); // 使用欧几里得距离计算
        }
        std::cerr << "Error: Distance calculation only supports Point2D." << std::endl;
            return 0.0; // 如果类型不匹配，返回0
    }

    Point3D toMe(const Point3D& other)
    {
        if(other.frame_ == frame_) {
            return Point3D(other.position_ - position_, frame_); // 如果点在同一坐标系下，直接返回位置差
        }
        return Point3D(other.copy().transform(frame_).position_ - position_, frame_); // 返回转换后的位置差
    }

    vec3 toVec3() const
    {
        return position_; // 返回点的位置作为vec3对象
    }
    
    vec3 position_; // 点的位置
    std::shared_ptr<const Frame> frame_; // 点所在的坐标系
}; 

#endif