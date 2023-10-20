#pragma once
#include <cmath>

#define ABS(x) ((x < 0) ? (-x) : (x))

#define PI 3.1415927f

struct Vec2 {
	float x, y;
};

struct Vector3 {
	float x, y, z;

	Vector3() {};
	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float Length() const { return sqrt(x * x + y * y + z * z); }
	Vector3 Normalize() const { return *this * (1 / Length()); }
	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
};

struct Vec4 {
	float x, y, z, w;
};

using Vec3 = Vector3;
using vec3 = Vector3;
using Vec = Vector3;
using vec = Vector3;

inline float DotProduct(vec3 src, vec3 dst) {
	return src.x * dst.x + src.y * dst.y + src.z * dst.z;
}

//Thanks to c5 for this knowledge
inline bool WorldToScreen(vec3 src, vec3 dst, vec3& screen, float fovx, float fovy, float windowWidth, float windowHeight, vec3 left, vec3 up, vec3 forward) {
    vec3 transform;
    float xc, yc;
    float px, py;
    float z;

    px = tan(fovx * PI / 360.0);
    py = tan(fovy * PI / 360.0);

    transform = dst - src; // swap positions?

    xc = windowWidth / 2.0;
    yc = windowHeight / 2.0;

    z = DotProduct(transform, left);

    if (z <= 0.1)
    {
        return false;
    }

    screen.x = xc - DotProduct(transform, up) * xc / (z * px);
    screen.y = yc - DotProduct(transform, forward) * yc / (z * py);

    return true;
}
//, int windowWidth, int windowHeight
inline bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight) {
    Vec4 clipCoords;
    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
    clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
    clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
    clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

    if (clipCoords.w < 0.1f)
        return false;
    
    Vec3 NDC;
    NDC.x = clipCoords.x / clipCoords.w; 
    NDC.y = clipCoords.y / clipCoords.w; 
    NDC.z = clipCoords.z / clipCoords.w;
    
    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
    
    return true;
}