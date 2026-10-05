#pragma once
#include <algorithm>
#include <array>
#include <cmath>

#include "common_concepts.hpp"

namespace zuc {

struct Vector2D {
    double x_ = 0;
    double y_ = 0;

    template <typename T>
        requires NumericType<T>
    Vector2D operator*(T k) const {
        return {x_ * k, y_ * k};
    };

    template <typename T>
        requires NumericType<T>
    Vector2D& operator*=(T k) {
        x_ *= k, y_ *= k;
        return *this;
    };

    Vector2D operator+(const Vector2D& v) const {
        return {x_ + v.x_, y_ + v.y_};
    }
    Vector2D operator-(const Vector2D& v) const {
        return {x_ - v.x_, y_ - v.y_};
    }
    Vector2D& operator+=(const Vector2D& v) {
        x_ += v.x_, y_ += v.y_;
        return *this;
    }
    Vector2D& operator-=(const Vector2D& v) {
        x_ -= v.x_, y_ -= v.y_;
        return *this;
    }
    template <typename T>
        requires NumericType<T>
    Vector2D operator/(T k) const {
        if (k == 0) {
            return {0, 0};
        }
        return {x_ / k, y_ / k};
    };
    template <typename T>
        requires NumericType<T>
    void operator/=(T k) {
        if (k == 0) {
            x_ = 0, y_ = 0;
            return;
        }
        x_ /= k, y_ /= k;
    };

    void normalize() { *this = *this / size(); }

    double size() const { return std::sqrt(x_ * x_ + y_ * y_); }
};

inline double get_dot_product(const Vector2D& v1, const Vector2D& v2) {
    return v1.x_ * v2.x_ + v1.y_ * v2.y_;
}

struct Vector3D {
    double x_ = 0;
    double y_ = 0;
    double z_ = 0;
    template <typename T>
        requires NumericType<T>
    Vector3D operator*(T k) const {
        return {x_ * k, y_ * k, z_ * k};
    };
    template <typename T>
        requires NumericType<T>
    Vector3D& operator*=(T k) {
        x_ *= k, y_ *= k, z_ *= k;
        return *this;
    };
    template <typename T>
        requires NumericType<T>
    Vector3D operator/(T k) const {
        if (k == 0) {
            return {0, 0, 0};
        }
        return {x_ / k, y_ / k, z_ / k};
    };
    template <typename T>
        requires NumericType<T>
    Vector3D& operator/=(T k) {
        if (k == 0) {
            x_ = 0, y_ = 0, z_ = 0;
            return *this;
        }
        x_ /= k, y_ /= k, z_ /= k;
        return *this;
    };

    Vector3D operator+(const Vector3D& v) const {
        return {x_ + v.x_, y_ + v.y_, z_ + v.z_};
    }
    Vector3D operator-(const Vector3D& v) const {
        return {x_ - v.x_, y_ - v.y_, z_ - v.z_};
    }
    Vector3D& operator+=(const Vector3D& v) {
        x_ += v.x_, y_ += v.y_, z_ += v.z_;
        return *this;
    }
    Vector3D& operator-=(const Vector3D& v) {
        x_ -= v.x_, y_ -= v.y_, z_ -= v.z_;
        return *this;
    }
    double size() const { return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); }
    void normalize() { *this = *this / size(); }
};

inline Vector3D get_cross_product(const Vector3D& v1, const Vector3D& v2) {
    return {v1.y_ * v2.z_ - v1.z_ * v2.y_, v1.z_ * v2.x_ - v1.x_ * v2.z_,
            v1.x_ * v2.y_ - v1.y_ * v2.x_};
}

inline double get_dot_product(const Vector3D& v1, const Vector3D& v2) {
    return v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
}

inline Vector3D get_normalized(const Vector3D& v) { return v / v.size(); }
inline Vector2D get_normalized(const Vector2D& v) { return v / v.size(); }

struct Vector4D {
    double x_ = 0;
    double y_ = 0;
    double z_ = 0;
    double w_ = 0;
    template <typename T>
        requires NumericType<T>
    Vector4D operator*(T k) const {
        return {x_ * k, y_ * k, z_ * k, w_ * k};
    };
    template <typename T>
        requires NumericType<T>
    Vector4D& operator*=(T k) {
        x_ *= k, y_ *= k, z_ *= k, w_ *= k;
        return *this;
    };
    template <typename T>
        requires NumericType<T>
    Vector4D operator/(T k) const {
        if (k == 0) {
            return {0, 0, 0, 0};
        }
        return {x_ / k, y_ / k, z_ / k, w_ / k};
    };
    template <typename T>
        requires NumericType<T>
    Vector4D& operator/=(T k) {
        if (k == 0) {
            x_ = 0, y_ = 0, z_ = 0, w_ = 0;
            return *this;
        }
        x_ /= k, y_ /= k, z_ /= k, w_ /= k;
        return *this;
    };
    Vector4D operator+(const Vector4D& v) const {
        return {x_ + v.x_, y_ + v.y_, z_ + v.z_, w_ + v.w_};
    }
    Vector4D operator-(const Vector4D& v) const {
        return {x_ - v.x_, y_ - v.y_, z_ - v.z_, w_ - v.w_};
    }
    Vector4D& operator+=(const Vector4D& v) {
        x_ += v.x_, y_ += v.y_, z_ += v.z_, w_ += v.w_;
        return *this;
    }
    Vector4D& operator-=(const Vector4D& v) {
        x_ -= v.x_, y_ -= v.y_, z_ -= v.z_, w_ -= v.w_;
        return *this;
    }
    double size() const {
        return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
    }
    void normalize() { *this = *this / size(); }
};
inline Vector4D get_normalized(const Vector4D& v) { return v / v.size(); }

struct Point2D {
    double x_ = 0;
    double y_ = 0;
};

struct Point3D {
    double x_ = 0;
    double y_ = 0;
    double z_ = 0;
};

struct Rect2D {
    double x_ = 0;
    double y_ = 0;
    double w_ = 0;
    double h_ = 0;
    inline double get_bottom() const { return y_ + h_; }
    inline double get_right() const { return x_ + w_; }
    inline double get_top() const { return y_; }
    inline double get_left() const { return x_; }
    inline double get_area() const { return w_ * h_; }
    inline bool contains_point(const Point2D& p) const {
        return x_ <= p.x_ && p.x_ <= x_ + w_ && y_ <= p.y_ && p.y_ <= y_ + h_;
    }
};

inline bool intersect(const Rect2D& r1, const Rect2D& r2) {
    return r1.get_right() > r2.get_left() && r1.get_top() < r2.get_bottom() &&
           r1.get_left() < r2.get_right() && r1.get_bottom() > r2.get_top();
}

inline Rect2D merge(const Rect2D& r1, const Rect2D& r2) {
    return {std::min(r1.get_left(), r2.get_left()),
            std::min(r1.get_top(), r2.get_top()),
            std::max(r1.get_right(), r2.get_right()) -
                std::min(r1.get_left(), r2.get_left()),
            std::max(r1.get_bottom(), r2.get_bottom()) -
                std::min(r1.get_top(), r2.get_top())};
}

/*
    col0 col1 col2
row0 [ a    b    c ]
row1 [ d    e    f ]
row2 [ g    h    i ]
NOTICE: row major order
*/

struct Matrix3 {
    std::array<double, 9> data_;
    inline double get(int row, int col) const { return data_[row * 3 + col]; }
    inline void set(int row, int col, double v) { data_[row * 3 + col] = v; }
    inline Matrix3& operator=(const Matrix3& m) {
        for (int i = 0; i < 9; i++) {
            data_[i] = m.data_[i];
        }
        return *this;
    }
    inline std::array<double, 3> get_row(int row) const {
        return {data_[row * 3], data_[row * 3 + 1], data_[row * 3 + 2]};
    }
    inline std::array<double, 3> get_col(int col) const {
        return {data_[col], data_[col + 3], data_[col + 6]};
    }

    inline Matrix3& operator*=(const Matrix3& m) {
        Matrix3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                double sum = 0;
                for (int k = 0; k < 3; k++)
                    sum += this->get(i, k) * m.get(k, j);
                result.set(i, j, sum);
            }
        *this = result;
        return *this;
    }
    inline Matrix3 operator*(const Matrix3& m) const {
        Matrix3 res = *this;
        res *= m;
        return res;
    }
    Vector3D operator*(const Vector3D& v) const {
        return Vector3D{get(0, 0) * v.x_ + get(0, 1) * v.y_ + get(0, 2) * v.z_,
                        get(1, 0) * v.x_ + get(1, 1) * v.y_ + get(1, 2) * v.z_,
                        get(2, 0) * v.x_ + get(2, 1) * v.y_ + get(2, 2) * v.z_};
    }
};

struct Matrix4 {
    std::array<double, 16> data_;
    inline double get(int row, int col) const { return data_[row * 4 + col]; }
    inline void set(int row, int col, double v) { data_[row * 4 + col] = v; }
    inline Matrix4& operator=(const Matrix4& m) {
        for (int i = 0; i < 16; i++) {
            data_[i] = m.data_[i];
        }
        return *this;
    }
    inline std::array<double, 4> get_row(int row) const {
        return {data_[row * 4], data_[row * 4 + 1], data_[row * 4 + 2],
                data_[row * 4 + 3]};
    }
    inline std::array<double, 4> get_col(int col) const {
        return {data_[col], data_[col + 4], data_[col + 8], data_[col + 12]};
    }
    inline Matrix4& operator*=(const Matrix4& m) {
        // a[i][j] = b[i][k] * c[k][j]
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                double sum = 0;
                for (int k = 0; k < 4; k++) {
                    sum += this->get(i, k) * m.get(k, j);
                }
                result.set(i, j, sum);
            }
        }
        *this = result;
        return *this;
    }
    inline Matrix4 operator*(const Matrix4& m) const {
        Matrix4 res = *this;
        res *= m;
        return res;
    }
    Vector4D operator*(const Vector4D& v) const {
        return Vector4D{get(0, 0) * v.x_ + get(0, 1) * v.y_ + get(0, 2) * v.z_ +
                            get(0, 3) * v.w_,
                        get(1, 0) * v.x_ + get(1, 1) * v.y_ + get(1, 2) * v.z_ +
                            get(1, 3) * v.w_,
                        get(2, 0) * v.x_ + get(2, 1) * v.y_ + get(2, 2) * v.z_ +
                            get(2, 3) * v.w_,
                        get(3, 0) * v.x_ + get(3, 1) * v.y_ + get(3, 2) * v.z_ +
                            get(3, 3) * v.w_};
    }
};

}  // namespace zuc