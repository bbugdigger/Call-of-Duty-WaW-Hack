#include "includes.h"

void Hack::Init() {
}

void Hack::Update() {
	memcpy(&viewMatrix, (PBYTE*)(dwViewMatrix), sizeof(viewMatrix));
}

//Thanks to c5 for this knowledge
bool WorldToScreen(vec3 src, vec3 dst, vec3& screen, float fovx, float fovy, float windowWidth, float windowHeight, vec3 left, vec3 up, vec3 forward) {
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

