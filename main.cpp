#include <iostream>
#include <memory>
#include "all.h"
#include "minifbAdapter.h"

int main()
{
    // auto p1 = std::make_shared<Point3D>(1.0, 2.0, 3.0);
    // auto p2 = std::make_shared<Point3D>(4.0, 5.0, 6.0);
    // std::shared_ptr<Line> line = Line::create(p1, p2);

    // std::cout << "Line length: " << line->length() << std::endl;
    // // // 测试点的坐标系变换
    // std::shared_ptr<Frame> frame = std::make_shared<Frame>();
    // frame->rotate(DegToRad(90.0), 0.0, 0.0); // 旋转90度绕X轴
    // p1->transform(frame); // 将点p1转换到新的坐标系
    // p1->print(); // 打印点p1的新位置
    // std::cout<<line->length()<<std::endl;

    auto p1 = std::make_shared<Point2D>(100.0, 100.0);
    auto p2 = std::make_shared<Point2D>(200.0, 200.0);
    auto p3 = std::make_shared<Point2D>(300.0, 100.0);
    std::shared_ptr<Line> line = Line::create(p1, p2);
    std::shared_ptr<Line> line2 = Line::create(p2, p3);
    std::shared_ptr<Line> line3 = Line::create(p3, p1);
    vec2 v1{1.0, 2.0},v2{1.0, 1.0},v3{2.0, 1.0};
    Canvas canvas;
    canvas.addPoint(p1);
    canvas.addPoint(p2);
    canvas.addPoint(p3);
    CanvasAdapter canvasAdapter("Test Canvas", 800, 600);
    canvasAdapter.bindCanvas(std::make_shared<Canvas>(canvas));
    while(canvasAdapter.wait())
    {
        static uint32_t frameCount = 0; // 帧计数器
        frameCount++;
        std::cout<< "Frame: " << frameCount << std::endl; // 打印当前帧数
        p1->setPosition({p1->position_[0] + v1[0], p1->position_[1] + v1[1]}); // 移动点p1
        p2->setPosition({p2->position_[0] + v2[0], p2->position_[1] + v2[1]}); // 移动点p2
        p3->setPosition({p3->position_[0] + v3[0], p3->position_[1] + v3[1]}); // 移动点p3
        // 检查点是否超出边界并反弹
        if (p1->position_[0] <= 0 || p1->position_[0] >= 800) v1[0] = -v1[0]; // 点p1碰到边界反弹
        if (p1->position_[1] <= 0 || p1->position_[1] >= 600) v1[1] = -v1[1]; // 点p1碰到边界反弹
        if (p2->position_[0] <= 0 || p2->position_[0] >= 800) v2[0] = -v2[0]; // 点p2碰到边界反弹
        if (p2->position_[1] <= 0 || p2->position_[1] >= 600) v2[1] = -v2[1]; // 点p2碰到边界反弹
        if (p3->position_[0] <= 0 || p3->position_[0] >= 800) v3[0] = -v3[0]; // 点p3碰到边界反弹
        if (p3->position_[1] <= 0 || p3->position_[1] >= 600) v3[1] = -v3[1]; // 点p3碰到边界反弹
        // 打印点的位置
        canvasAdapter.clear(); // 清空画布
        canvasAdapter.drawPoint(COLOR_GREEN); // 绘制点
        canvasAdapter.drawLine(COLOR_MAGENTA); // 绘制线段
        canvasAdapter.update(); // 更新窗口显示
    }

    // //创建minifb窗口
    // MiniFB window("Test Window", 800, 600);
    // window.clear(); // 清屏
    // // window.setPixel(400, 300, 0xFFFF0000); // 设置一个红色像素
    // window.update(); // 更新窗口显示
    // // 等待一段时间以便查看窗口
    // uint32_t x{},y{};
    // int8_t vx{1},vy{1};

    // while(window.wait())
    // {
    //     window.clear(); // 清屏
    //     // 在这里可以添加更多的绘图操作
    //     // window.setPixel(400, 300, 0xFF00FF00); // 设置一个绿色像素
    //     window.drawRect(x, y, 100, 100, COLOR_MAGENTA); // 绘制一个紫色矩形
    //     window.update(); // 更新窗口显示

    //     x += vx;
    //     y += vy;
    //     if(x+100 >= 800 || x <= 0) vx = -vx; // 碰到边界反弹
    //     if(y+100 >= 600 || y <= 0) vy = -vy; // 碰到边界反弹
    //     std::cout << "Current position: (" << x << ", " << y << ")" << std::endl;
    // }
    return 0;
}