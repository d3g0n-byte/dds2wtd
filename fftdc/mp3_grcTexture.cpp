#include "mp3_grcTexture.h"
#include "libertyFourXYZ/rage_grcTexture.h"

namespace rage_mp3 {

	void grcTexturePC::addToLayout(libertyFourXYZ::rsc85_layout* pLayout, DWORD dwDepth) {
		pgBaseRefCounted::addToLayout(pLayout, dwDepth);
		this->m_pszName.addToLayout(pLayout, dwDepth + 1);
		pLayout->addObject<BYTE>(this->m_pPixelData, 6, getPixelDataSize());
	}

	void grcTexturePC::replacePtrs(libertyFourXYZ::rsc85_layout* pLayout, rage::datResource* pRsc, DWORD dwDepth) {
		pgBaseRefCounted::replacePtrs(pLayout, pRsc, dwDepth);
		this->m_pszName.replacePtrs(pLayout, pRsc, dwDepth + 1);
		auto pTxd = this->m_pPixelData;
		pLayout->setPtr(this->m_pPixelData);
		DWORD size = getPixelDataSize();
		memcpy(pRsc->getFixup<BYTE>(this->m_pPixelData, size), pTxd, size);
	}

	void grcTexturePC::clearRefCount() {
		pgBaseRefCounted::clearRefCount();
	}

	void grcTexturePC::setRefCount() {
		pgBaseRefCounted::setRefCount();
	}

	rage::ConstString grcTexturePC::getClearName() {
		if (!this->m_pszName) return "";
		DWORD s = this->m_pszName.findLastOf('\\', '/', ':');
		if(s == rage::ConstString::npos)
			return this->m_pszName.getFileNameWithoutExt();
		else return rage::ConstString(this->m_pszName + s).getFileNameWithoutExt();
	}
	
	// rdr style names
	void grcTexturePC::recreateName() {
		if (!this->m_pszName) return;
		this->m_pszName = rage::ConstString::format("memory:$%08x,%u,%u:%s.dds", this, this->getPixelDataSize() + 168, 0, this->getClearName());
	}

	void grcTexturePC::fromV8(rage::grcTexturePC* pTxd) {
		this->m_wResourceType = rage::TEXTURE;
		m_wLayerCount = 0;
		m_pCachedTexture = NULL;
		m_dwPhysicalSize_unused = 0;
		m_pszName = pTxd->m_pszName;
		memset(__1c, 0x0, 4);
		m_wWidth = pTxd->m_wWidth;
		m_wHeight = pTxd->m_wHeight;
		_f24 = 1;
		m_wStride = pTxd->m_wMipStride;
		m_dwPixelFormat = pTxd->m_dwPixelFormat;
		m_eTextureType = pTxd->m_nbTextureType;
		m_nbLevels = pTxd->m_nbLevels;
		_f2e = 0;
		m_fColorExpR = pTxd->m_fColorExpR;
		m_fColorExpG = pTxd->m_fColorExpG;
		m_fColorExpB = pTxd->m_fColorExpB;
		m_fColorOfsR = pTxd->m_fColorOfsR;
		m_fColorOfsG = pTxd->m_fColorOfsG;
		m_fColorOfsB = pTxd->m_fColorOfsB;
		m_pPrev = NULL;
		m_pNext = NULL;
		if (pTxd->m_pPixelData) {
			DWORD size = getPixelDataSize();
			m_pPixelData = libertyFourXYZ::g_memory_manager.allocate<BYTE>("convert txd v8 to v11", size);
			memcpy(m_pPixelData, pTxd->m_pPixelData, size);
		}
		m_dwRefCount = pTxd->m_wUsageCount;
		memset(__54, 0x0, 8);
	}

	grcTexturePC::~grcTexturePC() {
		if (m_pPixelData)
			libertyFourXYZ::g_memory_manager.release(m_pPixelData);
	}

	grcTexturePC::grcTexturePC() {
		zero_fill_class(this);
	}

	DWORD grcTexturePC::getFirstMipSize() {
		return this->m_wStride * this->m_wHeight;
	}

	DWORD grcTexturePC::getPixelDataSize() {
		DWORD base = this->getFirstMipSize();
		DWORD retVal = base;
		for (BYTE a = 1; a < this->m_nbLevels; a++)
			retVal += ((base) >> (a * 2));
		return retVal;
	}

	DWORD grcTexturePC::getHash() { return atStringHash(this->getClearName(), 0); }


}
