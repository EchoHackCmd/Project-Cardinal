#include "GameMode.h"
#include "../../../Client/Client.h"

Client* clientI;

typedef void(__thiscall* GM_Tick)(GameMode* GM);
GM_Tick _GM_Tick;

typedef void(__thiscall* attack)(GameMode*, Actor*);
attack _attack;

void attackCallback(GameMode* _this, Actor* a) {
	if (clientI != nullptr) {
		for (auto C : clientI->categories) {
			for (auto M : C->modules) {
				if (M->isEnabled) M->onGmAttack(_this, a);
			}
		}
	}
	_attack(_this, a);
}

typedef void(__thiscall* startDestroy)(GameMode*, Vec3i*, UCHAR, bool*);
startDestroy _startDestroy;

void destroyCallback(GameMode* _this, Vec3i* blockPos, UCHAR blockFace, bool* idk) {
	if (clientI != nullptr) {
		for (auto C : clientI->categories) {
			for (auto M : C->modules) {
				if (M->isEnabled) M->onGmStartDestroy(_this, blockPos, blockFace);
			}
		}
	}
	_startDestroy(_this, blockPos, blockFace, idk);
}

void GMTick_Callback(GameMode* GM) {
	if (clientI != nullptr) {
		for (auto C : clientI->categories) {
			for (auto M : C->modules) {
				M->player = GM->player;
				if (M->isEnabled) M->onGmTick(GM);
			}
		}
	}
	_GM_Tick(GM);
}

void GameMode_Hook::init() {
	clientI = client;

	Utils::DebugLogF("Preparing GameMode Hook");

	uintptr_t sigAddr = Utils::FindSig("48 8D 05 ? ? ? ? 48 8B D9 48 89 01 8B FA 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 8B 01 BA ? ? ? ? FF 10 48 8B 8B");
	if (!sigAddr) return Utils::DebugLogF("Unable to find address!");
	int offset = *reinterpret_cast<int*>(sigAddr + 3);
	uintptr_t** VTable = reinterpret_cast<uintptr_t**>(sigAddr + offset + 7);

	if (MH_CreateHook((void*)VTable[10], &GMTick_Callback, reinterpret_cast<LPVOID*>(&_GM_Tick)) == MH_OK) {
		MH_EnableHook((void*)VTable[10]);
		Utils::DebugLogF("Successfully completed GameMode Hook!");
	}
	else {
		Utils::DebugLogF("Failed to create GameMode Hook :/");
	}

	if (MH_CreateHook((void*)VTable[14], &attackCallback, reinterpret_cast<LPVOID*>(&_attack)) == MH_OK) {
		MH_EnableHook((void*)VTable[14]);
		Utils::DebugLogF("Successfully completed Attack Hook!");
	}
	else {
		Utils::DebugLogF("Failed to create Attack Hook :/");
	}

	if (MH_CreateHook((void*)VTable[1], &destroyCallback, reinterpret_cast<LPVOID*>(&_startDestroy)) == MH_OK) {
		MH_EnableHook((void*)VTable[1]);
		Utils::DebugLogF("Successfully completed StartDestroyBlock Hook!");
	}
	else {
		Utils::DebugLogF("Failed to create StartDestroyBlock Hook :/");
	}
}