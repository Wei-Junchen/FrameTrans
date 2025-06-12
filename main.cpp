#include <iostream>
#include <memory>
#include "all.h"

int main()
{
    auto p1 = std::make_shared<Point>(1.0, 2.0, 3.0);
    auto p2 = std::make_shared<Point>(4.0, 5.0, 6.0);
    // std::shared_ptr<Line> line = Line::create(p1, p2);

    // std::cout << "Line length: " << line->length() << std::endl;
    // std::cout<<p1->lines_.size()<<" "<<p2->lines_.size()<<std::endl;
    // // 测试点的坐标系变换
    std::shared_ptr<Frame> frame = std::make_shared<Frame>();
    frame->rotate(DegToRad(90.0), 0.0, 0.0); // 旋转90度绕X轴
    p1->transform(frame); // 将点p1转换到新的坐标系
    std::cout << "Point p1 position after transform: (" 
              << p1->position_[0] << ", " 
              << p1->position_[1] << ", " 
              << p1->position_[2] << ")" << std::endl;

    // std::cout<<line->length()<<std::endl;

    // //定义坐标系
    // std::shared_ptr<Frame> frame = std::make_shared<Frame>();
    // std::shared_ptr<Frame> frame1 = std::make_shared<Frame>(); // 默认坐标系
    // frame->rotate(DegToRad(90.0), 0.0, 0.0); // 旋转45度绕X轴，30度绕Y轴，60度绕Z轴
    // frame1->rotate(DegToRad(0.0),DegToRad(90.0),0.0,frame); // 旋转90度绕Y轴，绕当前坐标系旋转
    
    // // 输出当前坐标系的基向量
    // std::cout << "Frame bases after rotation:" << std::endl;
    // for (int i = 0; i < 3; ++i) {
    //     std::cout << "Base " << i << ": (" 
    //               << frame->bases[i][0] << ", " 
    //               << frame->bases[i][1] << ", " 
    //               << frame->bases[i][2] << ")" << std::endl;
    // }

    // //基于四元数旋转测试
    // RotateQuaternion q = {DegToRad(45.0), vec3(0.0, 0.0, 1.0)};
    // std::shared_ptr<Frame> frame2 = std::make_shared<Frame>();
    // frame2->rotate(q); // 在frame1坐标系下旋转45度绕Z轴

    // // 输出旋转后的基向量
    // std::cout << "Frame2 bases after quaternion rotation:" << std::endl;
    // for (int i = 0; i < 3; ++i) {
    //     std::cout << "Base " << i << ": (" 
    //               << frame2->bases[i][0] << ", " 
    //               << frame2->bases[i][1] << ", " 
    //               << frame2->bases[i][2] << ")" << std::endl;
    // }
    return 0;
}