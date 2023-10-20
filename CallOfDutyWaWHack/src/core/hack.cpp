#include "includes.h"

void Hack::Init() {
}

void Hack::Update() {
	memcpy(&viewMatrix, (PBYTE*)(dwViewMatrix), sizeof(viewMatrix));
}
