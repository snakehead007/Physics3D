#pragma once

typedef Vec4f Color;
typedef Vec3f Color3;

namespace Graphics {

// Colors
namespace COLOR {
	extern Color DISABLED;
	extern Color ACCENT;
	extern Color BACK;
	extern Color ALPHA;

	extern Color RGB_R;
	extern Color RGB_G;
	extern Color RGB_B;
	extern Color A;

	extern Color NAVY;
	extern Color BLUE;
	extern Color AQUA;
	extern Color TEAL;
	extern Color OLIVE;
	extern Color GREEN;
	extern Color LIME;
	extern Color YELLOW;
	extern Color ORANGE;
	extern Color RED;
	extern Color MAROON;
	extern Color FUCHSIA;
	extern Color PURPLE;
	extern Color BLACK;
	extern Color GRAY;
	extern Color SILVER;
	extern Color WHITE;

	// format: 0xRRGGBBAA
	Color get(int hex, bool alpha);
	Color get(int hex);

	Color3 hsvToRgb(Color3 hsv);
	Color3 rgbToHsv(Color3 rgb);
	Color hsvaToRgba(Color hsva);
	Color rgbaToHsva(Color rgba);

	Color blend(const Color& color1, const Color& color2, float blend);
	Color3 blend(const Color3& color1, const Color3& color2, float blend);
};

};