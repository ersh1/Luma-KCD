#pragma once

#include "RE/RE.h"

class Offsets
{
public:
	static inline uintptr_t baseAddress;
	static inline RE::CD3D9Renderer* gcpRendD3D;

	/*using tSD3DPostEffectsUtils_CreateRenderTarget = bool (*)(const char* a_szTexName, RE::CTexture*& a_pTex, int a_iWidth, int a_iHeight, bool a_bUseAlpha, bool a_bMipMaps, RE::ETEX_Format a_eTF, int a_nCustomID, int a_nFlags);
	using tCTexture_CreateTextureObject = RE::CTexture* (*)(const char* a_name, uint32_t a_nWidth, uint32_t a_nHeight, int a_nDepth, RE::ETEX_Type a_eTT, uint32_t a_nFlags, RE::ETEX_Format a_eTF, int a_nCustomID);

	static inline tSD3DPostEffectsUtils_CreateRenderTarget SD3DPostEffectsUtils_CreateRenderTarget;
	static inline tCTexture_CreateTextureObject            CTexture_CreateTextureObject;*/

	//using tC3DEngine_GetHDRSetupParams = void (*)(void* a_unused, RE::HDRSetupParams& a_pParams);
	//static inline tC3DEngine_GetHDRSetupParams C3DEngine_GetHDRSetupParams;

	static void Init()
	{
		baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(TEXT("WHGame.dll")));

		gcpRendD3D = reinterpret_cast<RE::CD3D9Renderer*>(baseAddress + 0x339F000);

		/*SD3DPostEffectsUtils_CreateRenderTarget = reinterpret_cast<tSD3DPostEffectsUtils_CreateRenderTarget>(baseAddress + 0x802F04);
		CTexture_CreateTextureObject = reinterpret_cast<tCTexture_CreateTextureObject>(baseAddress + 0x2DE22C);*/

		//C3DEngine_GetHDRSetupParams = reinterpret_cast<tC3DEngine_GetHDRSetupParams>(baseAddress + 0x7FCA94);
	}
};