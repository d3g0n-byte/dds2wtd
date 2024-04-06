
#include "libertyFourXYZ/memory_manager.h"
#include "libertyFourXYZ/rage_base.h"
#include "libertyFourXYZ/rage_grcTexture.h"
#include "mp3_grcTexture.h"

namespace wtd {

	void v8fromDds(rage::ConstString& pszOutFile, DWORD dwCount, rage::ConstString* ppszDdsPath) {
		rage::pgDictionary<rage::grcTexturePC>* pTextureDict = new ("wtdFromDdsFiles, txdDict") rage::pgDictionary<rage::grcTexturePC>;
		for (DWORD i = 0; i < dwCount; i++) {
			rage::grcTexturePC* pTxd = new("wtd from dds files, for") rage::grcTexturePC;
			rage::ConstString* path = ppszDdsPath + i;
			int result = pTxd->fromDdsPath(*path);
			if (!result) pTextureDict->addElement(pTxd->getHash(), pTxd);
			else dealloc(pTxd);
		}

		for (WORD i = 0; i < pTextureDict->m_entries.size(); i++) {
			rage::grcTexturePC* pTexture = pTextureDict->m_entries[i].pElement;
			pTexture->recreateName();
		}
		pTextureDict->clearRefCount();

		libertyFourXYZ::rsc85_layout layout;
		layout.setMainObject(pTextureDict);

		pTextureDict->setRefCount();
		pTextureDict->addToLayout(&layout, 0);
		layout.create();

		rage::datResource* pResource = new("wtdFromDdsFiles, res") rage::datResource(pszOutFile.getFileNameWithoutExt());

		pResource->m_pMap->validateMap(layout.getResourceInfo());
		pTextureDict->replacePtrs(&layout, pResource, 0);
		memcpy(pResource->getFixup((rage::pgDictionary<rage::grcTexturePC>*)layout.mainObj.second, sizeof(*pTextureDict)), pTextureDict, sizeof(*pTextureDict));

		layout.setOldPtrs();
		dealloc(pTextureDict);

		//pResource->saveRawResource(pszOutFile.getFilePath(), pszOutFile.getFileNameWithoutExt(), 1);
		pResource->saveResource(pszOutFile.getFilePath(), pszOutFile.getFileNameWithoutExt(), "wtd", 8, layout.getResourceInfo(), 0);

		dealloc(pResource);
	}

	// 
	void v11fromDds(rage::ConstString& pszOutFile, DWORD dwCount, rage::ConstString* ppszDdsPath) {
		rage::pgDictionary<rage::grcTexturePC>* pTextureDictOld = new("wtdFromDdsFiles v8 to v11, txdDictOld") rage::pgDictionary<rage::grcTexturePC>;
		for (DWORD i = 0; i < dwCount; i++) {
			rage::grcTexturePC* pTxd = new("wtd from dds files, for")rage::grcTexturePC;
			rage::ConstString* path = ppszDdsPath + i;
			int result = pTxd->fromDdsPath(*path);
			if (!result) pTextureDictOld->addElement(pTxd->getHash(), pTxd);
			else dealloc(pTxd);
		}

		// to v11
		rage::pgDictionary<rage_mp3::grcTexturePC>* pTextureDict = new("wtdFromDdsFiles v8 to v11, txdDictNew") rage::pgDictionary<rage_mp3::grcTexturePC>;
		for (WORD i = 0; i < pTextureDictOld->m_entries.m_count; i++) {
			rage_mp3::grcTexturePC* pTxd = new("txd v11") rage_mp3::grcTexturePC;
			pTxd->fromV8(pTextureDictOld->m_entries.m_pElements[i].get());
			pTextureDict->addElement(pTextureDictOld->m_codes[i], pTxd);
		}

		// delete old
		dealloc(pTextureDictOld);

		for (WORD i = 0; i < pTextureDict->m_entries.size(); i++) {
			rage_mp3::grcTexturePC* pTexture = pTextureDict->m_entries[i].pElement;
			pTexture->recreateName();
		}

		pTextureDict->clearRefCount();

		libertyFourXYZ::rsc85_layout layout;
		layout.setMainObject(pTextureDict);

		pTextureDict->setRefCount();
		pTextureDict->addToLayout(&layout, 0);
		layout.create();

		rage::datResource* pResource = new("wtdFromDdsFiles v11, res") rage::datResource(pszOutFile.getFileNameWithoutExt());

		pResource->m_pMap->validateMap(layout.getResourceInfo());
		pTextureDict->replacePtrs(&layout, pResource, 0);
		memcpy(pResource->getFixup((rage::pgDictionary<rage_mp3::grcTexturePC>*)layout.mainObj.second, sizeof(*pTextureDict)), pTextureDict, sizeof(*pTextureDict));
		memset((size_t*)pResource->m_pMap->chunks[0].pDest + 1, 0xff, sizeof(size_t)); // very stupid fix

		layout.setOldPtrs();
		dealloc(pTextureDict);

		pResource->saveRawResource(pszOutFile.getFilePath(), pszOutFile.getFileNameWithoutExt(), 1);
		pResource->saveResource(pszOutFile.getFilePath(), pszOutFile.getFileNameWithoutExt(), "wtd", 11, layout.getResourceInfo(), 0);

		dealloc(pResource);

	}

}