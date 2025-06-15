#include <iostream>
#include <memory>
#include "all.h"
#include "minifbAdapter.h"

int main()
{
    std::shared_ptr<Frame> cubeFrame = std::make_shared<Frame>(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
    std::vector<std::shared_ptr<Point3D>> points;
    points.push_back(std::make_shared<Point3D>(-1.0,-1.0,-1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(1.0,-1.0,-1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(-1.0,1.0,-1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(-1.0,-1.0,1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(1.0,1.0,-1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(1.0,-1.0,1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(-1.0,1.0,1.0,cubeFrame));
    points.push_back(std::make_shared<Point3D>(1.0,1.0,1.0,cubeFrame));

    std::vector<std::shared_ptr<Line>> lines;
    lines.push_back(Line::create(points[0], points[1],COLOR_BLUE));
    lines.push_back(Line::create(points[0], points[2],COLOR_BLUE));
    lines.push_back(Line::create(points[0], points[3],COLOR_MAGENTA));
    lines.push_back(Line::create(points[1], points[4],COLOR_BLUE));
    lines.push_back(Line::create(points[1], points[5],COLOR_MAGENTA));
    lines.push_back(Line::create(points[2], points[4],COLOR_BLUE));
    lines.push_back(Line::create(points[2], points[6],COLOR_MAGENTA));
    lines.push_back(Line::create(points[3], points[5],COLOR_RED));
    lines.push_back(Line::create(points[3], points[6],COLOR_RED));
    lines.push_back(Line::create(points[4], points[7],COLOR_MAGENTA));
    lines.push_back(Line::create(points[5], points[7],COLOR_RED));
    lines.push_back(Line::create(points[6], points[7],COLOR_RED));
    lines.push_back(Line::create(points[0], points[4],COLOR_YELLOW)); // 添加一条线段连接点0和点1
    lines.push_back(Line::create(points[1], points[2],COLOR_YELLOW)); // 添加一条线段连接点1和点2
    Camera camera(60.0, 0.1, 1000.0);

    for (const auto& point : points) {
        camera.subscribePoint(point);
    }
    
    //设定相机的位置和朝向
    camera.setPosition(Point3D(-1.75, 0.0, 0.0)); // 设置相机位置,在静止坐标系中
    //std::shared_ptr<Frame> cameraDirectionFrame = std::make_shared<Frame>(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0));
    std::shared_ptr<Frame> cameraDirectionFrame = std::make_shared<Frame>(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
    cameraDirectionFrame->rotate(DegToRad(0.0),DegToRad(90.0),DegToRad(0.0));
    //打印旋转后的坐标系基向量
    std::cout << "Camera Direction Frame Bases:" << std::endl;
    for (const auto& base : cameraDirectionFrame->bases) {
        std::cout << "Base: (" << base[0] << ", " << base[1] << ", " << base[2] << ")" << std::endl;
    }

    camera.setWorldFrame(cameraDirectionFrame);

    camera.calculatePointsInCameraWorld(); // 计算所有点在相机世界系下的坐标
    std::cout << "Calculated points in camera world coordinates." << std::endl;
    camera.printAllPoints(); // 打印相机世界系下的点坐标

    auto canvas = std::make_shared<Canvas>(800, 600); // 创建一个画布
    camera.projectPointsToCanvas(*canvas); // 将点投影到画布上

    std::cout<<canvas->size() << " points projected to canvas." << std::endl;
    canvas->printPoints(); // 打印画布上的点信息

    CanvasAdapter canvasAdapter("Camera Projection", 800, 600);
    canvasAdapter.bindCanvas(canvas);

    while(canvasAdapter.wait())
    {
        cubeFrame->rotate({DegToRad(1),vec3{0.0, 0.0, 1.0}}); // 绕Y轴旋转
        camera.calculatePointsInCameraWorld(); // 重新计算所有点在相机世界系下的坐标
        camera.projectPointsToCanvas(*canvas); // 将点投影到画布上
        canvasAdapter.clear(); // 清空画布
        canvasAdapter.drawPoint(COLOR_GREEN); // 绘制点
        canvasAdapter.drawLine(); // 绘制线段
        canvasAdapter.update(); // 更新窗口显示
    }
    return 0;
}