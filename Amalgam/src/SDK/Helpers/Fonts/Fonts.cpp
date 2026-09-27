#include "Fonts.h"

#include "../../Definitions/Interfaces/IMatSystemSurface.h"
#include <ranges>

void CFonts::Reload(float flDPI, bool bOutline)
{
	//	Tahoma, weight per font, FONTFLAG_OUTLINE (512) only -> no anti-aliasing
	//	the 1px outline is baked into the glyphs, so StringOutlined skips its own ring for these fonts
	//	"Cheap text" additionally swaps in the font's own drop shadow
	int iFlags = FONTFLAG_OUTLINE;
	if (bOutline)
		iFlags |= FONTFLAG_DROPSHADOW;

	m_mFonts[FONT_ESP] = { "Tahoma", int(12 * flDPI), iFlags, 0 };
	m_mFonts[FONT_INDICATORS] = { "Tahoma", int(13 * flDPI), iFlags, -1 };

	for (auto& fFont : m_mFonts | std::views::values)
	{
		if (fFont.m_dwFont = I::MatSystemSurface->CreateFont())
			I::MatSystemSurface->SetFontGlyphSet(fFont.m_dwFont, fFont.m_szName, fFont.m_nTall, fFont.m_nWeight, 0, 0, fFont.m_nFlags);
	}
}

const Font_t& CFonts::GetFont(EFonts eFont)
{
	return m_mFonts[eFont];
}