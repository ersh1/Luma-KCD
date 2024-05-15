#include "Hooks.h"

#include "Settings.h"

#include <dxgi1_4.h>


namespace Hooks
{
	ID3D11DeviceContext* GetDeviceContext(RE::CD3D9Renderer* a_renderer)
	{
		if (a_renderer->m_nAsyncDeviceState) {
			while (a_renderer->m_nAsyncDeviceState) {
				SwitchToThread();
			}
		}
		return a_renderer->m_pDeviceContext;
	}

	void Hooks::Hook_FlashRenderInternal(RE::CD3D9Renderer* a_this, void* pPlayer, bool bStereo, bool bDoRealRender)
	{
		if (bDoRealRender) {
			auto context = GetDeviceContext(a_this);
			context->OMSetRenderTargets(1, &a_this->m_pBackBuffer, a_this->m_pNativeZSurface);
		}

		_Hook_FlashRenderInternal(a_this, pPlayer, bStereo, bDoRealRender);
	}

	bool Hooks::Hook_CreateDevice(RE::DeviceInfo* a_deviceInfo, bool a_bWindowed, int32_t a_width, int32_t a_height, int32_t a_backbufferWidth, int32_t a_backbufferHeight, int32_t a_zbpp)
	{
		bool bReturn = _Hook_CreateDevice(a_deviceInfo, a_bWindowed, a_width, a_height, a_backbufferWidth, a_backbufferHeight, a_zbpp);

		// set colorspace
		IDXGISwapChain3* swapChain3 = nullptr;
		a_deviceInfo->m_pSwapChain->QueryInterface(__uuidof(IDXGISwapChain3), reinterpret_cast<void**>(&swapChain3));

		DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;

		swapChain3->SetColorSpace1(colorSpace);
		swapChain3->Release();

		// good moment late enough to register reshade settings
		Settings::Main::GetSingleton()->RegisterReshadeOverlay();

		return bReturn;
	}

	bool Hooks::Hook_CreateRenderTarget(const char* a_szTexName, RE::CTexture*& a_pTex, int a_iWidth, int a_iHeight, bool a_bUseAlpha, bool a_bMipMaps, RE::ETEX_Format a_eTF, int a_nCustomID, int a_nFlags)
	{
		bool bReturn = _Hook_CreateRenderTarget(a_szTexName, a_pTex, a_iWidth, a_iHeight, a_bUseAlpha, a_bMipMaps, a_eTF, a_nCustomID, a_nFlags);

		// add ours
		_Hook_CreateRenderTarget("$TonemapTarget", ptexTonemapTarget, a_iWidth, a_iHeight, a_bUseAlpha, a_bMipMaps, RE::eTF_R16G16B16A16F, -1, a_nFlags);
		_Hook_CreateRenderTarget("$PostAATarget", ptexPostAATarget, a_iWidth, a_iHeight, a_bUseAlpha, a_bMipMaps, RE::eTF_R16G16B16A16F, -1, a_nFlags);
		_Hook_CreateRenderTarget("$NormalCopyTarget", ptexNormalCopyTarget, a_iWidth, a_iHeight, a_bUseAlpha, a_bMipMaps, RE::eTF_R8G8B8A8, -1, a_nFlags);

		return bReturn;
	}

	RE::CTexture* Hooks::Hook_CreateTextureObject(const char* a_name, uint32_t a_nWidth, uint32_t a_nHeight, int a_nDepth, RE::ETEX_Type a_eTT, uint32_t a_nFlags, RE::ETEX_Format a_eTF, int a_nCustomID)
	{
		RE::CTexture* pTex = _Hook_CreateTextureObject(a_name, a_nWidth, a_nHeight, a_nDepth, a_eTT, a_nFlags, a_eTF, a_nCustomID);

		// add ours
		ptexTonemapTarget = _Hook_CreateTextureObject("$TonemapTarget", a_nWidth, a_nHeight, a_nDepth, a_eTT, a_nFlags, RE::eTF_R16G16B16A16F, -1);
		ptexPostAATarget = _Hook_CreateTextureObject("$PostAATarget", a_nWidth, a_nHeight, a_nDepth, a_eTT, a_nFlags, RE::eTF_R16G16B16A16F, -1);
		ptexNormalCopyTarget = _Hook_CreateTextureObject("$NormalCopyTarget", a_nWidth, a_nHeight, a_nDepth, a_eTT, a_nFlags, RE::eTF_R8G8B8A8, -1);

		return pTex;
	}

	bool Hooks::Hook_FX_PushRenderTarget(RE::CD3D9Renderer* a_this, int a_nTarget, RE::CTexture* a_pTarget, RE::SD3DSurface* a_pDepthTarget, bool a_bClearOnResolve, int a_nCMSide, bool a_bScreenVP, uint32_t a_nTileCount)
	{
		// replace with ours
		return _Hook_FX_PushRenderTarget(a_this, a_nTarget, ptexTonemapTarget, a_pDepthTarget, a_bClearOnResolve, a_nCMSide, a_bScreenVP, a_nTileCount);
	}

	float NitsToPQ(float Y)
	{
		Y = std::powf(Y / 10000.f, 0.1593017578125f);

		// E'
		return std::powf((0.8359375f + 18.8515625f * Y)
		               / (1.f        + 18.6875f    * Y)
		       , 78.84375f);
	}

	bool Hooks::Hook_FXSetPSFloat(RE::CShader* a_this, const RE::CCryNameR& a_nameParam, RE::Vec4* a_fParams, int a_nParams)
	{
		// run original
		_Hook_FXSetPSFloat(a_this, a_nameParam, a_fParams, a_nParams);

		// run with ours
		static RE::CCryNameR lumaParamsName { "LumaTonemappingParams" };

		const auto settings = Settings::Main::GetSingleton();
		float fMaxLuminance = settings->PeakBrightness.GetValue();
		float fMaxLuminanceHalf = fMaxLuminance * 0.5f;
		fMaxLuminance = NitsToPQ(fMaxLuminance);
		fMaxLuminanceHalf = NitsToPQ(fMaxLuminanceHalf);
		float fPaperwhite = settings->GamePaperWhite.GetValue();
		float fExtendGamut = settings->ExtendGamut.GetValue();

		RE::Vec4 lumaParams = { fMaxLuminance, fMaxLuminanceHalf, fPaperwhite, fExtendGamut };

		bool bSuccess = _Hook_FXSetPSFloat(a_this, lumaParamsName, &lumaParams, 1);
		return bSuccess;
	}

	void Install()
	{
		Patches::Patch();
		Hooks::Hook();
	}
}
