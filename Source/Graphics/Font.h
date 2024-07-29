#pragma once

#include <string>
#include <vector> 

#include "Texture.h"

#ifdef _MSC_VER
#pragma warning(disable: 4505) // disable warning 4505 from msdf
#pragma warning(disable: 4458) 
#endif
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#ifdef _MSC_VER
#pragma warning(default: 4505) // reallow 4505
#pragma warning(default: 4458) 
#endif

/* Font - stores a font atlas, and rendering information
*  Rendering info includes:
*    (1) Kerning pairs + x advance
*    (2) UV's of the glyph
*	 (3) Vertex Positions of the glyph
*/
class Font {
	std::vector<msdf_atlas::GlyphGeometry> glyphs;
	msdf_atlas::FontGeometry fontGeometry;
	Texture atlas;
	
	Texture GenerateAtlas(const std::string& filename);
public:
	Font(const std::string& filename);

	void BindAtlas() { atlas.Bind(); }

	const msdf_atlas::FontGeometry& GetFontGeometry() { return fontGeometry; };
	const std::vector<msdf_atlas::GlyphGeometry>& GetGlyphs() { return glyphs; };

	int GetAtlasWidth() { return atlas.GetSize().x; };
	int GetAtlasHeight() { return atlas.GetSize().y; };
protected:
	friend bool operator<(const Font& lhs, const Font& rhs);
	friend bool operator==(const Font& lhs, const Font& rhs);
	friend bool operator!=(const Font& lhs, const Font& rhs);
};