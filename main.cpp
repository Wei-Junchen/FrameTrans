#include <iostream>
#include <memory>
#include "all.h"
#include "minifbAdapter.h"

int main()
{
    std::shared_ptr<Point> p1 = std::make_shared<Point3D>(1.0, 2.0, 3.0);
    std::shared_ptr<Point> p2 = std::make_shared<Point3D>(4.0, 5.0, 6.0);
    std::shared_ptr<Line> line = Line::create(p1, p2);

    std::cout << "Line length: " << line->length() << std::endl;
    // // 测试点的坐标系变换
    std::shared_ptr<Frame> frame = std::make_shared<Frame>();
    frame->rotate(DegToRad(90.0), 0.0, 0.0); // 旋转90度绕X轴
    dynamic_cast<Point3D*>(p1.get())->transform(frame); // 将点p1转换到新的坐标系
    dynamic_cast<Point3D*>(p1.get())->print(); // 打印点p1的新位置
    std::cout<<line->length()<<std::endl;

    //创建minifb窗口
    MiniFB window("Test Window", 800, 600);
    window.clear(); // 清屏
    // window.setPixel(400, 300, 0xFFFF0000); // 设置一个红色像素
    window.update(); // 更新窗口显示
    // 等待一段时间以便查看窗口
    uint32_t x{},y{};
    int8_t vx{1},vy{1};

    while(window.wait())
    {
        window.clear(); // 清屏
        // 在这里可以添加更多的绘图操作
        // window.setPixel(400, 300, 0xFF00FF00); // 设置一个绿色像素
        window.drawRect(x, y, 100, 100, COLOR_MAGENTA); // 绘制一个紫色矩形
        window.update(); // 更新窗口显示

        x += vx;
        y += vy;
        if(x+100 >= 800 || x <= 0) vx = -vx; // 碰到边界反弹
        if(y+100 >= 600 || y <= 0) vy = -vy; // 碰到边界反弹
        std::cout << "Current position: (" << x << ", " << y << ")" << std::endl;
    }
    return 0;
}