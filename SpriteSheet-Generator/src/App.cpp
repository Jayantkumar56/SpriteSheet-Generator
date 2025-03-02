


#include "Quirk.h"
#include "Core/Application/EntryPoint.h"
#include "Frame.h"
#include "Theme.h"

using namespace Quirk;

class MinecraftCloneApp : public Application {
public:
	MinecraftCloneApp() :
		Application(L"Sprite-Sheet Generator", RendererAPI::API::OpenGL)
	{
		LoadFonts();
		WindowSpecification tempSpec{ "Sprite-Sheet Generator", 1600, 900, 200, 50, true, false, true };
		AddFrame<SpriteGeneratorFrame>(tempSpec);
	}

private:
	void LoadFonts() {
		//FontManager::SetFontFileToFontWeight(FontWeight::Regular, std::string("assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf"));
		FontManager::SetFontFileToFontWeight( FontWeight::Regular,   "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf"   );
		FontManager::SetFontFileToFontWeight( FontWeight::Medium,    "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Medium.ttf"    );
		FontManager::SetFontFileToFontWeight( FontWeight::SemiBold,  "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-SemiBold.ttf"  );
		FontManager::SetFontFileToFontWeight( FontWeight::Bold,      "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Bold.ttf"      );
		FontManager::SetFontFileToFontWeight( FontWeight::ExtraBold, "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-ExtraBold.ttf" );
		FontManager::SetFontFileToFontWeight( FontWeight::Black,     "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Black.ttf"     );

		// SpriteGeneratorFrame related fonts 
		FontManager::LoadFont( FontWeight::Medium,   25 );
		FontManager::LoadFont( FontWeight::Regular,  22 );
		FontManager::LoadFont( FontWeight::Bold,     18 );
	}
};

namespace Quirk {

	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
		MinecraftCloneApp app;
		app.Run();
	}

}

