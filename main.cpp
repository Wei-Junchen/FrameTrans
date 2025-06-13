#include <iostream>
#include <memory>
#include "all.h"
#include "minifbAdapter.h"

int main()
{
    // auto p1 = std::make_shared<Point2D>(100.0, 100.0);
    // auto p2 = std::make_shared<Point2D>(200.0, 200.0);
    // auto p3 = std::make_shared<Point2D>(300.0, 100.0);
    // std::shared_ptr<Line> line = Line::create(p1, p2);
    // std::shared_ptr<Line> line2 = Line::create(p2, p3);
    // std::shared_ptr<Line> line3 = Line::create(p3, p1);
    // vec2 v1{1.0, 2.0},v2{1.0, 1.0},v3{2.0, 1.0};
    // Canvas canvas;
    // canvas.addPoint(p1);
    // canvas.addPoint(p2);
    // canvas.addPoint(p3);
    // CanvasAdapter canvasAdapter("Test Canvas", 800, 600);
    // canvasAdapter.bindCanvas(std::make_shared<Canvas>(canvas));
    // while(canvasAdapter.wait())
    // {
    //     static uint32_t frameCount = 0; // 帧计数器
    //     frameCount++;
    //     std::cout<< "Frame: " << frameCount << std::endl; // 打印当前帧数
    //     p1->setPosition({p1->position_[0] + v1[0], p1->position_[1] + v1[1]}); // 移动点p1
    //     p2->setPosition({p2->position_[0] + v2[0], p2->position_[1] + v2[1]}); // 移动点p2
    //     p3->setPosition({p3->position_[0] + v3[0], p3->position_[1] + v3[1]}); // 移动点p3
    //     // 检查点是否超出边界并反弹
    //     if (p1->position_[0] <= 0 || p1->position_[0] >= 800) v1[0] = -v1[0]; // 点p1碰到边界反弹
    //     if (p1->position_[1] <= 0 || p1->position_[1] >= 600) v1[1] = -v1[1]; // 点p1碰到边界反弹
    //     if (p2->position_[0] <= 0 || p2->position_[0] >= 800) v2[0] = -v2[0]; // 点p2碰到边界反弹
    //     if (p2->position_[1] <= 0 || p2->position_[1] >= 600) v2[1] = -v2[1]; // 点p2碰到边界反弹
    //     if (p3->position_[0] <= 0 || p3->position_[0] >= 800) v3[0] = -v3[0]; // 点p3碰到边界反弹
    //     if (p3->position_[1] <= 0 || p3->position_[1] >= 600) v3[1] = -v3[1]; // 点p3碰到边界反弹
    //     // 打印点的位置
    //     canvasAdapter.clear(); // 清空画布
    //     canvasAdapter.drawPoint(COLOR_GREEN); // 绘制点
    //     canvasAdapter.drawLine(COLOR_MAGENTA); // 绘制线段
    //     canvasAdapter.update(); // 更新窗口显示
    // }

    std::shared_ptr<Frame> cubeFrame = std::make_shared<Frame>(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
    std::vector<std::shared_ptr<Point>> points;
    points.push_back(std::make_shared<Point3D>(0.0,0.0,0.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(1.0,0.0,0.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(0.0,1.0,0.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(0.0,0.0,1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(1.0,1.0,0.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(1.0,0.0,1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(0.0,1.0,1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(1.0,1.0,1.0,cubeFrame));
    std::vector<std::shared_ptr<Line>> lines;
    lines.push_back(Line::create(points[0], points[1]));
    lines.push_back(Line::create(points[0], points[2]));
    lines.push_back(Line::create(points[0], points[3]));
    lines.push_back(Line::create(points[1], points[4]));
    lines.push_back(Line::create(points[1], points[5]));
    lines.push_back(Line::create(points[2], points[4]));
    lines.push_back(Line::create(points[2], points[6]));
    lines.push_back(Line::create(points[3], points[5]));
    lines.push_back(Line::create(points[3], points[6]));
    lines.push_back(Line::create(points[4], points[7]));
    lines.push_back(Line::create(points[5], points[7]));
    lines.push_back(Line::create(points[6], points[7]));

    Camera camera(60.0, 0.1, 1000.0);
    for (const auto& point : points) {
        camera.subscribePoint(std::static_pointer_cast<const Point3D>(point));
    }
    //设定相机的位置和朝向
    camera.setPosition(Point3D(3.0, 3.0, 2.0)); // 设置相机位置,在静止坐标系中
    std::shared_ptr<Frame> cameraDirectionFrame = std::make_shared<Frame>(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(-1.0, 0.0, 0.0));
    cameraDirectionFrame->rotate(0.0,0.0,DegToRad(45.0));
    camera.setWorldFrame(cameraDirectionFrame);

    camera.calculatePointsInCameraWorld(); // 计算所有点在相机世界系下的坐标
    return 0;
}