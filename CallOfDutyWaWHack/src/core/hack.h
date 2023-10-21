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
		DEFINE_MEMBER_N(int, m_Alive, 0x7C);
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

	Vec3 getHeadPos() {
		return *(Vec3*)(*(uintptr_t*)this + 0x154);
	}
};

inline uintptr_t dwEntityList = 0x018E7448;
inline uintptr_t dwLocalPlayer = 0x018ED068;
inline uintptr_t dwViewMatrix = 0x008e870c;

inline float viewMatrix[16];

class Hack {
public:
	ID3DXLine* LineL;
	ID3DXFont* FontF;

	Vec2 crosshair2D;
	int crosshairSize = 4;

	~Hack();

	void Init();
	void Update();

	void CheckButtons();

	struct Settings {
		bool showMenu		= true;
		bool snaplines		= false;
		bool box2D			= false;
		bool status			= false;
		bool statusText		= false;
		bool rcsCrosshair	= false;
		bool aimbot			= false;
		bool points			= false;
		bool health			= false;
		bool ammo			= false;
		bool grenades		= false;
	} settings;

	struct Buttons {
		DWORD showMenuBtn = VK_INSERT;
		DWORD snaplinesBtn = VK_NUMPAD1;
		DWORD box2DBtn = VK_NUMPAD2;
		DWORD status2DBtn = VK_NUMPAD3;
		DWORD statusTextBtn = VK_NUMPAD4;
		DWORD rcsCrosshairBtn = VK_NUMPAD5;
		DWORD pointsBtn = VK_NUMPAD6;
		DWORD healthBtn = VK_NUMPAD7;
		DWORD ammoBtn = VK_NUMPAD8;
		DWORD grenadesBtn = VK_NUMPAD9;
	} button;

	struct Colors {
		D3DCOLOR health = D3DCOLOR_ARGB(255, 46, 139, 87);
		D3DCOLOR blue = D3DCOLOR_ARGB(255, 30, 144, 255);
		D3DCOLOR white = D3DCOLOR_ARGB(255, 255, 255, 255);
		struct Zombie {
			D3DCOLOR esp = D3DCOLOR_ARGB(255, 255, 0, 0);
			D3DCOLOR snapline = D3DCOLOR_ARGB(255, 255, 0, 0);
		} zombie;
	} color;
};