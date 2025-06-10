#include <iostream>
#include <memory>
#include "frame.h"

int main()
{
//定义坐标系
    std::shared_ptr<Frame> frame,frame1; // 默认坐标系
    frame->rotate(DegToRad(90.0), 0.0, 0.0); // 旋转45度绕X轴，30度绕Y轴，60度绕Z轴
    frame1->rotate(DegToRad(0.0),DegToRad(90.0),0.0,frame); // 旋转90度绕Y轴，绕当前坐标系旋转
    
    return 0;
}