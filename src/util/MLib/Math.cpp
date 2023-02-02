/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "Math.h"
#include <algorithm>
#include <cmath>

/*****************************************************************************/
std::vector<double> Math::LinearTransform(const std::vector<double>& x, double a,
                                          double b) {
    auto c = x.back();
    std::vector<double> result;
    result.reserve(x.size());
    auto factor = c + ((b - a) / (x[0] - c));

    for (auto& e : x) {
        result.push_back(factor * (e - c));
    }
    return result;
}

/*****************************************************************************/
std::vector<double> Math::DRange(double a, double b, int n) {
    std::vector<double> result;
    result.reserve(n);
    double step = (b - a) / n;
    double value = 0.0;

    for (long i = 0; i < n; ++i) {
        result.push_back(value);
        value += step;
    }
    return result;
}

/*****************************************************************************/
std::vector<long> Math::IRange(long a, long b, int n) {
    std::vector<long> result;
    result.reserve(n);
    long step = static_cast<long>((b - a) / n);
    long value = 0;

    for (long i = 0; i < n; ++i) {
        result.push_back(value);
        value += step;
    }
    return result;
}

/*****************************************************************************/
/*****************************************************************************/
Vector::Vector(size_t n, double init) : _size(n) {
    _data = new double[n];
    for (size_t i = 0; i < n; ++i) {
        _data[i] = init;
    }
}

/*****************************************************************************/
Vector::Vector(const std::vector<double>& data) : _size(data.size()) {
    _data = new double[_size];
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = data[i];
    }
}

/*****************************************************************************/
Vector::Vector(double* data, size_t n) : _size(n) {
    _data = data;
}

/*****************************************************************************/
Vector::~Vector() {
    delete[] _data;
}

/*****************************************************************************/
Vector* Vector::Zeros(size_t n) {
    return new Vector(n, 0.0);
}

/*****************************************************************************/
double* Vector::GetData() const {
    return _data;
}

/*****************************************************************************/
Vector* Vector::operator*(Vector* vec) const {
    return Cross(vec);
}

/*****************************************************************************/
Vector* Vector::operator*(double x) {
    auto result = new double[_size];

    for (size_t i = 0; i < _size; ++i) {
        result[i] = _data[i] * x;
    }
    return new Vector(result, _size);
}

/*****************************************************************************/
Vector* Vector::operator+(Vector* vec) {
    auto vecData = vec->GetData();
    assert(_size == vec->_size);
    auto* result = new double[_size];

    for (size_t i = 0; i < _size; ++i) {
        result[i] = _data[i] + vecData[i];
    }
    return new Vector(result, _size);
}

/*****************************************************************************/
double Vector::Dot(Vector* vec) const {
    auto vecData = vec->GetData();
    assert(_size == vec->Size());
    double result = 0.0;

    for (size_t i = 0; i < _size; ++i) {
        result += _data[i] * vecData[i];
    }
    return result;
}

/*****************************************************************************/
Vector* Vector::Cross(Vector* vec) const {
    return nullptr;
}

/*****************************************************************************/
double Vector::Norm() const {
    double sumsSquared = 0.0;

    for (size_t i = 0; i < _size; ++i) {
        sumsSquared += pow(_data[i], 2);
    }
    return sqrt(sumsSquared);
}

/*****************************************************************************/
double Vector::Cosine(Vector* vec) const {
    return this->Dot(vec) / (this->Norm() * vec->Norm());
}

/*****************************************************************************/
double Vector::Angle(Vector* vec) const {
    return acos(Cosine(vec));
}

/*****************************************************************************/
size_t Vector::Size() const {
    return _size;
}
