#pragma once
#include "reshade/reshade.hpp"

#define CONFIG_ENTRY ""
#include "DKUtil/Config.hpp"

namespace Settings
{
    using namespace DKUtil::Alias;

	class Setting
	{
	public:
	    std::string name;
		std::string description;

		Setting(const std::string& a_name, const std::string& a_description) :
			name(a_name), description(a_description)
		{}

		virtual ~Setting() = default;
		virtual bool IsDefault() const = 0;
        
	};

	class IntSlider : public Setting
	{
	public:
		Integer     value;
		int32_t     defaultValue;
		int32_t     sliderMin;
		int32_t     sliderMax;
		std::string suffix = "";

		IntSlider(const std::string& a_name, const std::string& a_description, const std::string& a_key, const std::string& a_section, int32_t a_defaultValue, int32_t a_sliderMin, int32_t a_sliderMax, std::string_view a_suffix = "") :
			Setting{ a_name, a_description }, value{ a_key, a_section }, defaultValue(a_defaultValue), sliderMin(a_sliderMin), sliderMax(a_sliderMax), suffix(a_suffix) {}

		bool IsDefault() const override { return value.get_data() == defaultValue; }

		int32_t GetValue() const { return value.get_data(); }
		std::string GetSliderText() const;
	};

	class FloatSlider : public Setting
	{
	public:
	    Double value;
		float  defaultValue;
		float sliderMin;
		float sliderMax;
		std::string suffix;

		FloatSlider(const std::string& a_name, const std::string& a_description, const std::string& a_key, const std::string& a_section, float a_defaultValue, float a_sliderMin, float a_sliderMax, std::string a_suffix) :
			Setting{ a_name, a_description }, value{ a_key, a_section }, defaultValue(a_defaultValue), sliderMin(a_sliderMin), sliderMax(a_sliderMax), suffix(a_suffix) {}

		bool IsDefault() const override { return value.get_data() == defaultValue; }

		float GetValue() const { return value.get_data(); }
		std::string GetSliderText() const;
	};

    class Main : public DKUtil::model::Singleton<Main>
    {
    public:
		FloatSlider PeakLuminance{
			"Peak Luminance",
			"Sets the peak luminance in HDR."
				"\nThe value should match your display's peak luminance."
				"\n"
				"\nThis does not affect the game's average luminance.",
			"PeakLuminance", "HDR",
			1000.f,
			400.f,
			10000.f,
			"%.0f nits"
		};
		FloatSlider LuminanceMultiplier{
			"Luminance Multiplier",
			"idk",
			"LuminanceMultiplier", "HDR",
			1.f,
			0.5f,
			2.f,
			"%.2f"
		};
		FloatSlider UILuminance{
			"UI Luminance",
			"Sets the user-interface brightness in HDR modes."
				"\nThis setting represents the brightness of UI elements."
				"\n"
				"\nThe default value is 203.",
			"UILuminance", "HDR",
			203.f, // ITU reference default is 203
			80.f,
			500.f,
			"%.0f nits"
		};
		FloatSlider ExtendGamut{
			"HDR Saturation",
			"Perceptually increases the saturation to a max of DCI-P3 or BT.2020 gamut."
				"\n"
				"\nNeutral at 0\%."
				"\n"
				"\nUses darktable UCS as colour space to perform the increase.", //this is needed to follow the license requirements
			"ExtendGamut", "HDR",
			10.f,
			0.f,
			100.f,
			"%.0f%%"
		};
		IntSlider ExtendGamutTarget{
			"HDR Saturation Type",
			"0: DCI-P3 target\n"
			"1: BT.2020 target",
			"ExtendGamutTarget", "HDR",
			0,
			0,
			1
		};

		void RegisterReshadeOverlay();

        void Load() noexcept;
		void Save() noexcept;

		static void DrawReshadeSettings(reshade::api::effect_runtime*);

    private:
		TomlConfig config = COMPILE_PROXY("Luma-KCD.toml");

		bool bReshadeSettingsOverlayRegistered = false;

		void DrawReshadeTooltip(const char* a_desc);
		bool DrawReshadeIntSlider(IntSlider& a_slider);
		bool DrawReshadeFloatSlider(FloatSlider& a_slider);
		bool DrawReshadeResetButton(Setting& a_setting);
		void DrawReshadeSettings();
    };

	
}
