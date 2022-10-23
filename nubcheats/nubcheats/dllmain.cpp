#include "pch.h"
#include "offsets.h"
#include <thread>
#include <Windows.h>
#include <stdint.h>
#include <iostream>


//global variables
const auto client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));			 
const auto entitylist = *reinterpret_cast<uintptr_t*>(client + offsets::dwEntityList);	 
const auto LocalPlayer = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayer);




void Triggerbot()
{
	
	
		const auto client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));
		const auto entity = *reinterpret_cast<uintptr_t*>(client + offsets::dwEntityList);
		const auto LocalPlayer = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayer);
		const auto health = *reinterpret_cast<int*>(LocalPlayer + offsets::m_iHealth);

		const auto myteam = *reinterpret_cast<int*>(LocalPlayer + offsets::m_iTeamNum);
		const auto crosshairid = *reinterpret_cast<int*>(LocalPlayer + offsets::m_iCrosshairId);
		const auto everyteam = *reinterpret_cast<int*>(client + offsets::dwEntityList + (crosshairid - 1) * 0x10);
		const auto enemyteam = *reinterpret_cast<int*>(everyteam + offsets::m_iTeamNum);

		if (crosshairid > 0 && crosshairid < 64 && myteam != enemyteam && health > 0)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
			Sleep(100);

		}

	
}

void Bhop()
{

	const auto client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));
	const auto LocalPlayer = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayer);
	const auto health = *reinterpret_cast<int*>(LocalPlayer + offsets::m_iHealth);
	const auto flags = *reinterpret_cast<uintptr_t*>(LocalPlayer + offsets::m_fFlags);
	
	if (flags & (1 << 0) && health > 0 && LocalPlayer !=NULL)
	{
		*reinterpret_cast<BYTE*>(client + offsets::dwForceJump) = 6;
		Sleep(1);
		*reinterpret_cast<BYTE*>(client + offsets::dwForceJump) = 4;
	}
}

void Glow()
{
	const auto client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));
	const auto LocalPlayer = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayer);
	int myTeam = *reinterpret_cast<uintptr_t*>(LocalPlayer + offsets::m_iTeamNum);
	
	
	DWORD glowObject = *reinterpret_cast<DWORD*>(client + offsets::dwGlowObjectManager);

	for (int i = 0; i < 64; i++)
	{
		uintptr_t entity = *(uintptr_t*)(client + offsets::dwEntityList + i * 0x10);
		int enemyTeam = *reinterpret_cast<uintptr_t*>(client + offsets::m_iTeamNum);
		int health = *reinterpret_cast<uintptr_t*>(entity + offsets::m_iHealth);

		if (entity && LocalPlayer != NULL)
		{
			int entityTeam = *reinterpret_cast<uintptr_t*>(entity + offsets::m_iTeamNum);
			int glowIndex = *reinterpret_cast<uintptr_t*>(entity + offsets::m_iGlowIndex);
			if (enemyTeam != entity)
			{
				*reinterpret_cast<float*>(glowObject + (glowIndex * 0x38) + 0x8) = 1; //red
				*reinterpret_cast<float*>(glowObject + (glowIndex * 0x38) + 0xC) = 1;//green
				*reinterpret_cast<float*>(glowObject + (glowIndex * 0x38) + 0x10) = 1;//blue
				*reinterpret_cast<float*>(glowObject + (glowIndex * 0x38) + 0x14) = 1.7;//alpha
			}
			else
			{
				*reinterpret_cast<float*>(glowObject + (glowIndex * 0x38) + 0x8) = 0; //red
				*reinterpret_cast<float*>(glowObject + (glowIndex * 0x38) + 0xC) = 0;//green
				*reinterpret_cast<float*>(glowObject + (glowIndex * 0x38) + 0x10) = 2;//blue
				*reinterpret_cast<float*>(glowObject + (glowIndex * 0x38) + 0x14) = 1.7;//alpha
			}
			 
			*reinterpret_cast<bool*>(glowObject + (glowIndex * 0x38) + 0x28) = true;
			*reinterpret_cast<bool*>(glowObject + (glowIndex * 0x38) + 0x29) = false;
			
		}



	}
}




void mainthread(HMODULE hinstDLL)
{
	
	
	while (!GetAsyncKeyState(VK_END))
	{
		
		
		while (entitylist > 1)
		{
			

			//bhop
			if (GetAsyncKeyState(VK_SPACE))
			{
				Bhop();

			}

			//trigger bot
			if (GetAsyncKeyState(VK_XBUTTON2))
			{
				Triggerbot();
			}
			//glow

			
			
				Glow();
			
			
			//aimbot
			

		}
			
	}
	
	FreeLibraryAndExitThread(hinstDLL, 0);
}

int _stdcall DllMain(HMODULE hinstDLL, int fdwReason,  void* lpvReserved)
{
	if (fdwReason == 1)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(nullptr, NULL, LPTHREAD_START_ROUTINE(mainthread), hinstDLL, NULL, NULL);
	}
	if (mainthread)
		CloseHandle(mainthread);
	return TRUE;
	

	
}
