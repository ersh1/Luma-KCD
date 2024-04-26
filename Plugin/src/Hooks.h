#pragma once

#include "Offsets.h"
#include "RE/RE.h"

namespace Hooks
{
	class Patches
	{
	public:
		static void Patch()
		{
			
			// Patch internal CryEngine RGBA8 to RGBA16F
			{
				constexpr uint8_t format = 0x13;
				//dku::Hook::WriteImm(Offsets::baseAddress + 0x46518C, format);   // $SceneDiffuse
				//dku::Hook::WriteImm(Offsets::baseAddress + 0x4651AC, format);   // $SceneSpecular
				//dku::Hook::WriteImm(Offsets::baseAddress + 0x465085, format);   // $SceneNormalsMap

				dku::Hook::WriteImm(Offsets::baseAddress + 0x802A6B, format);   // $PrevFrameScaled

				dku::Hook::WriteImm(Offsets::baseAddress + 0x802A38, format);   // $BackBufferScaled_d2
				dku::Hook::WriteImm(Offsets::baseAddress + 0x802A97, format);   // $BackBufferScaledTemp_d2
				dku::Hook::WriteImm(Offsets::baseAddress + 0x802B54, format);   // $BackBufferScaled_d4
				dku::Hook::WriteImm(Offsets::baseAddress + 0x802B8A, format);   // $BackBufferScaledTemp_d4
				dku::Hook::WriteImm(Offsets::baseAddress + 0x802BBD, format);   // $BackBufferScaled_d8

				//dku::Hook::WriteImm(Offsets::baseAddress + 0xB125E8, format);   // $BackBuffer
				//dku::Hook::WriteImm(Offsets::baseAddress + 0xB1259A, format);   // $BackBuffer
				
				dku::Hook::WriteImm(Offsets::baseAddress + 0x8029B2, format);   // $PrevBackBuffer0
				dku::Hook::WriteImm(Offsets::baseAddress + 0x8029DE, format);   // $PrevBackBuffer1
				dku::Hook::WriteImm(Offsets::baseAddress + 0x130E408, format);  // $PrevBackBuffer0_R
				dku::Hook::WriteImm(Offsets::baseAddress + 0x130E433, format);  // $PrevBackBuffer1_R
			}

			// Patch DXGI_FORMAT RGBA8 to RGBA16F
			{
				constexpr uint8_t format = 0x0A;
				dku::Hook::WriteImm(Offsets::baseAddress + 0x8E9D8A, format);  // Swapchain
			}

			// Patch DXGI_SWAP_EFFECT to DXGI_SWAP_EFFECT_FLIP_DISCARD
			{
				struct Patch : Xbyak::CodeGenerator
				{
					Patch()
					{
						// DXGI_SWAP_EFFECT_FLIP_DISCARD instead of DXGI_SWAP_EFFECT_DISCARD
						mov(dword[rdi + 0x3C], DXGI_SWAP_EFFECT_FLIP_DISCARD);

						// original code we wrote over
						add(eax, 2);
					}
				};

				Patch patch{};
				patch.ready();

				auto offset = std::make_pair(0x314, 0x31B);
				auto hook = dku::Hook::AddASMPatch(Offsets::baseAddress + 0x8E9B30, offset, &patch);
				hook->Enable();
			}
		}
	};
	class Hooks
	{
	public:
		static void Hook()
		{
			// Hook CD3D9Renderer::FlashRenderInternal because DXGI_SWAP_EFFECT_FLIP_DISCARD unbinds backbuffer during Present. So we need to call OMSetRenderTargets to bind it again every frame.
			{
				uintptr_t vtable = Offsets::baseAddress + 0x215B148;
				auto      Hook = dku::Hook::AddVMTHook(&vtable, 0x17B, FUNC_INFO(Hook_FlashRenderInternal));

				using FlashRenderInternal_t = std::add_pointer_t<void(RE::CD3D9Renderer*, void*, bool, bool)>;
				_Hook_FlashRenderInternal = Hook->GetOldFunction<FlashRenderInternal_t>();
				Hook->Enable();
			}

			// Hook swapchain creation and set colorspace
			{
				_Hook_CreateDevice = dku::Hook::write_call(Offsets::baseAddress + 0x2A841C, Hook_CreateDevice);
			}

			// Replicate how the game treats render targets
			{
				// Hook SD3DPostEffectsUtils::CreateRenderTarget for $SceneDiffuse in CDeferredShading::CreateDeferredMaps
				_Hook_CreateRenderTarget = dku::Hook::write_call(Offsets::baseAddress + 0x465194, Hook_CreateRenderTarget);

				// Hook CTexture::CreateTextureObject for $SceneDiffuse
				_Hook_CreateTextureObject = dku::Hook::write_call(Offsets::baseAddress + 0x803E3C, Hook_CreateTextureObject);
			}

			// Replace with our new RTs
			{
				_Hook_FX_PushRenderTarget = dku::Hook::write_call(Offsets::baseAddress + 0x130A5F4, Hook_FX_PushRenderTarget);  // use TonemapTarget as target for tonemapper

				// Read TonemapTarget instead of SceneDiffuse
				{
					struct Patch : Xbyak::CodeGenerator
					{
						Patch(uintptr_t a_addr)
						{
							//// call our function
							mov(rax, a_addr);
							call(rax);
							mov(rdi, rax);
						}
					};

					Patch patch(reinterpret_cast<uintptr_t>(GetTonemapTargetRT));
					patch.ready();

					auto offset = std::make_pair(0x69, 0x70);
					auto hook = dku::Hook::AddASMPatch(Offsets::baseAddress + 0x7FE0C4, offset, &patch);
					hook->Enable();
				}

				// Use PostAATarget instead of SceneNormalsMap #1
				{
					struct Patch : Xbyak::CodeGenerator
					{
						Patch(uintptr_t a_addr)
						{
							push(rax);

							//// call our function
							mov(rax, a_addr);
							call(rax);
							mov(rbx, rax);

							pop(rax);
						}
					};

					Patch patch(reinterpret_cast<uintptr_t>(GetPostAATargetRT));
					patch.ready();

					auto offset = std::make_pair(0x25, 0x2C);
					auto hook = dku::Hook::AddASMPatch(Offsets::baseAddress + 0x7FDC7C, offset, &patch);
					hook->Enable();
				}

				// Use PostAATarget instead of SceneNormalsMap #2
				{
					struct Patch : Xbyak::CodeGenerator
					{
						Patch(uintptr_t a_addr)
						{
							//// call our function
							mov(rax, a_addr);
							call(rax);
							mov(rbx, rax);
						}
					};

					Patch patch(reinterpret_cast<uintptr_t>(GetPostAATargetRT));
					patch.ready();

					auto offset = std::make_pair(0xB0D0CE, 0xB0D0D5);
					auto hook = dku::Hook::AddASMPatch(Offsets::baseAddress + 0x7FDC7C, offset, &patch);
					hook->Enable();
				}
			}
		}

	private:
		static void          Hook_FlashRenderInternal(RE::CD3D9Renderer* a_this, void* pPlayer, bool bStereo, bool bDoRealRender);
		static bool          Hook_CreateDevice(RE::DeviceInfo* a_deviceInfo, bool a_bWindowed, int32_t a_width, int32_t a_height, int32_t a_backbufferWidth, int32_t a_backbufferHeight, int32_t a_zbpp);
		static bool          Hook_CreateRenderTarget(const char* a_szTexName, RE::CTexture*& a_pTex, int a_iWidth, int a_iHeight, bool a_bUseAlpha, bool a_bMipMaps, RE::ETEX_Format a_eTF, int a_nCustomID, int a_nFlags);
		static RE::CTexture* Hook_CreateTextureObject(const char* a_name, uint32_t a_nWidth, uint32_t a_nHeight, int a_nDepth, RE::ETEX_Type a_eTT, uint32_t a_nFlags, RE::ETEX_Format a_eTF, int a_nCustomID);
		static bool          Hook_FX_PushRenderTarget(RE::CD3D9Renderer* a_this, int a_nTarget, RE::CTexture* a_pTarget, RE::SD3DSurface* a_pDepthTarget, bool a_bClearOnResolve, int a_nCMSide, bool a_bScreenVP, uint32_t a_nTileCount);

		static inline std::add_pointer_t<decltype(Hook_FlashRenderInternal)> _Hook_FlashRenderInternal;
		static inline std::add_pointer_t<decltype(Hook_CreateDevice)>        _Hook_CreateDevice;
		static inline std::add_pointer_t<decltype(Hook_CreateRenderTarget)> _Hook_CreateRenderTarget;
		static inline std::add_pointer_t<decltype(Hook_CreateTextureObject)> _Hook_CreateTextureObject;
		static inline std::add_pointer_t<decltype(Hook_FX_PushRenderTarget)> _Hook_FX_PushRenderTarget;

		static uintptr_t            GetTonemapTargetRT() { return reinterpret_cast<uintptr_t>(ptexTonemapTarget); }
		static uintptr_t            GetPostAATargetRT() { return reinterpret_cast<uintptr_t>(ptexPostAATarget); }
		static inline RE::CTexture* ptexTonemapTarget;
		static inline RE::CTexture* ptexPostAATarget;
	};

	void Install();
}