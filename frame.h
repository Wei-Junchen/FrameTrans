#ifndef FRAME_H
#define FRAME_H

#include "mymath.h"
#include <memory>
#include <array>

//坐标系类,所有坐标系原点在(0,0,0),基向量为单位向量
class Frame
{
public:
    static std::shared_ptr<const Frame> getBaseFrame() {
        static std::shared_ptr<const Frame> basePtr = std::make_shared<const Frame>();
        return basePtr;
    }    

    // 默认构造函数，创建一个单位坐标系
    Frame() : bases{vec3{1.0, 0.0, 0.0}, vec3{0.0, 1.0, 0.0}, vec3{0.0, 0.0, 1.0}} {}

    Frame(vec3 v1,vec3 v2,vec3 v3) : bases{v1, v2, v3} 
    {
        // 确保基向量是正交的
        bases[1] = bases[1] - bases[1].dot(bases[0]) * bases[0];
        bases[2] = bases[2] - bases[2].dot(bases[0]) * bases[0];
        bases[2] = bases[2] - bases[2].dot(bases[1]) * bases[1];
        // 归一化基向量
        for(auto& base : bases) {
            base.normalize();
        }
    }

    //角度定义坐标系
    Frame(double theta_x, double theta_y, double theta_z) 
    {
        // 计算旋转矩阵
        Matrix3x3 rotation_x = {
            {1.0, 0.0, 0.0},
            {0.0, std::cos(theta_x), -std::sin(theta_x)},
            {0.0, std::sin(theta_x), std::cos(theta_x)}
        };
        Matrix3x3 rotation_y = {
            {std::cos(theta_y), 0.0, std::sin(theta_y)},
            {0.0, 1.0, 0.0},
            {-std::sin(theta_y), 0.0, std::cos(theta_y)}
        };
        Matrix3x3 rotation_z = {
            {std::cos(theta_z), -std::sin(theta_z), 0.0},
            {std::sin(theta_z), std::cos(theta_z), 0.0},
            {0.0, 0.0, 1.0}
        };
        // 总旋转矩阵
        Matrix3x3 rotation = rotation_z * rotation_y * rotation_x;
        // 设置基向量
        bases[0] = rotation * vec3(1.0, 0.0, 0.0);
        bases[1] = rotation * vec3(0.0, 1.0, 0.0);
        bases[2] = rotation * vec3(0.0, 0.0, 1.0);
    }

    //当前坐标系相对于另一个坐标系(S)进行旋转(绕Sx轴,Sy轴,Sz轴顺序旋转),缺省为默认坐标系
    void rotate(double theta_x,double theta_y,double theta_z,const std::shared_ptr<const Frame> other = Frame::getBaseFrame())
    {
        //先计算当前坐标系在另一个坐标系下的基向量
        vec3 newBase[3];
        for (int i = 0; i < 3; ++i) {
            newBase[i] = {bases[i].dot(other->bases[0]),
                          bases[i].dot(other->bases[1]),
                          bases[i].dot(other->bases[2])};
        }

        //计算旋转矩阵
        Matrix3x3 rotation_x = {
            {1.0, 0.0, 0.0},
            {0.0, std::cos(theta_x), -std::sin(theta_x)},
            {0.0, std::sin(theta_x), std::cos(theta_x)}
        };
        Matrix3x3 rotation_y = {
            {std::cos(theta_y), 0.0, std::sin(theta_y)},
            {0.0, 1.0, 0.0},
            {-std::sin(theta_y), 0.0, std::cos(theta_y)}
        };
        Matrix3x3 rotation_z = {
            {std::cos(theta_z), -std::sin(theta_z), 0.0},
            {std::sin(theta_z), std::cos(theta_z), 0.0},
            {0.0, 0.0, 1.0}
        };
        //总旋转矩阵
        Matrix3x3 rotation = rotation_z * rotation_y * rotation_x;
        //对当前坐标系的基向量进行旋转
        vec3 rotatedBases[3];
        for (int i = 0; i < 3; ++i) {
            rotatedBases[i] = rotation * newBase[i];
        }
        
        //更新当前坐标系的基向量(还要转回基准坐标系)
        for (int i = 0; i < 3; ++i) {
            bases[i] =  rotatedBases[i][0] * other->bases[0] +
                        rotatedBases[i][1] * other->bases[1] +
                        rotatedBases[i][2] * other->bases[2];
            //归一化基向量
            bases[i].normalize();
        }
    }

    //基于四元数的旋转
    void rotate(const RotateQuaternion& q, const std::shared_ptr<const Frame> other = Frame::getBaseFrame())
    {
        //先计算当前坐标系在另一个坐标系下的基向量
        vec3 newBase[3];
        for (int i = 0; i < 3; ++i) {
            newBase[i] = {bases[i].dot(other->bases[0]),
                          bases[i].dot(other->bases[1]),
                          bases[i].dot(other->bases[2])};
        }

        //对当前坐标系的基向量进行旋转
        vec3 rotatedBases[3];
        for (int i = 0; i < 3; ++i) {
            rotatedBases[i] = (q * Quaternion(0.0, newBase[i]) * q.reverse()).toVec3();
        }
        
        //更新当前坐标系的基向量(还要转回基准坐标系)
        for (int i = 0; i < 3; ++i) {
            bases[i] =  rotatedBases[i][0] * other->bases[0] +
                        rotatedBases[i][1] * other->bases[1] +
                        rotatedBases[i][2] * other->bases[2];
            //归一化基向量
            bases[i].normalize();
        }
    }
    void print() const
    {
        for (int i = 0; i < 3; ++i) {
            std::cout << "Base " << i << ": (" 
                      << bases[i][0] << ", " 
                      << bases[i][1] << ", " 
                      << bases[i][2] << ")" << std::endl;
        }
    }

    //当前坐标系相对于基准坐标系(基准坐标系为右手坐标系)的三个基向量
    std::array<vec3,3> bases;
};

#endif