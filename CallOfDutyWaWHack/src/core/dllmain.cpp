#include "includes.h"
#include <iostream>
#include <sstream>
#include <string.h>

// data
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

Hack* hack;
Vec2 zombieOriginScreen;
Vec2 zombieHeadScreen;

// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;


	int menuOffX = windowWidth / 2;
	int menuOffY = 50;
	D3DCOLOR enabled = D3DCOLOR_ARGB(255, 0, 255, 0);
	D3DCOLOR disabled = D3DCOLOR_ARGB(255, 255, 0, 0);

	if (!hack->settings.showMenu) {
		DrawText("Show Menu	(INS)", menuOffX, menuOffY, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else {
		DrawText("Snaplines				(NUMPAD1)", menuOffX, menuOffY + 1 * 12, hack->settings.snaplines ? enabled : disabled);
		DrawText("2D Box				(NUMPAD2)", menuOffX, menuOffY + 2 * 12, hack->settings.box2D ? enabled : disabled);
		DrawText("2D Statusbars			(NUMPAD3)", menuOffX, menuOffY + 3 * 12, hack->settings.status ? enabled : disabled);
		DrawText("Status Text			(NUMPAD4)", menuOffX, menuOffY + 4 * 12, hack->settings.statusText ? enabled : disabled);
		DrawText("Recoil crosshair		(NUMPAD5)", menuOffX, menuOffY + 5 * 12, hack->settings.rcsCrosshair ? enabled : disabled);
		DrawText("Lock points to 50000	(NUMPAD6)", menuOffX, menuOffY + 6 * 12, hack->settings.points ? enabled : disabled);
		DrawText("Lock health to 10000	(NUMPAD7)", menuOffX, menuOffY + 7 * 12, hack->settings.health ? enabled : disabled);
		DrawText("Infinite ammo			(NUMPAD8)", menuOffX, menuOffY + 8 * 12, hack->settings.ammo ? enabled : disabled);
		DrawText("Infinite grenades		(NUMPAD9)", menuOffX, menuOffY + 9 * 12, hack->settings.grenades ? enabled : disabled);
		DrawText("Hide Menu				(INS)", menuOffX, menuOffY + 10 * 12, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// drawing stuff
	Vec3* cords = (Vec3*)(dwLocalPlayer + 0x20);
	int health = *(int*)(dwLocalPlayer + 0x164);

	for (int i = 0; i < 24; i++) {
		Zombie* currZombie = (Zombie*)(dwEntityList + i * 0x88);
		Vec3 zombieCords = currZombie->GetOrigin();

		// check for nullptr here because of the way Zombie entity is set up (padding chars!!!) 
		if (zombieCords.x == 0 && zombieCords.y == 0 && zombieCords.z == 0)
			break;

		//if (!currZombie->isAlive())
		if (currZombie->getHealth() <= 0)
			continue;

		
		if (WorldToScreen(zombieCords, zombieOriginScreen, viewMatrix, windowWidth, windowHeight)) {

			// snapline
			if (hack->settings.snaplines) {
				DrawLine(zombieOriginScreen.x, zombieOriginScreen.y, windowWidth / 2, windowHeight, 2, hack->color.zombie.snapline);
			}

			Vec3 zombieHead = currZombie->getHeadPos();
			if (WorldToScreen(zombieHead, zombieHeadScreen, viewMatrix, windowWidth, windowHeight)) {
				
				if (hack->settings.box2D) {
					DrawEspBox2D(zombieOriginScreen, zombieHeadScreen, 2, hack->color.zombie.esp);
				}

				if (hack->settings.status) {
					/*int height = ABS(zombieOriginScreen.y - zombieHeadScreen.y);
					int dX = (zombieOriginScreen.x - zombieHeadScreen.x);

					float healthPerc = currZombie->getHealth() / 100.f;

					Vec2 botHealth, topHealth;
					int healthHeight = height * healthPerc;

					botHealth.y = zombieOriginScreen.y;

					botHealth.x = zombieOriginScreen.x - (height / 4) - 2;

					topHealth.y = zombieHeadScreen.y + height - healthHeight;

					topHealth.x = zombieOriginScreen.x - (height / 4) - 2 - (dX * healthPerc);

					DrawLine(botHealth, topHealth, 2, D3DCOLOR_ARGB(255, 46, 139, 87));*/
				}

				if (hack->settings.statusText) {
					std::stringstream s1;
					s1 << currZombie->getHealth();
					std::string t1 = "health: " + s1.str();
					char* healthMsg = (char*)t1.c_str();

					DrawText(healthMsg, zombieOriginScreen.x, zombieOriginScreen.y, hack->color.white);
				}
			}
		}
	}
	
	// crosshair
	if (hack->settings.rcsCrosshair) {
		Vec2 l, r, t, b;
		l = r = t = b = hack->crosshair2D;
		l.x -= hack->crosshairSize;
		r.x += hack->crosshairSize;
		b.y += hack->crosshairSize;
		t.y -= hack->crosshairSize;

		DrawLine(l, r, 2, hack->color.white);
		DrawLine(t, b, 2, hack->color.white);
	}

	// call og function
	oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {
	hack = new Hack();

	// hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	// hack loop
	while (!GetAsyncKeyState(VK_END)) {
		hack->Update();
	}

	// unhook
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	// uninject
	FreeLibraryAndExitThread(hModule, 0);
}








DWORD WINAPI TestHackThread(HMODULE hModule) {
	//Create Debugging Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "[*] Debugging Console Started\n";

	hack = new Hack();

	Vec3* cords = (Vec3*)(dwLocalPlayer + 0x20);
	int health = *(int*)(dwLocalPlayer + 0x164);

	// hack loop
	while (!GetAsyncKeyState(VK_END)) {

		system("cls");

		std::cout << "LocalPlayer cords: "  << cords->x << ", " << cords->y << ", " << cords->z << std::endl;
		std::cout << "LocalPlayer health: " << health << std::endl;

		std::cout << "=================\n";

		for (int i = 0; i < 24; i++) {
			Zombie* currZombie = (Zombie*)(dwEntityList + i * 0x88);
			Vec3 zombieCords = currZombie->GetOrigin();

			// check for nullptr here because of the way Zombie entity is set up (padding chars!!!) 
			if (zombieCords.x == 0 && zombieCords.y == 0 && zombieCords.z == 0)
				break;
			
			int zombieHealth = currZombie->getHealth();
			//Vec3* zombieCords = (Vec3*)(hack->dwEntityList + i * 0x88 + 0x14);
			//int zombieHealth = *(int*)(*(uintptr_t*)(hack->dwEntityList + i * 0x88) + 0x1C8);

			std::cout << "Zombie" << i << " cords: " << zombieCords.x << ", " << zombieCords.y << ", " << zombieCords.z << std::endl;
			std::cout << "Zombie" << i << " health: " << zombieHealth << std::endl;
			std::cout << "=================\n";
		}

		Sleep(100);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	return TRUE;
}