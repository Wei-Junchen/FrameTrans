#ifndef MYMATH_H
#define MYMATH_H

#include <cmath>
#include <initializer_list>
#include <exception>
#include <optional>

constexpr double DegToRad(double deg) {
    return deg * 3.14159265358979323 / 180.0;
}

enum class vectorType
{
    RowVector, // 行向量
    ColumnVector // 列向量
};

template<std::size_t M>
class vec
{
    friend vec<M> operator*(double scalar, const vec<M>& v)
    {
        vec<M> result;
        for(std::size_t i = 0; i < M; ++i) {
            result[i] = v.data[i] * scalar;
        }
        return result;
    }
    friend vec<M> operator/(double scalar, const vec<M>& v)
    {
        if(scalar == 0) {
            throw std::invalid_argument("Division by zero");
        }
        vec<M> result;
        for(std::size_t i = 0; i < M; ++i) {
            result[i] = scalar / v.data[i];
        }
        return result;
    }
    friend vec<M> operator-(const vec<M>& v)
    {
        vec<M> result;
        for(std::size_t i = 0; i < M; ++i) {
            result[i] = -v.data[i];
        }
        return result;
    }
public:
    vec() = default;
    //列表初始化构造函数
    vec(std::initializer_list<double> list,vectorType type = vectorType::RowVector) : type_(type)
    {
        std::size_t i = 0;
        for(auto it = list.begin(); it != list.end() && i < M; ++it, ++i) {
            data[i] = *it;
        }
    }
    double& operator[](std::size_t index) 
    {
        if(index >= M) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    double operator[](std::size_t index) const 
    {
        if(index >= M) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    vec<M> operator+(const vec<M>& other) const 
    {
        if(other.type_ != type_) {
            throw std::invalid_argument("Cannot add vectors of different types");
        }
        vec<M> result;
        for(std::size_t i = 0; i < M; ++i) {
            result[i] = data[i] + other.data[i];
        }
        return result;
    }
    vec<M> operator-(const vec<M>& other) const 
    {
        if(other.type_ != type_) {
            throw std::invalid_argument("Cannot subtract vectors of different types");
        }
        vec<M> result;
        for(std::size_t i = 0; i < M; ++i) {
            result[i] = data[i] - other.data[i];
        }
        return result;
    }
    vec<M> operator*(double scalar) const 
    {
        vec<M> result;
        for(std::size_t i = 0; i < M; ++i) {
            result[i] = data[i] * scalar;
        }
        return result;
    }
    vec<M> operator/(double scalar) const 
    {
        if(scalar == 0) {
            throw std::invalid_argument("Division by zero");
        }
        vec<M> result;
        for(std::size_t i = 0; i < M; ++i) {
            result[i] = data[i] / scalar;
        }
        return result;
    }
    double dot(const vec<M>& other) const 
    {
        double result = 0.0;
        for(std::size_t i = 0; i < M; ++i) {
            result += data[i] * other.data[i];
        }
        return result;
    }
    vec<M> normalize() const 
    {
        double length = 0.0;
        for(std::size_t i = 0; i < M; ++i) {
            length += data[i] * data[i];
        }
        length = std::sqrt(length);
        if(length == 0) return vec<M>{}; // 避免除以零
        vec<M> result;
        for(std::size_t i = 0; i < M; ++i) {
            result[i] = data[i] / length;
        }
        return result;
    }
    void normalize() 
    {
        double length = 0.0;
        for(std::size_t i = 0; i < M; ++i) {
            length += data[i] * data[i];
        }
        length = std::sqrt(length);
        if(length == 0) return; // 避免除以零
        for(std::size_t i = 0; i < M; ++i) {
            data[i] /= length;
        }
    }
    vec<M> transpose() const
    {
        vec<M> result;
        for(std::size_t i = 0; i < M; ++i) {
            result[i] = data[i]; // 对于向量，转置操作实际上没有改变
        }
        if(type_ == vectorType::RowVector) {
            result.type_ = vectorType::ColumnVector;
        } else {
            result.type_ = vectorType::RowVector;
        }
        return result;
    }
    //行列转置
    void transpose()
    {
        if(type_ == vectorType::RowVector) {
            type_ = vectorType::ColumnVector;
        } else {
            type_ = vectorType::RowVector;
        }
    }
    double norm() const 
    {
        double length = 0.0;
        for(std::size_t i = 0; i < M; ++i) {
            length += data[i] * data[i];
        }
        return std::sqrt(length);
    }
    vectorType type() const 
    {
        return type_;
    }
protected:
    //是行向量还是列向量
    vectorType type_ {vectorType::RowVector}; // 默认是行向量
    double data[M] {}; // 使用数组存储数据
};

class vec3 : public vec<3>
{
public:
    vec3() = default;
    vec3(double x, double y, double z) : vec<3>{{x, y, z}} {}
    vec3(const vec<3>& v) : vec<3>(v) {}
    // 计算向量的叉积
    vec3 cross(const vec3& other) const {
        return vec3(
            data[1] * other.data[2] - data[2] * other.data[1],
            data[2] * other.data[0] - data[0] * other.data[2],
            data[0] * other.data[1] - data[1] * other.data[0]
        );
    }
};

using vec2 = vec<2>; // 定义二维向量类型

//定义四元数类
class Quaternion
{
public:
    Quaternion() = default;
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
        return Quaternion(w_, -v_);
    }

    vec3 toVec3() const {
        return vec3(v_[0], v_[1], v_[2]);
    }
protected:
    double w_ {1.0};
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

template<std::size_t M,std::size_t N>
class Matrix
{
public:
    Matrix() = default;
    
private:
    double data[M][N] {}; 
};

class Matrix3x3
{
public:
    Matrix3x3() : data{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}{}
    Matrix3x3(vec3 v1, vec3 v2, vec3 v3){
        if(v1.type() != v2.type() || v1.type() != v3.type()) {
            throw std::invalid_argument("Vectors must be of the same type (RowVector or ColumnVector)");
        }
        if(v1.type() == vectorType::RowVector)
        {
            data[0][0] = v1[0]; data[0][1] = v1[1]; data[0][2] = v1[2];
            data[1][0] = v2[0]; data[1][1] = v2[1]; data[1][2] = v2[2];
            data[2][0] = v3[0]; data[2][1] = v3[1]; data[2][2] = v3[2];
        }
        else
        {
            data[0][0] = v1[0]; data[0][1] = v2[0]; data[0][2] = v3[0];
            data[1][0] = v1[1]; data[1][1] = v2[1]; data[1][2] = v3[1];
            data[2][0] = v1[2]; data[2][1] = v2[2]; data[2][2] = v3[2];
        }
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

    std::optional<Matrix3x3> reverse() const {
        double det = determinant();
        if (det == 0) {
            return std::nullopt; // 矩阵不可逆
        }
        Matrix3x3 inv;
        for (std::size_t i = 0; i < 3; ++i) {
            for (std::size_t j = 0; j < 3; ++j) {
                inv(i, j) = cofactor(j, i) / det * ((i + j) % 2 == 0 ? 1 : -1);
            }
        }
        return inv;
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

    void print() const {
        for (std::size_t i = 0; i < 3; ++i) {
            for (std::size_t j = 0; j < 3; ++j) {
                std::cout << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
// private:
    double data[3][3];
};

#endif 