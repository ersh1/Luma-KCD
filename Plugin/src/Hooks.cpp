#include "Hooks.h"

#include "Settings.h"

#include <dxgi1_4.h>

static const float MaxGamutExpansion[2] =
{
	0.0037935f,
	0.0102255f
};

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

	HRESULT Hooks::OnPresent(IDXGISwapChain* a_pSwapChain, uint32_t a_syncInterval, uint32_t a_presentFlags)
	{
		ID3D11DeviceContext*    ctx = nullptr;
		ID3D11RenderTargetView* v1 = nullptr;
		ID3D11DepthStencilView* v2 = nullptr;

		ID3D11Device* dev;
		if (SUCCEEDED(a_pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&dev))) {
			dev->GetImmediateContext(&ctx);
			ctx->OMGetRenderTargets(1, &v1, &v2);
		}

		HRESULT r = a_pSwapChain->Present(a_syncInterval, a_presentFlags);

		if (ctx != nullptr) {
			ctx->OMSetRenderTargets(1, &v1, v2);
			if (v1 != nullptr) {
				v1->Release();
			}
			if (v2 != nullptr) {
				v2->Release();
			}
			ctx->Release();
		}

		return r;
	}

	HRESULT Hooks::Hook_Present(IDXGISwapChain* a_pSwapChain, uint32_t a_syncInterval, uint32_t a_presentFlags)
	{
		return OnPresent(a_pSwapChain, a_syncInterval, a_presentFlags);
	}

	HRESULT Hooks::Hook_Present_UIOnly(IDXGISwapChain* a_pSwapChain, uint32_t a_syncInterval, uint32_t a_presentFlags)
	{
		return OnPresent(a_pSwapChain, a_syncInterval, a_presentFlags);
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

	float NitsToPQ(float Y)
	{
		Y = std::powf(Y / 10000.f, 0.1593017578125f);

		// E'
		return std::powf((0.8359375f + 18.8515625f * Y) / (1.f + 18.6875f * Y), 78.84375f);
	}

	bool Hooks::Hook_FXSetPSFloat(RE::CShader* a_this, const RE::CCryNameR& a_nameParam, RE::Vec4* a_fParams, int a_nParams)
	{
		// run original
		_Hook_FXSetPSFloat(a_this, a_nameParam, a_fParams, a_nParams);

		// run with ours
		static RE::CCryNameR lumaParamsName { "LumaTonemappingParams" };

		const auto settings = Settings::Main::GetSingleton();

		// max luminance
		float fMaxLuminance = settings->PeakLuminance.GetValue();
		float fMaxLuminanceHalf = fMaxLuminance * 0.5f;
		fMaxLuminance = NitsToPQ(fMaxLuminance);
		fMaxLuminanceHalf = NitsToPQ(fMaxLuminanceHalf);

		// game paperwhite
		//RE::HDRSetupParams hdrParams;
		//Offsets::C3DEngine_GetHDRSetupParams(nullptr, hdrParams);

		float fLuminanceMultiplier = settings->LuminanceMultiplier.GetValue();
		//fPaperWhite /= std::powf(10.f, 0.034607309f + 0.7577371f * log10(hdrParams.HDRFilmCurve.w * 203.f));

		// extend gamut
		float fExtendGamut = settings->ExtendGamut.GetValue();
		int32_t iExtendGamutTarget = settings->ExtendGamutTarget.GetValue();
		fExtendGamut *= MaxGamutExpansion[iExtendGamutTarget];
		fExtendGamut += 1.f;
		// store gamut target as sign
		fExtendGamut *= float(iExtendGamutTarget * 2 - 1);

		RE::Vec4 lumaParams = { fMaxLuminance, fMaxLuminanceHalf, fLuminanceMultiplier, fExtendGamut };

		bool bSuccess = _Hook_FXSetPSFloat(a_this, lumaParamsName, &lumaParams, 1);
		return bSuccess;
	}

	bool Hooks::Hook_mfParseParamComp(void* a_this, int comp, RE::SCGParam* pCurParam, const char* szSemantic, char* params, const char* szAnnotations, void* FXParams, void* ef, uint32_t nParamFlags, RE::EHWShaderClass eSHClass, bool bExpressionOperand)
	{
		// run original
		bool bResult = _Hook_mfParseParamComp(a_this, comp, pCurParam, szSemantic, params, szAnnotations, FXParams, ef, nParamFlags, eSHClass, bExpressionOperand);

		// our param will fail to parse fully because there's no relevant entry in the sParams array, so finish it up manually
		if (!bResult && pCurParam && stricmp (szSemantic, "PB_SFLumaParamaters") == 0) {
			pCurParam->m_eCGParamType = RE::ECGParam::ECGP_LumaParamaters;
			return true; 
		}

		return bResult;
	}

	bool Hooks::Hook_SetShaderParameters(float*& pSrc, RE::ECGParam paramType)
	{
		// run original
		bool bResult = _Hook_SetShaderParameters(pSrc, paramType);

		// our enum value will fail to be recognized here; handle it manually
		if (!bResult && paramType == RE::ECGParam::ECGP_LumaParamaters) {
			const auto settings = Settings::Main::GetSingleton();

			float fUILuminance = settings->UILuminance.GetValue();
			float fLuminanceMultiplier = settings->LuminanceMultiplier.GetValue();
			float fMaxLuminance = settings->PeakLuminance.GetValue();
			float fMaxLuminanceHalf = fMaxLuminance * 0.5f;

			fMaxLuminance = NitsToPQ(fMaxLuminance);
			fMaxLuminanceHalf = NitsToPQ(fMaxLuminanceHalf);

			pSrc[0] = fUILuminance / 80.f;
			pSrc[1] = fLuminanceMultiplier;
			pSrc[2] = fMaxLuminance;
			pSrc[3] = fMaxLuminanceHalf;
			return true;
		}

		return bResult;
	}

	bool Hooks::Hook_FX_DeferredShadowMaskGen_FX_PushRenderTarget(RE::CD3D9Renderer* a_this, int a_nTarget, RE::D3DSurface* a_pTarget, RE::SDepthTexture* a_pDepthTarget, uint32_t a_nTileCount)
	{
		bool bResult = _Hook_FX_DeferredShadowMaskGen_FX_PushRenderTarget(a_this, a_nTarget, a_pTarget, a_pDepthTarget, a_nTileCount);

		// clear the target that was just pushed

		a_this->RT_SetViewport(0, 0, a_this->GetWidth(), a_this->GetHeight());

		RE::ColorF clearColor = { 0.f, 0.f, 0.f, 0.f };
		a_this->EF_ClearBuffers(FRT_CLEAR_COLOR | FRT_CLEAR_IMMEDIATE, &clearColor);

		return bResult;
	}

	void Install()
	{
		Patches::Patch();
		Hooks::Hook();
	}
}
