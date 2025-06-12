#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"
#include "frame.h"
#include "canvas.h"
#include <vector>
#include <memory>

// 相机类
class Camera
{
public:
    Camera(double fov = 60.0, double nearPlane = 0.1, double farPlane = 1000.0)
        : fieldOfView_(fov), nearPlane_(nearPlane), farPlane_(farPlane),
          worldFrame_(Frame::getBaseFrame()), position_(vec3(0.0, 0.0, 0.0), worldFrame_) {}
    
    //计算所有点到在相机世界系下的坐标
    void calculatePointsInCameraWorld()
    {
        for(std::size_t i = 0;i<subscribedPoints_.size();++i)
        {
            pointsInCameraWorld_[i] = subscribedPoints_[i]->position_ - position_.transform(subscribedPoints_[i]->frame_).position_;
        }
    }

    //把所有点投影到一个画布上
    Canvas projectPointsToCanvas(const Canvas& canvas)
    {
        Canvas projectedCanvas;
        for (const auto& point : pointsInCameraWorld_) {
            if(point[2] < 0.0 || point[2] < nearPlane_ || point[2] > farPlane_)
                continue; // 忽略不可见的点
            projectedCanvas.addPoint({point[0],point[1]}); // 投影到画布上
        }
        return projectedCanvas;
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

private:
    //相机订阅的点
    std::vector<std::shared_ptr<const Point3D>> subscribedPoints_;
    std::vector<vec3> pointsInCameraWorld_; // 相机世界系下的点坐标

    //定义相机视野范围
    double fieldOfView_; // 相机视野角度
    double nearPlane_; // 最近可见平面
    double farPlane_; // 最远可见平面

    //相机世界系(包含相机方向，但不包含位置)
    std::shared_ptr<const Frame> worldFrame_;
    Point3D position_; // 相机位置(包含位置和坐标系)
};

#endif