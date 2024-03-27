#pragma once

#include "libertyFourXYZ/rage_base.h"

namespace rage {
	class grcTexturePC;
}

//using namespace rage;

namespace rage_mp3 {

	class grcTexture : public rage::pgBaseRefCounted {
		void addToLayout(libertyFourXYZ::rsc85_layout* pLayout, DWORD dwDepth) { pgBaseRefCounted::addToLayout(pLayout, dwDepth); }
		void replacePtrs(libertyFourXYZ::rsc85_layout* pLayout, rage::datResource* pRsc, DWORD dwDepth) { pgBaseRefCounted::replacePtrs(pLayout, pRsc, dwDepth); }
		//void place() // unused
	};

	class grcTexturePC : public grcTexture {
	public:
		WORD m_wResourceType; // 0 - regular txd, 1 - target, 2 - ref
		WORD m_wLayerCount;
		void* m_pCachedTexture;
		DWORD m_dwPhysicalSize_unused;
		rage::ConstString m_pszName;
		BYTE __1c[0x4];
		WORD m_wWidth;
		WORD m_wHeight;
		WORD _f24; // = 1
		WORD m_wStride;
		DWORD m_dwPixelFormat;
		BYTE m_eTextureType;
		BYTE m_nbLevels;
		WORD _f2e;
		float m_fColorExpR;
		float m_fColorExpG;
		float m_fColorExpB;
		float m_fColorOfsR;
		float m_fColorOfsG;
		float m_fColorOfsB;
		grcTexturePC* m_pPrev;
		grcTexturePC* m_pNext;
		BYTE* m_pPixelData;
		BYTE __54[0x8];

		void recreateName();
		rage::ConstString getClearName();
		DWORD getHash();
		DWORD getPixelDataSize();
		DWORD getFirstMipSize();
		void addToLayout(libertyFourXYZ::rsc85_layout* pLayout, DWORD dwDepth);
		void replacePtrs(libertyFourXYZ::rsc85_layout* pLayout, rage::datResource* pRsc, DWORD dwDepth);
		void clearRefCount();
		void setRefCount();
		void fromV8(rage::grcTexturePC* pTxd);
		~grcTexturePC();
		grcTexturePC();


	};
}