#pragma once
#include <Windows.h>
namespace rage{ class ConstString; }

namespace wtd {
	void v8fromDds(rage::ConstString& pszOutFile, DWORD dwCount, rage::ConstString* ppszDdsPath);
	void v11fromDds(rage::ConstString& pszOutFile, DWORD dwCount, rage::ConstString* ppszDdsPath);
}