#include <iostream>
#include <memory>
#include "all.h"

int main()
{
    auto p1 = std::make_shared<Point>(1.0, 2.0, 3.0);
    auto p2 = std::make_shared<Point>(4.0, 5.0, 6.0);
    std::shared_ptr<Line> line = Line::create(p1, p2);

    std::cout << "Line length: " << line->length() << std::endl;
    // // 测试点的坐标系变换
    std::shared_ptr<Frame> frame = std::make_shared<Frame>();
    frame->rotate(DegToRad(90.0), 0.0, 0.0); // 旋转90度绕X轴
    p1->transform(frame); // 将点p1转换到新的坐标系

    std::cout<<line->length()<<std::endl;
    return 0;
}