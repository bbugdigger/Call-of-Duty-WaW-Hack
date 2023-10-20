#pragma once
#include "includes.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

class Zombie {
public:
	union {
		// origin
		DEFINE_MEMBER_N(Vec3, m_Origin, 0x14);
		// alive
		DEFINE_MEMBER_N(int, m_Alive, 0x68);
	};
public:
	Vec3 GetOrigin() {
		return m_Origin;
	}

	int isAlive() {
		return m_Alive;
	}

	int getHealth() {
		return *(int*)(*(uintptr_t*)this + 0x1C8);
	}
};

class Hack {
public:
	uintptr_t dwEntityList	= 0x018E7448;
	uintptr_t dwLocalPlayer = 0x018ED068;
	uintptr_t dwViewMatrix	= 0x008e870c;

	float viewMatrix[16];

	void Init();
	void Update();
	bool WorldToScreen(vec3 src, vec3 dst, vec3& screen, float fovx, float fovy, float windowWidth, float windowHeight, vec3 left, vec3 up, vec3 forward);

	struct Settings {
		bool showMenu		= true;
		bool snaplines		= false;
		bool box2D			= false;
		bool status			= false;
		bool statusText		= false;
		bool rcsCrosshair	= false;
		bool aimbot			= false;
		bool points			= false;
	} settings;
};