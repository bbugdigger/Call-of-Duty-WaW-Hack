#include "includes.h"

Hack::~Hack() {
	this->FontF->Release();
	this->LineL->Release();
}

void Hack::Init() {
}

void Hack::Update() {
	memcpy(&viewMatrix, (PBYTE*)(dwViewMatrix), sizeof(viewMatrix));
	this->CheckButtons();
}

void Hack::CheckButtons() {
	if (GetAsyncKeyState(button.showMenuBtn) & 1)
		settings.showMenu = !settings.showMenu;
	if (GetAsyncKeyState(button.snaplinesBtn) & 1)
		settings.snaplines = !settings.snaplines;
	if (GetAsyncKeyState(button.box2DBtn) & 1)
		settings.box2D = !settings.box2D;
	if (GetAsyncKeyState(button.status2DBtn) & 1)
		settings.status = !settings.status;
	if (GetAsyncKeyState(button.statusTextBtn) & 1)
		settings.statusText = !settings.statusText;
	if (GetAsyncKeyState(button.rcsCrosshairBtn) & 1)
		settings.rcsCrosshair = !settings.rcsCrosshair;

}