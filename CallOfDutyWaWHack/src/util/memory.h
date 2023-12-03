#pragma once
#include <cstdint>
#include <windows.h>
#include <vector>

namespace memory {
	// given the interface pointer, calls the function at index provided
	template <typename Return, typename ... Args>
	constexpr Return Call(void* _this, const std::uintptr_t index, Args ... args) noexcept {

		typedef Return(__thiscall* Function)(void*, decltype(args) ...);
		return (*static_cast<Function**>(_this))[index](_this, args...);
	}

	// given the interface pointer, returns the function at index provided
	constexpr void* Get(void* _this, const std::uintptr_t index) noexcept {

		return (*static_cast<void***>(_this))[index];
	}

	void Patch(BYTE* dst, int src, unsigned int size) {

		DWORD oldProtect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);

		memcpy(dst, &src, size);
		VirtualProtect(dst, size, oldProtect, &oldProtect);
	}

	void Patch(BYTE* dst, BYTE* src, unsigned int size) {

		DWORD oldProtect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);

		memcpy(dst, src, size);
		VirtualProtect(dst, size, oldProtect, &oldProtect);
	}

	void Nop(BYTE* dst, unsigned int size) {

		DWORD oldProtect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);

		memset(dst, 0x90, size);
		VirtualProtect(dst, size, oldProtect, &oldProtect);
	}

	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int>& offsets) {

		uintptr_t addr = ptr;
		for (unsigned int i = 0; i < offsets.size(); i++) {
			addr = *(uintptr_t*)addr;
			addr += offsets[i];
		}
		return addr;
	}
}