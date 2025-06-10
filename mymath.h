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
    Matrix3x3() : data{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}} {}
    Matrix3x3(vec3 v1, vec3 v2, vec3 v3) {
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
private:
    double data[3][3];
};

#endif 
