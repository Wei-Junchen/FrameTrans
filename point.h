#ifndef POINT_H
#define POINT_H

#include "mymath.h"
#include "frame.h"
#include <memory>
#include <vector>

class Line; // 前向声明，避免循环依赖

//点类
class Point
{
    friend class Line; // 允许Line类访问Point的私有成员
public:
    Point(double x,double y,double z,std::shared_ptr<const Frame> frame = Frame::getBaseFrame()) : position_{vec3{x, y, z}}, frame_{frame} {}
    Point(const vec3& position, std::shared_ptr<const Frame> frame = Frame::getBaseFrame()) : position_{position}, frame_{frame} {}
    
    //坐标系变换
    Point* transform(std::shared_ptr<const Frame> newFrame)
    {
        std::cout<<"old frame bases:"<<std::endl;
        std::cout<<frame_->bases[0][0]<<" "<<frame_->bases[0][1]<<" "<<frame_->bases[0][2]<<std::endl;
        std::cout<<frame_->bases[1][0]<<" "<<frame_->bases[1][1]<<" "<<frame_->bases[1][2]<<std::endl;
        std::cout<<frame_->bases[2][0]<<" "<<frame_->bases[2][1]<<" "<<frame_->bases[2][2]<<std::endl;

        std::cout<<"new frame bases:"<<std::endl;
        std::cout<<newFrame->bases[0][0]<<" "<<newFrame->bases[0][1]<<" "<<newFrame->bases[0][2]<<std::endl;
        std::cout<<newFrame->bases[1][0]<<" "<<newFrame->bases[1][1]<<" "<<newFrame->bases[1][2]<<std::endl;
        std::cout<<newFrame->bases[2][0]<<" "<<newFrame->bases[2][1]<<" "<<newFrame->bases[2][2]<<std::endl;

        Matrix3x3 newBases = Matrix3x3{newFrame->bases[0], newFrame->bases[1], newFrame->bases[2]}; // 获取新坐标系的基向量矩阵
        newBases = newBases.reverse(); // 计算新坐标系的逆矩阵

        std::cout<<"newBases:"<<std::endl;
        std::cout<<newBases[0][0]<<" "<<newBases[0][1]<<" "<<newBases[0][2]<<std::endl;
        std::cout<<newBases[1][0]<<" "<<newBases[1][1]<<" "<<newBases[1][2]<<std::endl;
        std::cout<<newBases[2][0]<<" "<<newBases[2][1]<<" "<<newBases[2][2]<<std::endl;

        // 将点的位置从当前坐标系转换到新的坐标系
        vec3 newPosition;
        Matrix3x3 transMatrix = Matrix3x3{newFrame->bases[0], newFrame->bases[1], newFrame->bases[2]}.reverse() * Matrix3x3{frame_->bases[0], frame_->bases[1], frame_->bases[2]}; // 获取新坐标系的逆矩阵
        
        std::cout<<"transMatrix:"<<std::endl;
        std::cout<<transMatrix[0][0]<<" "<<transMatrix[0][1]<<" "<<transMatrix[0][2]<<std::endl;
        std::cout<<transMatrix[1][0]<<" "<<transMatrix[1][1]<<" "<<transMatrix[1][2]<<std::endl;
        std::cout<<transMatrix[2][0]<<" "<<transMatrix[2][1]<<" "<<transMatrix[2][2]<<std::endl;


        newPosition = transMatrix[0] * position_[0] + transMatrix[1] * position_[1] + transMatrix[2] * position_[2];

        std::cout<<"newPosition:"<<std::endl;
        std::cout<<newPosition[0]<<" "<<newPosition[1]<<" "<<newPosition[2]<<std::endl;
        position_ = newPosition;
        frame_ = newFrame; // 更新坐标系
        return this; // 返回当前点对象的指针
    }

    Point copy() const
    {
        return Point(position_, frame_); // 返回一个新的Point对象，位置和坐标系相同
    }

    vec3 position_; // 点的位置
    std::shared_ptr<const Frame> frame_; // 点所在的坐标系
// private:
    std::vector<std::shared_ptr<const Line>> lines_; // 点所在的线段集合
};


#endif