#include <cmath>
#include <cstdlib>

#include "doctest.h"
#include "include/maths_kits.hpp"


using namespace zuc;

constexpr double EPS = 1e-9;

static bool approx(double a, double b, double eps = EPS) {
    return std::abs(a - b) < eps;
}

// =============================================================================
// Vector2D
// =============================================================================
TEST_SUITE("Vector2D") {
    TEST_CASE("default construction") {
        Vector2D v;
        CHECK(approx(v.x_, 0.0));
        CHECK(approx(v.y_, 0.0));
    }

    TEST_CASE("scalar multiplication") {
        Vector2D v{3.0, 4.0};
        auto r = v * 2.0;
        CHECK(approx(r.x_, 6.0));
        CHECK(approx(r.y_, 8.0));
    }

    TEST_CASE("scalar multiplication compound") {
        Vector2D v{3.0, 4.0};
        v *= 2.0;
        CHECK(approx(v.x_, 6.0));
        CHECK(approx(v.y_, 8.0));
    }

    TEST_CASE("addition and subtraction") {
        Vector2D a{1.0, 2.0}, b{3.0, 4.0};
        auto sum = a + b;
        CHECK(approx(sum.x_, 4.0));
        CHECK(approx(sum.y_, 6.0));

        auto diff = b - a;
        CHECK(approx(diff.x_, 2.0));
        CHECK(approx(diff.y_, 2.0));
    }

    TEST_CASE("compound addition and subtraction") {
        Vector2D a{1.0, 2.0}, b{3.0, 4.0};
        a += b;
        CHECK(approx(a.x_, 4.0));
        CHECK(approx(a.y_, 6.0));

        a -= b;
        CHECK(approx(a.x_, 1.0));
        CHECK(approx(a.y_, 2.0));
    }

    TEST_CASE("division by scalar") {
        Vector2D v{6.0, 8.0};
        auto r = v / 2.0;
        CHECK(approx(r.x_, 3.0));
        CHECK(approx(r.y_, 4.0));
    }

    TEST_CASE("division by zero returns zero vector") {
        Vector2D v{3.0, 4.0};
        auto r = v / 0;
        CHECK(approx(r.x_, 0.0));
        CHECK(approx(r.y_, 0.0));
    }

    TEST_CASE("compound division by scalar") {
        Vector2D v{6.0, 8.0};
        v /= 2.0;
        CHECK(approx(v.x_, 3.0));
        CHECK(approx(v.y_, 4.0));
    }

    TEST_CASE("compound division by zero sets zero vector") {
        Vector2D v{3.0, 4.0};
        v /= 0;
        CHECK(approx(v.x_, 0.0));
        CHECK(approx(v.y_, 0.0));
    }

    TEST_CASE("size") {
        Vector2D v{3.0, 4.0};
        CHECK(approx(v.size(), 5.0));
    }

    TEST_CASE("normalize") {
        Vector2D v{3.0, 4.0};
        v.normalize();
        CHECK(approx(v.size(), 1.0));
        CHECK(approx(v.x_, 0.6));
        CHECK(approx(v.y_, 0.8));
    }

    TEST_CASE("dot product") {
        Vector2D a{1.0, 2.0}, b{3.0, 4.0};
        CHECK(approx(get_dot_product(a, b), 11.0));
    }

    TEST_CASE("get_normalized") {
        Vector2D v{3.0, 4.0};
        auto n = get_normalized(v);
        CHECK(approx(n.size(), 1.0));
        CHECK(approx(n.x_, 0.6));
        CHECK(approx(n.y_, 0.8));
    }
}

// =============================================================================
// Vector3D
// =============================================================================
TEST_SUITE("Vector3D") {
    TEST_CASE("default construction") {
        Vector3D v;
        CHECK(approx(v.x_, 0.0));
        CHECK(approx(v.y_, 0.0));
        CHECK(approx(v.z_, 0.0));
    }

    TEST_CASE("scalar multiplication") {
        Vector3D v{1.0, 2.0, 3.0};
        auto r = v * 3.0;
        CHECK(approx(r.x_, 3.0));
        CHECK(approx(r.y_, 6.0));
        CHECK(approx(r.z_, 9.0));
    }

    TEST_CASE("scalar multiplication compound") {
        Vector3D v{1.0, 2.0, 3.0};
        v *= 3.0;
        CHECK(approx(v.x_, 3.0));
        CHECK(approx(v.y_, 6.0));
        CHECK(approx(v.z_, 9.0));
    }

    TEST_CASE("addition and subtraction") {
        Vector3D a{1.0, 2.0, 3.0}, b{4.0, 5.0, 6.0};
        auto sum = a + b;
        CHECK(approx(sum.x_, 5.0));
        CHECK(approx(sum.y_, 7.0));
        CHECK(approx(sum.z_, 9.0));

        auto diff = b - a;
        CHECK(approx(diff.x_, 3.0));
        CHECK(approx(diff.y_, 3.0));
        CHECK(approx(diff.z_, 3.0));
    }

    TEST_CASE("compound addition and subtraction") {
        Vector3D a{1.0, 2.0, 3.0}, b{4.0, 5.0, 6.0};
        a += b;
        CHECK(approx(a.x_, 5.0));
        CHECK(approx(a.y_, 7.0));
        CHECK(approx(a.z_, 9.0));

        a -= b;
        CHECK(approx(a.x_, 1.0));
        CHECK(approx(a.y_, 2.0));
        CHECK(approx(a.z_, 3.0));
    }

    TEST_CASE("division by scalar") {
        Vector3D v{6.0, 9.0, 12.0};
        auto r = v / 3.0;
        CHECK(approx(r.x_, 2.0));
        CHECK(approx(r.y_, 3.0));
        CHECK(approx(r.z_, 4.0));
    }

    TEST_CASE("division by zero returns zero vector") {
        Vector3D v{1.0, 2.0, 3.0};
        auto r = v / 0;
        CHECK(approx(r.x_, 0.0));
        CHECK(approx(r.y_, 0.0));
        CHECK(approx(r.z_, 0.0));
    }

    TEST_CASE("compound division by scalar") {
        Vector3D v{6.0, 9.0, 12.0};
        v /= 3.0;
        CHECK(approx(v.x_, 2.0));
        CHECK(approx(v.y_, 3.0));
        CHECK(approx(v.z_, 4.0));
    }

    TEST_CASE("compound division by zero sets zero vector") {
        Vector3D v{1.0, 2.0, 3.0};
        v /= 0;
        CHECK(approx(v.x_, 0.0));
        CHECK(approx(v.y_, 0.0));
        CHECK(approx(v.z_, 0.0));
    }

    TEST_CASE("size") {
        Vector3D v{1.0, 2.0, 2.0};
        CHECK(approx(v.size(), 3.0));
    }

    TEST_CASE("normalize") {
        Vector3D v{1.0, 2.0, 2.0};
        v.normalize();
        CHECK(approx(v.size(), 1.0));
    }

    TEST_CASE("dot product") {
        Vector3D a{1.0, 2.0, 3.0}, b{4.0, 5.0, 6.0};
        CHECK(approx(get_dot_product(a, b), 32.0));
    }

    TEST_CASE("cross product") {
        Vector3D a{1.0, 0.0, 0.0}, b{0.0, 1.0, 0.0};
        auto c = get_cross_product(a, b);
        CHECK(approx(c.x_, 0.0));
        CHECK(approx(c.y_, 0.0));
        CHECK(approx(c.z_, 1.0));
    }

    TEST_CASE("cross product anti-commutativity") {
        Vector3D a{1.0, 2.0, 3.0}, b{4.0, 5.0, 6.0};
        auto ab = get_cross_product(a, b);
        auto ba = get_cross_product(b, a);
        CHECK(approx(ab.x_, -ba.x_));
        CHECK(approx(ab.y_, -ba.y_));
        CHECK(approx(ab.z_, -ba.z_));
    }

    TEST_CASE("get_normalized") {
        Vector3D v{1.0, 2.0, 2.0};
        auto n = get_normalized(v);
        CHECK(approx(n.size(), 1.0));
    }
}

// =============================================================================
// Vector4D
// =============================================================================
TEST_SUITE("Vector4D") {
    TEST_CASE("default construction") {
        Vector4D v;
        CHECK(approx(v.x_, 0.0));
        CHECK(approx(v.y_, 0.0));
        CHECK(approx(v.z_, 0.0));
        CHECK(approx(v.w_, 0.0));
    }

    TEST_CASE("scalar multiplication") {
        Vector4D v{1.0, 2.0, 3.0, 4.0};
        auto r = v * 2.0;
        CHECK(approx(r.x_, 2.0));
        CHECK(approx(r.y_, 4.0));
        CHECK(approx(r.z_, 6.0));
        CHECK(approx(r.w_, 8.0));
    }

    TEST_CASE("scalar multiplication compound") {
        Vector4D v{1.0, 2.0, 3.0, 4.0};
        v *= 2.0;
        CHECK(approx(v.x_, 2.0));
        CHECK(approx(v.y_, 4.0));
        CHECK(approx(v.z_, 6.0));
        CHECK(approx(v.w_, 8.0));
    }

    TEST_CASE("addition and subtraction") {
        Vector4D a{1.0, 2.0, 3.0, 4.0}, b{5.0, 6.0, 7.0, 8.0};
        auto sum = a + b;
        CHECK(approx(sum.x_, 6.0));
        CHECK(approx(sum.y_, 8.0));
        CHECK(approx(sum.z_, 10.0));
        CHECK(approx(sum.w_, 12.0));

        auto diff = b - a;
        CHECK(approx(diff.x_, 4.0));
        CHECK(approx(diff.y_, 4.0));
        CHECK(approx(diff.z_, 4.0));
        CHECK(approx(diff.w_, 4.0));
    }

    TEST_CASE("compound addition and subtraction") {
        Vector4D a{1.0, 2.0, 3.0, 4.0}, b{5.0, 6.0, 7.0, 8.0};
        a += b;
        CHECK(approx(a.x_, 6.0));
        CHECK(approx(a.y_, 8.0));
        CHECK(approx(a.z_, 10.0));
        CHECK(approx(a.w_, 12.0));

        a -= b;
        CHECK(approx(a.x_, 1.0));
        CHECK(approx(a.y_, 2.0));
        CHECK(approx(a.z_, 3.0));
        CHECK(approx(a.w_, 4.0));
    }

    TEST_CASE("division by scalar") {
        Vector4D v{2.0, 4.0, 6.0, 8.0};
        auto r = v / 2.0;
        CHECK(approx(r.x_, 1.0));
        CHECK(approx(r.y_, 2.0));
        CHECK(approx(r.z_, 3.0));
        CHECK(approx(r.w_, 4.0));
    }

    TEST_CASE("division by zero returns zero vector") {
        Vector4D v{1.0, 2.0, 3.0, 4.0};
        auto r = v / 0;
        CHECK(approx(r.x_, 0.0));
        CHECK(approx(r.y_, 0.0));
        CHECK(approx(r.z_, 0.0));
        CHECK(approx(r.w_, 0.0));
    }

    TEST_CASE("compound division by scalar") {
        Vector4D v{2.0, 4.0, 6.0, 8.0};
        v /= 2.0;
        CHECK(approx(v.x_, 1.0));
        CHECK(approx(v.y_, 2.0));
        CHECK(approx(v.z_, 3.0));
        CHECK(approx(v.w_, 4.0));
    }

    TEST_CASE("compound division by zero sets zero vector") {
        Vector4D v{1.0, 2.0, 3.0, 4.0};
        v /= 0;
        CHECK(approx(v.x_, 0.0));
        CHECK(approx(v.y_, 0.0));
        CHECK(approx(v.z_, 0.0));
        CHECK(approx(v.w_, 0.0));
    }

    TEST_CASE("size") {
        Vector4D v{1.0, 2.0, 2.0, 4.0};
        CHECK(approx(v.size(), 5.0));
    }

    TEST_CASE("normalize") {
        Vector4D v{1.0, 2.0, 2.0, 4.0};
        v.normalize();
        CHECK(approx(v.size(), 1.0));
    }

    TEST_CASE("get_normalized") {
        Vector4D v{1.0, 2.0, 2.0, 4.0};
        auto n = get_normalized(v);
        CHECK(approx(n.size(), 1.0));
    }
}

// =============================================================================
// Point2D / Point3D
// =============================================================================
TEST_SUITE("Point2D and Point3D") {
    TEST_CASE("Point2D default construction") {
        Point2D p;
        CHECK(approx(p.x_, 0.0));
        CHECK(approx(p.y_, 0.0));
    }

    TEST_CASE("Point3D default construction") {
        Point3D p;
        CHECK(approx(p.x_, 0.0));
        CHECK(approx(p.y_, 0.0));
        CHECK(approx(p.z_, 0.0));
    }
}

// =============================================================================
// Rect2D
// =============================================================================
TEST_SUITE("Rect2D") {
    TEST_CASE("default construction") {
        Rect2D r;
        CHECK(approx(r.x_, 0.0));
        CHECK(approx(r.y_, 0.0));
        CHECK(approx(r.w_, 0.0));
        CHECK(approx(r.h_, 0.0));
    }

    TEST_CASE("accessors") {
        Rect2D r{10.0, 20.0, 30.0, 40.0};
        CHECK(approx(r.get_left(), 10.0));
        CHECK(approx(r.get_top(), 20.0));
        CHECK(approx(r.get_right(), 40.0));
        CHECK(approx(r.get_bottom(), 60.0));
        CHECK(approx(r.get_area(), 1200.0));
    }

    TEST_CASE("contains_point") {
        Rect2D r{0.0, 0.0, 10.0, 10.0};
        CHECK(r.contains_point({5.0, 5.0}));
        CHECK(r.contains_point({0.0, 0.0}));
        CHECK(r.contains_point({10.0, 10.0}));
        CHECK(!r.contains_point({11.0, 5.0}));
        CHECK(!r.contains_point({5.0, 11.0}));
    }

    TEST_CASE("intersect") {
        Rect2D a{0.0, 0.0, 10.0, 10.0};
        Rect2D b{5.0, 5.0, 10.0, 10.0};
        Rect2D c{20.0, 20.0, 5.0, 5.0};
        CHECK(intersect(a, b));
        CHECK(!intersect(a, c));
    }

    TEST_CASE("intersect touching edges") {
        Rect2D a{0.0, 0.0, 10.0, 10.0};
        Rect2D b{10.0, 0.0, 10.0, 10.0};
        CHECK(!intersect(a, b));
    }

    TEST_CASE("merge") {
        Rect2D a{0.0, 0.0, 10.0, 10.0};
        Rect2D b{5.0, 5.0, 10.0, 10.0};
        auto m = merge(a, b);
        CHECK(approx(m.get_left(), 0.0));
        CHECK(approx(m.get_top(), 0.0));
        CHECK(approx(m.get_right(), 15.0));
        CHECK(approx(m.get_bottom(), 15.0));
    }

    TEST_CASE("merge disjoint rects") {
        Rect2D a{0.0, 0.0, 5.0, 5.0};
        Rect2D b{10.0, 10.0, 5.0, 5.0};
        auto m = merge(a, b);
        CHECK(approx(m.get_left(), 0.0));
        CHECK(approx(m.get_top(), 0.0));
        CHECK(approx(m.get_right(), 15.0));
        CHECK(approx(m.get_bottom(), 15.0));
    }

    TEST_CASE("zero area rect") {
        Rect2D r{5.0, 5.0, 0.0, 0.0};
        CHECK(approx(r.get_area(), 0.0));
        CHECK(r.contains_point({5.0, 5.0}));
    }
}

// =============================================================================
// Matrix3
// =============================================================================
TEST_SUITE("Matrix3") {
    TEST_CASE("identity multiplication") {
        Matrix3 identity{};
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                identity.set(i, j, (i == j) ? 1.0 : 0.0);

        Matrix3 m{};
        m.set(0, 0, 1.0);
        m.set(0, 1, 2.0);
        m.set(0, 2, 3.0);
        m.set(1, 0, 4.0);
        m.set(1, 1, 5.0);
        m.set(1, 2, 6.0);
        m.set(2, 0, 7.0);
        m.set(2, 1, 8.0);
        m.set(2, 2, 9.0);

        auto result = m * identity;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                CHECK(approx(result.get(i, j), m.get(i, j)));
    }

    TEST_CASE("matrix multiplication") {
        Matrix3 a{};
        a.set(0, 0, 1.0);
        a.set(0, 1, 0.0);
        a.set(0, 2, 0.0);
        a.set(1, 0, 0.0);
        a.set(1, 1, 1.0);
        a.set(1, 2, 0.0);
        a.set(2, 0, 0.0);
        a.set(2, 1, 0.0);
        a.set(2, 2, 1.0);

        Matrix3 b{};
        b.set(0, 0, 2.0);
        b.set(0, 1, 0.0);
        b.set(0, 2, 0.0);
        b.set(1, 0, 0.0);
        b.set(1, 1, 2.0);
        b.set(1, 2, 0.0);
        b.set(2, 0, 0.0);
        b.set(2, 1, 0.0);
        b.set(2, 2, 2.0);

        auto result = a * b;
        for (int i = 0; i < 3; i++) CHECK(approx(result.get(i, i), 2.0));
    }

    TEST_CASE("get_row and get_col") {
        Matrix3 m{};
        m.set(0, 0, 1.0);
        m.set(0, 1, 2.0);
        m.set(0, 2, 3.0);
        m.set(1, 0, 4.0);
        m.set(1, 1, 5.0);
        m.set(1, 2, 6.0);
        m.set(2, 0, 7.0);
        m.set(2, 1, 8.0);
        m.set(2, 2, 9.0);

        auto row0 = m.get_row(0);
        CHECK(approx(row0[0], 1.0));
        CHECK(approx(row0[1], 2.0));
        CHECK(approx(row0[2], 3.0));

        auto col0 = m.get_col(0);
        CHECK(approx(col0[0], 1.0));
        CHECK(approx(col0[1], 4.0));
        CHECK(approx(col0[2], 7.0));
    }

    TEST_CASE("matrix-vector multiplication") {
        Matrix3 m{};
        m.set(0, 0, 1.0);
        m.set(0, 1, 0.0);
        m.set(0, 2, 0.0);
        m.set(1, 0, 0.0);
        m.set(1, 1, 1.0);
        m.set(1, 2, 0.0);
        m.set(2, 0, 0.0);
        m.set(2, 1, 0.0);
        m.set(2, 2, 1.0);

        Vector3D v{3.0, 4.0, 5.0};
        auto r = m * v;
        CHECK(approx(r.x_, 3.0));
        CHECK(approx(r.y_, 4.0));
        CHECK(approx(r.z_, 5.0));
    }

    TEST_CASE("compound multiplication") {
        Matrix3 a{};
        a.set(0, 0, 2.0);
        a.set(0, 1, 0.0);
        a.set(0, 2, 0.0);
        a.set(1, 0, 0.0);
        a.set(1, 1, 2.0);
        a.set(1, 2, 0.0);
        a.set(2, 0, 0.0);
        a.set(2, 1, 0.0);
        a.set(2, 2, 2.0);

        Matrix3 b = a;
        a *= b;
        for (int i = 0; i < 3; i++) CHECK(approx(a.get(i, i), 4.0));
    }
}

// =============================================================================
// Matrix4
// =============================================================================
TEST_SUITE("Matrix4") {
    TEST_CASE("identity multiplication") {
        Matrix4 identity{};
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                identity.set(i, j, (i == j) ? 1.0 : 0.0);

        Matrix4 m{};
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m.set(i, j, static_cast<double>(i * 4 + j + 1));

        auto result = m * identity;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                CHECK(approx(result.get(i, j), m.get(i, j)));
    }

    TEST_CASE("get_row and get_col") {
        Matrix4 m{};
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m.set(i, j, static_cast<double>(i * 4 + j + 1));

        auto row0 = m.get_row(0);
        CHECK(approx(row0[0], 1.0));
        CHECK(approx(row0[1], 2.0));
        CHECK(approx(row0[2], 3.0));
        CHECK(approx(row0[3], 4.0));

        auto col0 = m.get_col(0);
        CHECK(approx(col0[0], 1.0));
        CHECK(approx(col0[1], 5.0));
        CHECK(approx(col0[2], 9.0));
        CHECK(approx(col0[3], 13.0));
    }

    TEST_CASE("matrix-vector multiplication") {
        Matrix4 identity{};
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                identity.set(i, j, (i == j) ? 1.0 : 0.0);

        Vector4D v{1.0, 2.0, 3.0, 4.0};
        auto r = identity * v;
        CHECK(approx(r.x_, 1.0));
        CHECK(approx(r.y_, 2.0));
        CHECK(approx(r.z_, 3.0));
        CHECK(approx(r.w_, 4.0));
    }

    TEST_CASE("scaling matrix") {
        Matrix4 s{};
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) s.set(i, j, (i == j) ? 2.0 : 0.0);

        Vector4D v{1.0, 2.0, 3.0, 4.0};
        auto r = s * v;
        CHECK(approx(r.x_, 2.0));
        CHECK(approx(r.y_, 4.0));
        CHECK(approx(r.z_, 6.0));
        CHECK(approx(r.w_, 8.0));
    }

    TEST_CASE("compound multiplication") {
        Matrix4 a{};
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) a.set(i, j, (i == j) ? 2.0 : 0.0);

        Matrix4 b = a;
        a *= b;
        for (int i = 0; i < 4; i++) CHECK(approx(a.get(i, i), 4.0));
    }
}