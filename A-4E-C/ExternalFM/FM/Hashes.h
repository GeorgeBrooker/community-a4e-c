#pragma once
    #include <Windows.h>

    static WCHAR* files[] = {L"entry.lua",L"A-4E-C.lua",L"Cockpit/Scripts/EFM_Data_Bus.lua",L"Entry/Suspension.lua"};

    static BYTE hashes[][32] = {
	{0x98,0xf0,0xdd,0x2e,0x09,0x28,0xb9,0xe4,0x64,0x7b,0x31,0x1d,0x59,0xab,0xe2,0x5c,0x0d,0x45,0x2a,0xc7},
	{0x66,0x6a,0xd0,0x80,0xf0,0xe1,0xc9,0xd6,0x88,0x8d,0x4a,0x7a,0x3a,0x06,0x57,0x41,0x56,0x4a,0x03,0xeb},
	{0x35,0xb8,0xb5,0xfc,0x45,0x06,0x48,0xf2,0x26,0x0e,0x99,0xde,0x4d,0xb8,0x08,0x83,0xe0,0x35,0x2d,0x04},
	{0x7a,0x1c,0x9c,0xfc,0x33,0x1e,0x94,0xcb,0xc6,0x68,0x8c,0x06,0xe8,0x46,0xdd,0x2b,0x25,0x62,0xb0,0xab}
};
    