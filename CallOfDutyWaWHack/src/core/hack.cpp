#include "includes.h"

Hack::~Hack() {
	this->FontF->Release();
	this->LineL->Release();
}

void Hack::Init() {
}

void Hack::Update() {
	memcpy(&viewMatrix, (PBYTE*)(dwViewMatrix), sizeof(viewMatrix));
}
