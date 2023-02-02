/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef MLIB_MATH_H
#define MLIB_MATH_H

#include <vector>

/*****************************************************************************/
class Math {
public:
    static std::vector<double> LinearTransform(const std::vector<double>& data,
                                               double lower, double upper);
    static std::vector<double> DRange(double a, double b, int n);
    static std::vector<long> IRange(long a, long b, int n);
};

/*****************************************************************************/
class Vector {
public:
    explicit Vector(size_t n, double init = 0.0);
    explicit Vector(const std::vector<double>& data);
    explicit Vector(double* data, size_t n);
    ~Vector();
    [[nodiscard]] static Vector* Zeros(size_t n);
    [[nodiscard]] double* GetData() const;
    Vector* operator*(Vector* vec) const;
    Vector* operator*(double x);
    Vector* operator+(Vector* vec);
    [[nodiscard]] double Dot(Vector* vec) const;
    [[nodiscard]] Vector* Cross(Vector* vec) const;
    [[nodiscard]] double Norm() const;
    [[nodiscard]] double Cosine(Vector* vec) const;
    [[nodiscard]] double Angle(Vector* vec) const;
    [[nodiscard]] size_t Size() const;

private:
    double* _data;
    size_t _size;
};

#endif  //MLIB_MATH_H