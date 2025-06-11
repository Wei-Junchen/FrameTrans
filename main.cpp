#include <iostream>
#include <memory>
#include "frame.h"

int main()
{
    //定义坐标系
    std::shared_ptr<Frame> frame = std::make_shared<Frame>();
    std::shared_ptr<Frame> frame1 = std::make_shared<Frame>(); // 默认坐标系
    frame->rotate(DegToRad(90.0), 0.0, 0.0); // 旋转45度绕X轴，30度绕Y轴，60度绕Z轴
    frame1->rotate(DegToRad(0.0),DegToRad(90.0),0.0,frame); // 旋转90度绕Y轴，绕当前坐标系旋转
    
    // 输出当前坐标系的基向量
    std::cout << "Frame bases after rotation:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "Base " << i << ": (" 
                  << frame->bases[i][0] << ", " 
                  << frame->bases[i][1] << ", " 
                  << frame->bases[i][2] << ")" << std::endl;
    }

    //基于四元数旋转测试
    RotateQuaternion q = {DegToRad(45.0), vec3(0.0, 0.0, 1.0)};
    std::shared_ptr<Frame> frame2 = std::make_shared<Frame>();
    frame2->rotate(q); // 在frame1坐标系下旋转45度绕Z轴

    // 输出旋转后的基向量
    std::cout << "Frame2 bases after quaternion rotation:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "Base " << i << ": (" 
                  << frame2->bases[i][0] << ", " 
                  << frame2->bases[i][1] << ", " 
                  << frame2->bases[i][2] << ")" << std::endl;
    }
    return 0;
}