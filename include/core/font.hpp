#ifndef GWAIN_CORE_FONT_HPP
#define GWAIN_CORE_FONT_HPP

#include "texture.hpp"
#include "error.hpp"

#include "..\artist\checker.hpp"

#define STB_TRUETYPE_IMPLEMENTATION  
#define STBTT_RASTERIZER_VERSION 1
#include "..\stb\stb_truetype.h"

#include <map>
#include <fstream>

namespace gwain
{
    struct glyph
    {
        int width_ = 0;     // width in pixels of glyph
        int height_ = 0;    // height in pixels of glyph
        int x_ = 0;	        // origin x of glyph
        int y_ = 0;         // origin y of glyph
        int xOff_ = 0;		// start x of glyph
        int yOff_ = 0;		//start y of glyph
        int xAdvance_ = 0;  //xAdvance
    };

    class font
    {
        glyph default_character_;
    public:

        typedef std::map<wchar_t, glyph> character_map;

        font(int max_height, int max_width)
            : atlas_(std::make_shared<texture>("default_font_atlas", image(max_height / 2, max_width / 2, rgba32(255, 255, 255, 255), rgba32()))), max_height_(max_height), max_width_(max_width), baseline_(0)
        {
            default_character_.width_ = max_width_;
            default_character_.height_ = max_height_;
        }

        font(const std::shared_ptr<texture>& texture, const character_map& map, int max_height, int max_width, int baseline)
            : atlas_(texture), map_(map), max_height_(max_height), max_width_(max_width), baseline_(baseline)
        {
            default_character_.width_ = max_width_;
            default_character_.height_ = max_height_;
        }

        const std::shared_ptr<texture>& atlas() const { return atlas_; }
        const character_map& map() const { return map_; }
        glyph character(wchar_t c) const { auto itr = map_.find(c); return itr == map_.end() ? default_character_ : itr->second; }

        int max_height() const { return max_height_; }
        int max_width() const { return max_width_; }
        int baseline() const { return baseline_; }

    private:
        std::shared_ptr<texture> atlas_;
        character_map map_;
        int max_height_;
        int max_width_;
        int baseline_;
    };

#ifndef GWAIN_NOSTB
    static font ttf_font(const std::string& filepath, int ptSize)
    {
        font::character_map char_map;

        //16 pixel =12 ptSize  (factor=4/3)
        //96 Glyph for ASCII, i.e. arrange 10 x 10 in the Fontmap
        int numberOfGlyph = 96;
        int firstCodePoint = 32;
        float GlyphPixel = ptSize * static_cast<float>(4 / 3);
        float GlyphPerRow = std::ceilf(std::sqrt(static_cast<float>(numberOfGlyph)));

        std::vector<unsigned char> fontBitmap;
        std::vector<stbtt_packedchar>pcdata;
        unsigned int pixelWH = static_cast<unsigned int>(std::pow(2, std::ceilf((std::log(GlyphPerRow * GlyphPixel)) / logf(2))));

        std::ifstream fileIn(filepath, std::ios::binary | std::ios::ate);
        if (!fileIn)
            throw exception(error::fontReadFail, filepath);

        auto fend = fileIn.tellg();
        fileIn.seekg(0, std::ios::beg);
        auto size = std::size_t(fend - fileIn.tellg());
        std::vector<unsigned char> buffer(size);
        fileIn.read((char*)buffer.data(), buffer.size());
        stbtt_fontinfo f;
        stbtt_InitFont(&f, reinterpret_cast<unsigned char*>(buffer.data()), 0);

        unsigned int fontMapWidth = pixelWH, fontMapHeight = pixelWH;

        stbtt_pack_context pack;
        pcdata.resize(numberOfGlyph);
        fontBitmap.resize(fontMapWidth * fontMapHeight);
        stbtt_PackBegin(&pack, fontBitmap.data(), fontMapWidth, fontMapHeight, 0, 2, nullptr);
        stbtt_PackFontRange(&pack, reinterpret_cast<unsigned char*>(buffer.data()), 0, static_cast<float>(ptSize), firstCodePoint, numberOfGlyph - 1, pcdata.data());
        stbtt_PackSetOversampling(&pack, 3, 2);
        stbtt_PackEnd(&pack);
        float scale = stbtt_ScaleForPixelHeight(&f, static_cast<float>(ptSize));
        int baseLine = 0;
        int maxHeight = 0;
        int maxWidth = 0;
        for (unsigned int i = 0; i < pcdata.size(); i++)
        {
            const stbtt_packedchar* b = &pcdata[i];
            wchar_t character = i + firstCodePoint;
            glyph g;

            int advance, lsb = 0;

            stbtt_GetCodepointHMetrics(&f, character, &advance, &lsb);

            g.xAdvance_ = static_cast<int>(std::roundf(static_cast<float>(advance) * scale));
            g.xOff_ = static_cast<int>(b->xoff);
            g.yOff_ = static_cast<int>(-b->yoff2) - 1;
            g.width_ = (b->x1 - b->x0);

            //g.xAdvance_ = b->xoff2;
            g.height_ = (b->y1 - b->y0) + 2;
            g.x_ = b->x0;
            g.y_ = (fontMapHeight - b->y1);
            baseLine = baseLine < g.yOff_ ? baseLine : g.yOff_;
            maxHeight = maxHeight > g.height_ ? maxHeight : g.height_;
            maxWidth = maxWidth > g.width_ ? maxWidth : g.width_;
            char_map.try_emplace(character, g);
        }
        baseLine = -baseLine;

        //create an image to hold all the characters...
        image img(fontMapWidth, fontMapHeight);
        for (i p = 0; p < img.pixel_count(); ++p)
            img[p] = rgba32(fontBitmap[p], fontBitmap[p], fontBitmap[p], fontBitmap[p]);
        
        //transform::vertical(img);


        return font(std::make_shared<texture>(filepath + "_" + std::to_string(ptSize), img), char_map, maxHeight, maxWidth, baseLine);
    }
#endif // GWAIN_NOSTB
}

#endif // GWAIN_CORE_FONT_HPP
