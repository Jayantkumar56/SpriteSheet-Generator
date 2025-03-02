

#pragma once

#include "Quirk.h"

enum class ThemeName {
	DarkTheme = 0, LightTheme = 1
};

enum  class ColorName {
	PopupBorder, DarkText, DropdownButton
};

class Theme {
public:
	Theme() = delete;
	~Theme() = delete;

	static inline void SetTheme(ThemeName theme) {
		switch (theme) {
			case ThemeName::DarkTheme:		SetDarkTheme();			return;
			case ThemeName::LightTheme:		SetLightTheme();		return;
		}
	}

	static ImVec4 GetColor(ColorName color);

private:
	static void SetDarkTheme();
	static void SetLightTheme();
};

