#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;


const double EPSILON = 1e-10;

class Vector2
{
public:
    double x, y;
    Vector2(): x(0), y(0) {}
    Vector2(double x, double y): x(x), y(y) { }
    Vector2(const Vector2 &other): x(other.x), y(other.y) { }
    double norm()
    {
        return sqrt(x * x + y * y);
    }
    Vector2 normalized()
    {
        Vector2 result = *this / norm();
        return result;
    }
    double dot(Vector2 rhs)
    {
        return x * rhs.x + y * rhs.y;
    }
    double cross(Vector2 rhs)
    {
        return x * rhs.y - y * rhs.x;
    }
    Vector2 operator+(Vector2 rhs)
    {
        Vector2 result(x + rhs.x, y + rhs.y);
        return result;
    }
    Vector2 operator+=(Vector2 rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    Vector2 operator-(Vector2 rhs)
    {
        Vector2 result(x - rhs.x, y - rhs.y);
        return result;
    }
    Vector2 operator-=(Vector2 rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    Vector2 operator-()
    {
        Vector2 result(-x, -y);
        return result;
    }
    Vector2 operator*(double scalar)
    {
        Vector2 result(x * scalar, y * scalar);
        return result;
    }
    Vector2 operator/(double scalar)
    {
        Vector2 result(x / scalar, y / scalar);
        return result;
    }
    bool operator==(Vector2 rhs)
    {
        return x == rhs.x && y == rhs.y;
    }
    bool operator<(Vector2 rhs)
    {
        if (x == rhs.x)
            return y < rhs.y;
        return x < rhs.x;
    }
    bool operator<=(Vector2 rhs)
    {
        if (x == rhs.x)
            return y <= rhs.y;
        return x <= rhs.x;
    }
    bool operator>(Vector2 rhs)
    {
        return rhs < *this;
    }
    bool operator>=(Vector2 rhs)
    {
        return rhs <= *this;
    }
};

bool is_intersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d)
{
    double ret1 = (b - a).cross(c - a) * (b - a).cross(d - a);
    double ret2 = (d - c).cross(a - c) * (d - c).cross(b - c);

    if (ret1 == 0 && ret2 == 0) {
        if (a > b)
            swap(a, b);
        if (c > d)
            swap(c, d);
        return a <= d && c <= b;
    }

    return ret1 <= 0 && ret2 <= 0;
}

pair<bool, Vector2> get_intersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d)
{
    if (a > b)
        swap(a, b);
    if (c > d)
        swap(c, d);
    if (a > c) {
        swap(a, c);
        swap(b, d);
    }

    if (!is_intersect(a, b, c, d))
        return {false, Vector2(NAN, NAN)};
    
    Vector2 dir1 = (b - a).normalized();
    Vector2 dir2 = (d - c).normalized();
    double den = dir1.cross(dir2);
    if (-EPSILON <= den && den <= EPSILON) {
        if (b == c)
            return {true, b};
        return {true, Vector2(NAN, NAN)};
    }
    else {
        double l = (c - a).cross(dir1) / den;
        Vector2 intersection = c + dir2 * l;
        return {true, intersection};
    }
}

vector<Vector2> get_convex_hull(vector<Vector2> points)
{
    sort(points.begin(), points.end());
    Vector2 start_point = points[0];
    for (Vector2 &point:points)
        point -= start_point;

    stable_sort(points.begin(), points.end(), [](Vector2 A, Vector2 B) {
        return A.cross(B) > 0;
    });

    vector<Vector2> lasts;
    for (Vector2 &point:points)
    {
        while (lasts.size() > 1)
        {
            Vector2 current = lasts.back();
            Vector2 last = lasts[lasts.size() - 2];
            if ((current - last).cross(point - last) > 0.0)
                break;
            lasts.pop_back();
        }
        lasts.push_back(point);
    }
    for (Vector2 &point:lasts)
        point += start_point;

    return lasts;
}

pair<double, double> get_projection(vector<Vector2> &points, Vector2 &axis)
{
    double min_val = axis.dot(points[0]);
    double max_val = min_val;
    for (int i = 1; i < points.size(); i++)
    {
        double projected = axis.dot(points[i]);
        min_val = min(min_val, projected);
        max_val = max(max_val, projected);
    }
    return {min_val, max_val};
}

vector<Vector2> get_normals(vector<Vector2> &points)
{
    vector<Vector2> ret;
    if (points.size() == 1)
        return ret;

    for (int i = 0; i < points.size(); i++)
    {
        Vector2 &a = points[i];
        Vector2 &b = points[(i + 1) % points.size()];
        ret.push_back(Vector2((b - a).y, -(b - a).x));
    }
    return ret;
}

bool can_separate(vector<Vector2> &A, vector<Vector2> &B)
{
    if (A.size() == 1 && B.size() == 1)
        return true;

    auto c_a = get_convex_hull(A);
    auto c_b = get_convex_hull(B);
    auto n_a = get_normals(c_a);
    auto n_b = get_normals(c_b);
    n_a.insert(n_a.end(), n_b.begin(), n_b.end());
    if (c_a.size() > 1)
        n_a.push_back(Vector2(c_a[1] - c_a[0]));
    if (c_b.size() > 1)
        n_a.push_back(Vector2(c_b[1] - c_b[0]));
        
    for (Vector2 &axis:n_a)
    {
        auto p_a = get_projection(c_a, axis);
        auto p_b = get_projection(c_b, axis);
        if (!((p_a.second >= p_b.first) && (p_b.second >= p_a.first)))
            return true;
    }
    return false;
}

pair<Vector2, Vector2> get_max_points(vector<Vector2> &points)
{
    int left = 0, right = max_element(points.begin(), points.end()) - points.begin();
    int ret1 = left, ret2 = right;
    double max_len = (points[right] - points[left]).norm();
    int end = right;

    Vector2 left_dir = Vector2(0, -1.0);

    vector<Vector2> edges;
    for (int i = 0; i < points.size(); i++)
        edges.push_back((points[(i + 1) % points.size()] - points[i]).normalized());
    
    while (right != 0 || left != end)
    {
        double next1 = left_dir.dot(edges[left]);
        double next2 = -left_dir.dot(edges[right]);
        if (left != end && (right == 0 || next1 > next2)) {
            left_dir = edges[left];
            left = (left + 1) % points.size();
        }
        else {
            left_dir = -edges[right];
            right = (right + 1) % points.size();
        }
        
        double len = (points[right] - points[left]).norm();
        if (len > max_len) {
            ret1 = left;
            ret2 = right;
            max_len = len;
        }
    }
    return { points[ret1], points[ret2] };
}