#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"
#include "frame.h"
#include "canvas.h"
#include <vector>
#include <memory>

#define VISION_RATIO 1000.0

struct brokenLine
{
    std::shared_ptr<const Point3D> brokenPoint; // 断点(不可投影的点)
    std::shared_ptr<const Line> line; // 断点所在的线段
};

// 相机类
class Camera
{
public:
    Camera(double fov = 60.0, double nearPlane = 0.5, double farPlane = 1000.0)
        : fieldOfView_(fov), nearPlane_(nearPlane), farPlane_(farPlane),
          worldFrame_(Frame::getBaseFrame()), position_(vec3(0.0, 0.0, 0.0), worldFrame_) {}
    
    //计算所有点到在相机世界系下的坐标
    void calculatePointsInCameraWorld()
    {
        for(std::size_t i = 0;i<subscribedPoints_.size();++i)
        {
            Point3D positionInCameraWorld = position_.toMe(*subscribedPoints_[i]); // 计算点在相机世界系下的位置
            positionInCameraWorld.transform(worldFrame_); // 将点转换到相机的世界系下
            if(pointsInCameraWorld_.size() <= i)
                pointsInCameraWorld_.push_back(positionInCameraWorld.toVec3()); // 如果当前点的索引超出范围，则添加新的点
            else
                pointsInCameraWorld_[i] = positionInCameraWorld.toVec3(); // 更新现有点的坐标
        }
    }

    //把所有点投影到一个画布上
    void projectPointsToCanvas(Canvas& canvas)
    {
        std::vector<std::shared_ptr<Line>> lines; // 用于存储线段
        std::vector<brokenLine> brokenLines; // 用于存储断点和线段关系
        canvas.clear();
        resetAddedPoints(); // 清空之前添加的点
        for(std::size_t i = 0; i < pointsInCameraWorld_.size(); ++i)
        {
            if(isPointInView(pointsInCameraWorld_[i]))
            {
                for(const auto& line : subscribedPoints_[i]->lines_)
                {
                    //如果线段的起点和终点都在可视范围内，则直接投影
                    if(line->start_ == subscribedPoints_[i])
                    {
                        //寻找line->end_的下标
                        std::size_t endPointIndex = std::find(subscribedPoints_.begin(), subscribedPoints_.end(), line->end_) - subscribedPoints_.begin(); // 计算终点的索引
                        if(isPointInView(pointsInCameraWorld_[endPointIndex]))
                        {
                            if(!addedPoints[i])
                                addedPoints[i] = canvas.addPoint(vec2{pointsInCameraWorld_[i][0] * (nearPlane_ / pointsInCameraWorld_[i][2]) * VISION_RATIO, 
                                                                                     pointsInCameraWorld_[i][1] * (nearPlane_ / pointsInCameraWorld_[i][2]) * VISION_RATIO}); // 将起点投影到画布上
                            if(!addedPoints[endPointIndex])
                                addedPoints[endPointIndex] = canvas.addPoint(vec2{pointsInCameraWorld_[endPointIndex][0] * (nearPlane_ / pointsInCameraWorld_[endPointIndex][2]) * VISION_RATIO,
                                                                                        pointsInCameraWorld_[endPointIndex][1] * (nearPlane_ / pointsInCameraWorld_[endPointIndex][2]) * VISION_RATIO}); // 将终点投影到画布上
                            lines.push_back(Line::create(addedPoints[i], addedPoints[endPointIndex],line->color)); // 创建线段并添加到画布上
                        }
                        else 
                            brokenLines.push_back({subscribedPoints_[endPointIndex], line}); // 如果线段的终点不在可视范围内，则将断点和线段关系存储起来
                    }
                    else 
                    {
                        //寻找line->start_的下标
                        std::size_t startPointIndex = std::find(subscribedPoints_.begin(), subscribedPoints_.end(), line->start_) - subscribedPoints_.begin(); // 计算起点的索引
                        if(isPointInView(pointsInCameraWorld_[startPointIndex]))
                        {
                            if(!addedPoints[i])
                                addedPoints[i] = canvas.addPoint(vec2{pointsInCameraWorld_[i][0] * (nearPlane_ / pointsInCameraWorld_[i][2]) * VISION_RATIO, 
                                                                                     pointsInCameraWorld_[i][1] * (nearPlane_ / pointsInCameraWorld_[i][2]) * VISION_RATIO}); // 将起点投影到画布上
                            if(!addedPoints[startPointIndex])
                                addedPoints[startPointIndex] = canvas.addPoint(vec2{pointsInCameraWorld_[startPointIndex][0] * (nearPlane_ / pointsInCameraWorld_[startPointIndex][2]) * VISION_RATIO,
                                                                                        pointsInCameraWorld_[startPointIndex][1] * (nearPlane_ / pointsInCameraWorld_[startPointIndex][2]) * VISION_RATIO}); // 将终点投影到画布上
                            lines.push_back(Line::create(addedPoints[i], addedPoints[startPointIndex],line->color)); // 创建线段并添加到画布上
                        }
                        else 
                            brokenLines.push_back({subscribedPoints_[startPointIndex], line}); // 如果线段的起点不在可视范围内，则将断点和线段关系存储起来
                    }
                }
            }
            else 
            {
                if(subscribedPoints_[i]->lines_.size() > 0) // 如果点有线段
                {
                    for(const auto& line : subscribedPoints_[i]->lines_)
                    {
                        if(line->start_ == subscribedPoints_[i])
                        {
                            //寻找line->end_的下标
                            std::size_t endPointIndex = std::find(subscribedPoints_.begin(), subscribedPoints_.end(), line->end_) - subscribedPoints_.begin(); // 计算终点的索引
                            if(isPointInView(pointsInCameraWorld_[endPointIndex]))
                                brokenLines.push_back({subscribedPoints_[i], line}); // 如果线段的终点在可视范围内，则将断点和线段关系存储起来
                            else 
                                continue; // 如果线段的终点不在可视范围内，则跳过
                        }
                        else 
                        {
                            //寻找line->start_的下标
                            std::size_t startPointIndex = std::find(subscribedPoints_.begin(), subscribedPoints_.end(), line->start_) - subscribedPoints_.begin(); // 计算起点的索引
                            if(isPointInView(pointsInCameraWorld_[startPointIndex]))
                                brokenLines.push_back({subscribedPoints_[i], line}); // 如果线段的起点在可视范围内，则将断点和线段关系存储起来
                            else 
                                continue; // 如果线段的起点不在可视范围内，则跳过
                        }
                    }
                }
            }
        }
        //对brokenLines进行处理
        //TODO: 实现断点和线段的处理逻辑
        for(const auto& broken : brokenLines)
        {
            ;;
        }
    }

    //切换相机方向坐标系
    void setWorldFrame(std::shared_ptr<const Frame> worldFrame)
    {
        worldFrame_ = worldFrame;
    }

    //设置相机位置
    void setPosition(Point3D position)
    {
        position_ = position;
    }

    // 添加订阅的点
    void subscribePoint(std::shared_ptr<const Point3D> point)
    {
        subscribedPoints_.push_back(point);
    }
    void setFieldOfView(double fov)
    {
        fieldOfView_ = fov;
    }
    void setNearPlane(double nearPlane)
    {
        nearPlane_ = nearPlane;
    }
    void setFarPlane(double farPlane)
    {
        farPlane_ = farPlane;
    }

    void printAllPoints() const
    {
        for (int i = 0; i < pointsInCameraWorld_.size(); ++i){
            std::cout<<pointsInCameraWorld_[i][0] << ", " 
                     << pointsInCameraWorld_[i][1] << ", " 
                     << pointsInCameraWorld_[i][2] << std::endl; // 打印相机世界系下的点坐标
        }
    }

    bool isPointInView(const vec3 position) const
    {
        return true; // TODO: 实现点是否在相机视野范围内的判断逻辑


        // // 检查点是否在相机的视野范围内
        // double distance = (point.position_ - position_.position_).norm();
        // if (distance < nearPlane_ || distance > farPlane_) {
        //     return false; // 点在最近或最远平面之外
        // }
        // // 计算点在相机视野中的角度
        // double angle = std::atan2(point.position_[1] - position_.position_[1], point.position_[0] - position_.position_[0]);
        // return std::abs(angle) <= fieldOfView_ / 2.0; // 检查点是否在视野角度范围内
    }

private:
    void resetAddedPoints()
    {
        addedPoints.clear();
        addedPoints.resize(subscribedPoints_.size(), nullptr); // 初始化所有点为未添加状态
    }

    //相机订阅的点
    std::vector<std::shared_ptr<const Point3D>> subscribedPoints_;
    std::vector<vec3> pointsInCameraWorld_; // 相机世界系下的点坐标
    std::vector<std::shared_ptr<Point2D>> addedPoints; // 用于存储已添加的点

    //定义相机视野范围
    double fieldOfView_; // 相机视野角度
    double nearPlane_; // 最近可见平面
    double farPlane_; // 最远可见平面

    //相机世界系(包含相机方向，但不包含位置)
    std::shared_ptr<const Frame> worldFrame_;
    Point3D position_; // 相机位置(包含位置和坐标系)
};

#endif