#pragma once

#include <d3d11_1.h>

namespace RE
{
	class CTexture;
	class CShader;

	template <class T>
	struct Color_tpl
	{
		T r, g, b, a;
	};

	typedef Color_tpl<float> ColorF;

	enum ETEX_Format : uint8_t
	{
		eTF_Unknown = 0x0,
		eTF_R8G8B8A8S = 0x1,
		eTF_R8G8B8A8 = 0x2,
		eTF_A8 = 0x4,
		eTF_R8 = 0x5,
		eTF_R8S = 0x6,
		eTF_R16 = 0x7,
		eTF_R16F = 0x8,
		eTF_R32F = 0x9,
		eTF_R8G8 = 0xA,
		eTF_R8G8S = 0xB,
		eTF_R16G16 = 0xC,
		eTF_R16G16S = 0xD,
		eTF_R16G16F = 0xE,
		eTF_R11G11B10F = 0xF,
		eTF_R10G10B10A2 = 0x10,
		eTF_R16G16B16A16 = 0x11,
		eTF_R16G16B16A16S = 0x12,
		eTF_R16G16B16A16F = 0x13,
		eTF_R32G32B32A32F = 0x14,
		eTF_CTX1 = 0x15,
		eTF_BC1 = 0x16,
		eTF_BC2 = 0x17,
		eTF_BC3 = 0x18,
		eTF_BC4U = 0x19,
		eTF_BC4S = 0x1A,
		eTF_BC5U = 0x1B,
		eTF_BC5S = 0x1C,
		eTF_BC6UH = 0x1D,
		eTF_BC6SH = 0x1E,
		eTF_BC7 = 0x1F,
		eTF_R9G9B9E5 = 0x20,
		eTF_D16 = 0x21,
		eTF_D24S8 = 0x22,
		eTF_D32F = 0x23,
		eTF_D32FS8 = 0x24,
		eTF_B5G6R5 = 0x25,
		eTF_B5G5R5 = 0x26,
		eTF_B4G4R4A4 = 0x27,
		eTF_EAC_R11 = 0x28,
		eTF_EAC_RG11 = 0x29,
		eTF_ETC2 = 0x2A,
		eTF_ETC2A = 0x2B,
		eTF_A8L8 = 0x2C,
		eTF_L8 = 0x2D,
		eTF_L8V8U8 = 0x2E,
		eTF_B8G8R8 = 0x2F,
		eTF_L8V8U8X8 = 0x30,
		eTF_B8G8R8X8 = 0x31,
		eTF_B8G8R8A8 = 0x32,
		eTF_PVRTC2 = 0x33,
		eTF_PVRTC4 = 0x34,
		eTF_ASTC_4x4 = 0x35,
		eTF_ASTC_5x4 = 0x36,
		eTF_ASTC_5x5 = 0x37,
		eTF_ASTC_6x5 = 0x38,
		eTF_ASTC_6x6 = 0x39,
		eTF_ASTC_8x5 = 0x3A,
		eTF_ASTC_8x6 = 0x3B,
		eTF_ASTC_8x8 = 0x3C,
		eTF_ASTC_10x5 = 0x3D,
		eTF_ASTC_10x6 = 0x3E,
		eTF_ASTC_10x8 = 0x3F,
		eTF_ASTC_10x10 = 0x40,
		eTF_ASTC_12x10 = 0x41,
		eTF_ASTC_12x12 = 0x42,
		eTF_R16U = 0x43,
		eTF_R16G16U = 0x44,
		eTF_R10G10B10A2UI = 0x45,
		eTF_MaxFormat = 0x46,
	};

	enum ETEX_Type : uint8_t
	{
		eTT_1D = 0,
		eTT_2D,
		eTT_3D,
		eTT_Cube,
		eTT_AutoCube,
		eTT_Auto2D,
		eTT_User,
		eTT_NearestCube,
		eTT_MaxTexType,
	};

#define FRT_CLEAR_COLOR 0x1
#define FRT_CLEAR_DEPTH 0x2
#define FRT_CLEAR_STENCIL 0x4
#define FRT_CLEAR (FRT_CLEAR_COLOR | FRT_CLEAR_DEPTH | FRT_CLEAR_STENCIL)
#define FRT_CLEAR_FOGCOLOR 0x8
#define FRT_CLEAR_IMMEDIATE 0x10
#define FRT_CLEAR_COLORMASK 0x20

	class CD3D9Renderer
	{
	public:
		virtual void Unk00();
		virtual void Unk01();
		virtual void Unk02();
		virtual void Unk03();
		virtual void Unk04();
		virtual void Unk05();
		virtual void Unk06();
		virtual void Unk07();
		virtual void Unk08();
		virtual void Unk09();
		virtual void Unk0A();
		virtual void Unk0B();
		virtual void Unk0C();
		virtual void Unk0D();
		virtual void Unk0E();
		virtual void Unk0F();
		virtual void Unk10();
		virtual void Unk11();
		virtual void Unk12();
		virtual void Unk13();
		virtual void Unk14();
		virtual void Unk15();
		virtual void Unk16();
		virtual void Unk17();
		virtual void Unk18();
		virtual void Unk19();
		virtual void Unk1A();
		virtual void Unk1B();
		virtual void Unk1C();
		virtual void Unk1D();
		virtual void Unk1E();
		virtual void Unk1F();
		virtual void Unk20();
		virtual void Unk21();
		virtual void Unk22();
		virtual void Unk23();
		virtual void Unk24();
		virtual void Unk25();
		virtual void Unk26();
		virtual void Unk27();
		virtual void Unk28();
		virtual void Unk29();
		virtual void Unk2A();
		virtual void Unk2B();
		virtual void Unk2C();
		virtual void Unk2D();
		virtual void Unk2E();
		virtual void Unk2F();
		virtual void Unk30();
		virtual void Unk31();
		virtual void Unk32();
		virtual void Unk33();
		virtual void Unk34();
		virtual void Unk35();
		virtual void Unk36();
		virtual void Unk37();
		virtual void Unk38();
		virtual void Unk39();
		virtual void Unk3A();
		virtual void Unk3B();
		virtual void Unk3C();
		virtual void Unk3D();
		virtual void Unk3E();
		virtual void Unk3F();
		virtual void Unk40();
		virtual void Unk41();
		virtual void Unk42();
		virtual void Unk43();
		virtual void Unk44();
		virtual uint32_t GetHeight();
		virtual uint32_t GetWidth();
		virtual void Unk47();
		virtual void Unk48();
		virtual void Unk49();
		virtual void Unk4A();
		virtual void Unk4B();
		virtual void Unk4C();
		virtual void Unk4D();
		virtual void Unk4E();
		virtual void Unk4F();
		virtual void Unk50();
		virtual void Unk51();
		virtual void Unk52();
		virtual void Unk53();
		virtual void Unk54();
		virtual void Unk55();
		virtual void Unk56();
		virtual void Unk57();
		virtual void Unk58();
		virtual void Unk59();
		virtual void Unk5A();
		virtual void Unk5B();
		virtual void Unk5C();
		virtual void Unk5D();
		virtual void Unk5E();
		virtual void Unk5F();
		virtual void Unk60();
		virtual void Unk61();
		virtual void Unk62();
		virtual void Unk63();
		virtual void Unk64();
		virtual void Unk65();
		virtual void Unk66();
		virtual void Unk67();
		virtual void Unk68();
		virtual void Unk69();
		virtual void Unk6A();
		virtual void Unk6B();
		virtual void Unk6C();
		virtual void Unk6D();
		virtual void Unk6E();
		virtual void Unk6F();
		virtual void Unk70();
		virtual void Unk71();
		virtual void Unk72();
		virtual void Unk73();
		virtual void Unk74();
		virtual void Unk75();
		virtual void Unk76();
		virtual void Unk77();
		virtual void Unk78();
		virtual void Unk79();
		virtual void Unk7A();
		virtual void Unk7B();
		virtual void Unk7C();
		virtual void Unk7D();
		virtual void Unk7E();
		virtual void Unk7F();
		virtual void Unk80();
		virtual void Unk81();
		virtual void Unk82();
		virtual void Unk83();
		virtual void Unk84();
		virtual void Unk85();
		virtual void Unk86();
		virtual void Unk87();
		virtual void Unk88();
		virtual void Unk89();
		virtual void Unk8A();
		virtual void Unk8B();
		virtual void Unk8C();
		virtual void Unk8D();
		virtual void Unk8E();
		virtual void Unk8F();
		virtual void Unk90();
		virtual void Unk91();
		virtual void Unk92();
		virtual void Unk93();
		virtual void Unk94();
		virtual void Unk95();
		virtual void Unk96();
		virtual void Unk97();
		virtual void Unk98();
		virtual void Unk99();
		virtual void Unk9A();
		virtual void Unk9B();
		virtual void Unk9C();
		virtual void Unk9D();
		virtual void Unk9E();
		virtual void Unk9F();
		virtual void UnkA0();
		virtual void UnkA1();
		virtual void UnkA2();
		virtual void UnkA3();
		virtual void UnkA4();
		virtual void UnkA5();
		virtual void UnkA6();
		virtual void UnkA7();
		virtual void UnkA8();
		virtual void UnkA9();
		virtual void UnkAA();
		virtual void UnkAB();
		virtual void UnkAC();
		virtual void UnkAD();
		virtual void UnkAE();
		virtual void UnkAF();
		virtual void UnkB0();
		virtual void UnkB1();
		virtual void UnkB2();
		virtual void UnkB3();
		virtual void UnkB4();
		virtual void UnkB5();
		virtual void UnkB6();
		virtual void UnkB7();
		virtual void UnkB8();
		virtual void UnkB9();
		virtual void UnkBA();
		virtual void UnkBB();
		virtual void UnkBC();
		virtual void UnkBD();
		virtual void UnkBE();
		virtual void UnkBF();
		virtual void UnkC0();
		virtual void UnkC1();
		virtual void UnkC2();
		virtual void UnkC3();
		virtual void UnkC4();
		virtual void UnkC5();
		virtual void UnkC6();
		virtual void UnkC7();
		virtual void UnkC8();
		virtual void UnkC9();
		virtual void UnkCA();
		virtual void UnkCB();
		virtual void UnkCC();
		virtual void UnkCD();
		virtual void UnkCE();
		virtual void UnkCF();
		virtual void UnkD0();
		virtual void UnkD1();
		virtual void UnkD2();
		virtual void UnkD3();
		virtual void UnkD4();
		virtual void UnkD5();
		virtual void UnkD6();
		virtual void UnkD7();
		virtual void UnkD8();
		virtual void UnkD9();
		virtual void UnkDA();
		virtual void UnkDB();
		virtual void UnkDC();
		virtual void UnkDD();
		virtual void UnkDE();
		virtual void UnkDF();
		virtual void UnkE0();
		virtual void UnkE1();
		virtual void UnkE2();
		virtual void UnkE3();
		virtual void UnkE4();
		virtual void UnkE5();
		virtual void UnkE6();
		virtual void UnkE7();
		virtual void UnkE8();
		virtual void UnkE9();
		virtual void UnkEA();
		virtual void UnkEB();
		virtual void UnkEC();
		virtual void UnkED();
		virtual void UnkEE();
		virtual void UnkEF();
		virtual void UnkF0();
		virtual void UnkF1();
		virtual void UnkF2();
		virtual void UnkF3();
		virtual void UnkF4();
		virtual void UnkF5();
		virtual void UnkF6();
		virtual void UnkF7();
		virtual void UnkF8();
		virtual void UnkF9();
		virtual void UnkFA();
		virtual void UnkFB();
		virtual void UnkFC();
		virtual void UnkFD();
		virtual void UnkFE();
		virtual void UnkFF();
		virtual void Unk100();
		virtual void Unk101();
		virtual void Unk102();
		virtual void Unk103();
		virtual void Unk104();
		virtual void Unk105();
		virtual void Unk106();
		virtual void Unk107();
		virtual void Unk108();
		virtual void Unk109();
		virtual void Unk10A();
		virtual void Unk10B();
		virtual void Unk10C();
		virtual void Unk10D();
		virtual void Unk10E();
		virtual void Unk10F();
		virtual void Unk110();
		virtual void Unk111();
		virtual void Unk112();
		virtual void Unk113();
		virtual void Unk114();
		virtual void Unk115();
		virtual void Unk116();
		virtual void Unk117();
		virtual void Unk118();
		virtual void Unk119();
		virtual void Unk11A();
		virtual void Unk11B();
		virtual void Unk11C();
		virtual void Unk11D();
		virtual void Unk11E();
		virtual void Unk11F();
		virtual void Unk120();
		virtual void Unk121();
		virtual void Unk122();
		virtual void Unk123();
		virtual void Unk124();
		virtual void Unk125();
		virtual void Unk126();
		virtual void Unk127();
		virtual void Unk128();
		virtual void Unk129();
		virtual void Unk12A();
		virtual void Unk12B();
		virtual void Unk12C();
		virtual void Unk12D();
		virtual void Unk12E();
		virtual void Unk12F();
		virtual void Unk130();
		virtual void Unk131();
		virtual void Unk132();
		virtual void Unk133();
		virtual void Unk134();
		virtual void Unk135();
		virtual void Unk136();
		virtual void Unk137();
		virtual void Unk138();
		virtual void Unk139();
		virtual void Unk13A();
		virtual void Unk13B();
		virtual void Unk13C();
		virtual void Unk13D();
		virtual void Unk13E();
		virtual void Unk13F();
		virtual void Unk140();
		virtual void Unk141();
		virtual void Unk142();
		virtual void Unk143();
		virtual void Unk144();
		virtual void Unk145();
		virtual void Unk146();
		virtual void Unk147();
		virtual void Unk148();
		virtual void Unk149();
		virtual void Unk14A();
		virtual void Unk14B();
		virtual void Unk14C();
		virtual void Unk14D();
		virtual void Unk14E();
		virtual void Unk14F();
		virtual void Unk150();
		virtual void Unk151();
		virtual void Unk152();
		virtual void Unk153();
		virtual void Unk154();
		virtual void Unk155();
		virtual void Unk156();
		virtual void Unk157();
		virtual void Unk158();
		virtual void Unk159();
		virtual void Unk15A();
		virtual void Unk15B();
		virtual void Unk15C();
		virtual void Unk15D();
		virtual void Unk15E();
		virtual void Unk15F();
		virtual void Unk160();
		virtual void Unk161();
		virtual void Unk162();
		virtual void Unk163();
		virtual void Unk164();
		virtual void Unk165();
		virtual void Unk166();
		virtual void Unk167();
		virtual void Unk168();
		virtual void Unk169();
		virtual void Unk16A();
		virtual void Unk16B();
		virtual void Unk16C();
		virtual void Unk16D();
		virtual void Unk16E();
		virtual void Unk16F();
		virtual void Unk170();
		virtual void Unk171();
		virtual void Unk172();
		virtual void Unk173();
		virtual void Unk174();
		virtual void Unk175();
		virtual void Unk176();
		virtual void RT_SetViewport(int x, int y, int width, int height, int id = -1);
		virtual void Unk178();
		virtual void Unk179();
		virtual void Unk17A();
		virtual void Unk17B();
		virtual void Unk17C();
		virtual void Unk17D();
		virtual void Unk17E();
		virtual void EF_ClearBuffers(uint32_t nFlags, const ColorF* Colors, float fDepth = 1.f, uint8_t nStencil = 0);
		//...

		// members
		uint64_t unk00[0x4955];
		ID3D11RenderTargetView* m_pBackBuffer; // 0x24AB0
		uint64_t unk24AB8;
		uint64_t unk24AC0;
		ID3D11DepthStencilView* m_pNativeZSurface; // 0x24AC8
		uint64_t unk24AD0[0x8A9];
		volatile int32_t m_nAsyncDeviceState; //0x29018
		uint64_t         unk29020[0x11E];
		ID3D11DeviceContext1* m_pDeviceContext; //0x29910
	};
	static_assert(offsetof(CD3D9Renderer, m_pBackBuffer) == 0x24AB0);
	static_assert(offsetof(CD3D9Renderer, m_pNativeZSurface) == 0x24AC8);
	static_assert(offsetof(CD3D9Renderer, m_nAsyncDeviceState) == 0x29018);
	static_assert(offsetof(CD3D9Renderer, m_pDeviceContext) == 0x29910);

	struct D3DSurface;
	struct SDepthTexture;

	struct DeviceInfo
	{
		IDXGIFactory1*        m_pFactory;
		IDXGIAdapter*         m_pAdapter;
		IDXGIOutput*          m_pOutput;
		ID3D11Device*         m_pDevice;
		ID3D11DeviceContext1* m_pContext;
		IDXGISwapChain*       m_pSwapChain;
		unsigned int          m_pCurrentBackBufferRTVIndex;
		DXGI_ADAPTER_DESC1    m_adapterDesc;
		DXGI_SWAP_CHAIN_DESC  m_swapChainDesc;
		DXGI_RATIONAL         m_refreshRate;
		DXGI_RATIONAL         m_desktopRefreshRate;
		D3D_DRIVER_TYPE       m_driverType;
		unsigned int          m_creationFlags;
		D3D_FEATURE_LEVEL     m_featureLevel;
		DXGI_FORMAT           m_autoDepthStencilFmt;
		unsigned int          m_outputIndex;
		unsigned int          m_syncInterval;
		unsigned int          m_presentFlags;
		bool                  m_activated;
		bool                  m_activatedMT;
	};

	struct Vec4
	{
		float x;
		float y;
		float z;
		float w;
	};

	class CCryNameR
	{
	public:
		CCryNameR()
		{
			m_str = nullptr;
		}

		CCryNameR(const char* s);

	private:
		const char* m_str;
	};

	enum EHWShaderClass
	{
		eHWSC_Vertex = 0,
		eHWSC_Pixel = 1,
		eHWSC_Geometry = 2,
		eHWSC_Compute = 3,
		eHWSC_Domain = 4,
		eHWSC_Hull = 5,
		eHWSC_Num = 6
	};

	enum ECGParam
	{
		ECGP_Unknown = 0,

		// ...

		ECGP_LumaUILuminance = 0xFF
	};

	struct SParamDB
	{
		const char* szName = nullptr;
		const char* szAliasName = nullptr;
		ECGParam    eParamType = ECGParam::ECGP_Unknown;
		uint32_t    nFlags = 0;
		void*       ParserFunc = nullptr;
	};

	struct SCGBind
	{
		CCryNameR m_Name;
		uint32_t  m_Flags;
		int16_t   m_dwBind;
		int16_t   m_dwCBufSlot;
		int32_t   m_nParameters;
	};

	struct SCGParam : SCGBind
	{
		ECGParam  m_eCGParamType;
		void*     m_pData;
		uintptr_t m_nID;
	};

	struct HDRSetupParams
	{
		Vec4 HDRFilmCurve;
		Vec4 HDRBloomColor;
		Vec4 HDRColorBalance;  // Vec4(fHDRBloomAmount * 0.3f, fHDRBloomAmount * 0.3f, fHDRBloomAmount * 0.3f, fGrainAmount); before being sent to the shader, x, y, z are multiplied by 0.125f
		Vec4 HDREyeAdaptation;
		Vec4 HDREyeAdaptationLegacy;
	};
}
