#ifndef MYMATH_H
#define MYMATH_H
#include <cmath>

constexpr double DegToRad(double deg) {
    return deg * 3.14159265358979323 / 180.0;
}

class vec2
{
public:
    vec2() : data{0.0, 0.0} {};
    vec2(double x, double y) : data{x, y} {};

    friend vec2 operator*(double scalar, const vec2& vec);

    double& operator[](std::size_t index) {
        return data[index];
    }

    const double& operator[](std::size_t index) const {
        return data[index];
    }

    vec2 operator+(const vec2& other) const {
        return vec2(data[0] + other.data[0], data[1] + other.data[1]);
    }

    vec2 operator-(const vec2& other) const {
        return vec2(data[0] - other.data[0], data[1] - other.data[1]);
    }

    vec2 operator*(double scalar) const {
        return vec2(data[0] * scalar, data[1] * scalar);
    }

    vec2 operator/(double scalar) const {
        return vec2(data[0] / scalar, data[1] / scalar);
    }

    double data[2];
};

vec2 operator*(double scalar, const vec2& vec) 
{
    return vec2(vec.data[0] * scalar, vec.data[1] * scalar);
}

class vec3
{
public:
    vec3() : data{0.0, 0.0, 0.0} {};
    vec3(double x,double y,double z) : data{x,y,z} {};

    friend vec3 operator*(double scalar, const vec3& vec);
    double& operator[](std::size_t index) {
        return data[index];
    }

    const double& operator[](std::size_t index) const {
        return data[index];
    }

    vec3 operator+(const vec3& other) const {
        vec3 result;
        for (std::size_t i = 0; i < 3; ++i) {
            result[i] = this->data[i] + other.data[i];
        }
        return result;
    }

    vec3 operator-(const vec3& other) const {
        vec3 result;
        for (std::size_t i = 0; i < 3; ++i) {
            result[i] = this->data[i] - other.data[i];
        }
        return result;
    }

    vec3 operator*(double scalar) const {
        vec3 result;
        for (std::size_t i = 0; i < 3; ++i) {
            result[i] = this->data[i] * scalar;
        }
        return result;
    }

    vec3 operator/(double scalar) const {
        vec3 result;
        for (std::size_t i = 0; i < 3; ++i) {
            result[i] = this->data[i] / scalar;
        }
        return result;
    }

    double dot(const vec3& other) const {
        double result {};
        for (std::size_t i = 0; i < 3; ++i) {
            result += this->data[i] * other.data[i];
        }
        return result;
    }

    vec3 cross(const vec3& other) const {
        vec3 result;
        result[0] = this->data[1] * other.data[2] - this->data[2] * other.data[1];
        result[1] = this->data[2] * other.data[0] - this->data[0] * other.data[2];
        result[2] = this->data[0] * other.data[1] - this->data[1] * other.data[0];
        return result;
    }
    //归一化
    vec3 normalize() const {
        double length = std::sqrt(this->dot(*this));
        if (length == 0) return vec3(0.0, 0.0, 0.0); // 避免除以零
        return *this / length;
    }

    void normalize()
    {
        double length = std::sqrt(this->dot(*this));
        if (length == 0) return; // 避免除以零
        for (std::size_t i = 0; i < 3; ++i) {
            data[i] /= length;
        }
    }
private:
    double data[3];
};

vec3 operator*(double scalar, const vec3& vec) 
{
    vec3 result;
    for (std::size_t i = 0; i < 3; ++i) {
        result[i] = vec.data[i] * scalar;
    }
    return result;
}

class Matrix3x3
{
public:
    Matrix3x3() : data{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},isDefined{false} {}
    Matrix3x3(vec3 v1, vec3 v2, vec3 v3):isDefined{true} {
        data[0][0] = v1[0]; data[0][1] = v1[1]; data[0][2] = v1[2];
        data[1][0] = v2[0]; data[1][1] = v2[1]; data[1][2] = v2[2];
        data[2][0] = v3[0]; data[2][1] = v3[1]; data[2][2] = v3[2];
    }
    double& operator()(std::size_t row, std::size_t col) {
        return data[row][col];
    }

    const double& operator()(std::size_t row, std::size_t col) const {
        return data[row][col];
    }

    vec3 operator[](std::size_t index) const {
        return vec3(data[index][0], data[index][1], data[index][2]);
    }

    Matrix3x3 operator*(const Matrix3x3& other) const {
        Matrix3x3 result;
        for (std::size_t i = 0; i < 3; ++i) {
            for (std::size_t j = 0; j < 3; ++j) {
                result(i, j) = this->data[i][0] * other(0, j) +
                               this->data[i][1] * other(1, j) +
                               this->data[i][2] * other(2, j);
            }
        }
        return result;
    }

    vec3 operator*(const vec3& vec) const {
        vec3 result;
        for (std::size_t i = 0; i < 3; ++i) {
            result[i] = this->data[i][0] * vec[0] +
                         this->data[i][1] * vec[1] +
                         this->data[i][2] * vec[2];
        }
        return result;
    }
//有bug，待修复
    Matrix3x3 reverse() const {
        Matrix3x3 result;
        double det = this->determinant();
        if (det == 0) return result; // 不可逆矩阵
        for (std::size_t i = 0; i < 3; ++i) {
            for (std::size_t j = 0; j < 3; ++j) {
                result(j, i) = this->cofactor(i, j) / det;
            }
        }
        return result;
    }

    //计算指定位置的余子式
    double cofactor(std::size_t row, std::size_t col) const {
        double minor[2][2];
        std::size_t m = 0, n = 0;
        for (std::size_t i = 0; i < 3; ++i) {
            if (i == row) continue;
            n = 0;
            for (std::size_t j = 0; j < 3; ++j) {
                if (j == col) continue;
                minor[m][n++] = data[i][j];
            }
            m++;
        }
        return minor[0][0] * minor[1][1] - minor[0][1] * minor[1][0];
    }

    // 计算矩阵的行列式
    double determinant() const {
        return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
               data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
               data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
    }
// private:
    double data[3][3];
    bool isDefined;
};

//定义四元数类
class Quaternion
{
public:
    Quaternion() : w_(1.0), v_(vec3{}) {} //单位四元数
    //四元数
    Quaternion(double w, vec3 v) : w_(w), v_(v) {}

    //四元数乘法
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w_ * other.w_ - v_.dot(other.v_),
            v_ * other.w_ + other.v_ * w_ + v_.cross(other.v_)
        );
    }

    Quaternion reverse() const {
        return Quaternion(w_, vec3{} -v_);
    }

    vec3 toVec3() const {
        return vec3(v_[0], v_[1], v_[2]);
    }
    
    double w_;
    vec3 v_;
};

class RotateQuaternion : public Quaternion
{
public:
    RotateQuaternion(double theta, vec3 v)
    {
        double halfTheta = theta / 2.0;
        w_ = std::cos(halfTheta);
        v.normalize(); // 确保向量是单位向量
        v_ = v * std::sin(halfTheta);
    }
};

#endif 
