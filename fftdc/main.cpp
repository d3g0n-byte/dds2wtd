// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "libertyFourXYZ/rage_string.h"
#include "libertyFourXYZ/trace.h"
#include "libertyFourXYZ/memory_manager.h"
#include "libertyFourXYZ/settings.h"
#include "libertyFourXYZ/globals.h"

#include <filesystem>

#include "wtd.h"

#define LFXYZ_VERSION 0
#define LFXYZ_VERSION2 101

// FuisonFix texture dictionary compiler
// LibertyFourXYZ was created for rdr, but then was ported to iv, so everything you need for wtd v11 is in this project.

auto g_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

void printInfo() {
	printf("Usage: fftdc.exe [-c_wtd_v8 | -c_wtd_v11] [-d path/to/file.dds... | -f path/to/folder]\n");
}

void exitFunc() {
	auto timeOnEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	__int64 timeUsed = timeOnEnd - g_startTime;
	printf("Elapsed %d.%03d second\n", (DWORD)timeUsed / 1000, (DWORD)timeUsed % 1000);
}

void main(int argc, const char* argv[]) {
	atexit(exitFunc);
	printf("Compiled on %s %s. Based on libertyFourXYZ v%i.%i\n", __DATE__, __TIME__, LFXYZ_VERSION, LFXYZ_VERSION2);
	
	libertyFourXYZ::initializeGlobals();
	libertyFourXYZ::readSettings();
	libertyFourXYZ::g_bForceUseRsc5 = 1;
	libertyFourXYZ::g_bMergeRscPages= 1;
	libertyFourXYZ::g_bUsePageMap = 1;
	libertyFourXYZ::g_nbMaxPageSizeForMerge = 5;
	//std::cout << "Hello World!\n";

	if (argc < 2) {
		printInfo();
		return;
	}

	if (!strcmp(argv[1],"-c_wtd_v8") || !strcmp(argv[1], "-c_wtd_v11")) {
		rage::ConstString* ppszDdsPath = NULL;

		rage::ConstString pszOutFile = argv[2];

		DWORD dwFileCount = 0;
		if (!strcmp(argv[3], "-d")) {
			dwFileCount = argc - 4;
			if (dwFileCount < 1) { error("[main] no dds textures found"); return; }
			ppszDdsPath = new("main, dds path 1") rage::ConstString[dwFileCount];
			for (int i = 4; i < argc; i++) ppszDdsPath[i - 4] = argv[i];
		}
		else if (!strcmp(argv[3], "-f")) {
			dwFileCount = 0;
			for (const auto& entry : std::filesystem::directory_iterator(argv[4]))
				if (entry.path().extension() == ".dds")
					dwFileCount++;

			if (dwFileCount < 1) { error("[main] no dds textures found"); return; }
			ppszDdsPath = new("main, dds path 2") rage::ConstString[dwFileCount];
			DWORD dwIndex = 0;
			for (const auto& entry : std::filesystem::directory_iterator(argv[4]))
				if (entry.path().extension() == ".dds")
					ppszDdsPath[dwIndex++] = entry.path().string().c_str();
		}
		if (!strcmp(argv[1], "-c_wtd_v8"))
			wtd::v8fromDds(pszOutFile, dwFileCount, ppszDdsPath);
		else
			wtd::v11fromDds(pszOutFile, dwFileCount, ppszDdsPath);
		dealloc_arr(ppszDdsPath);

	}else printInfo();
}

