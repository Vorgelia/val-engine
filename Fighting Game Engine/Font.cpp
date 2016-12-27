#include "Font.h"
#include "Texture.h"
#include "DebugLog.h"
FT_Library Font::library;
bool Font::init=false;

FontCharacter::FontCharacter(GLuint atlasIndex,glm::vec4 textureParams, glm::ivec2 size, glm::ivec2 bearing, GLuint advance){
	this->textureParams = textureParams;
	this->size = size;
	this->bearing = bearing;
	this->advance = advance;
	this->atlasIndex = atlasIndex;
}

Font::Font(FS::path p)
{
	//Small experiment with initialization. Class instances initialize the underlying framework if it isn't initialized.
	//Cleaner code, i guess? Last time i put initialization and font loading in GUI::Init() and it was h-o-r-r-i-b-l-e.
	//Last time was horrible in general.
	if (!init){
		init = FT_Init_FreeType(&library)==0;
		if (!init){
			DebugLog::Push("Unable to init Freetype",2);
			return;
		}
	}
	height = 0;
	name = p.string();
	//Font loading
	if (FT_New_Face(library, name.c_str(), 0, &face)){
		DebugLog::Push("Unable to load font: "+name, 2);
		return;
	}
	FT_Set_Pixel_Sizes(face, 0, TEXT_SIZE);//Force the Y size to TEXT_SIZE, X size automatic.

	//Load all the characters into textures, one by one.
	int csk = 0;
	std::vector<unsigned char> pixels(FONT_ATLAS_SIZE*FONT_ATLAS_SIZE);
	glm::ivec2 cursor=glm::ivec2(0,0);
	for (GLubyte c = 0; c < 255; ++c){
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)){//Load character texture to face->glyph->bitmap
			DebugLog::Push("Unable to load character: " + c, 2);
			continue;
		}
		
		if (cursor.x + face->glyph->bitmap.width>=FONT_ATLAS_SIZE){
			cursor.x = 0;
			cursor.y += TEXT_SIZE;
			if (cursor.y >= FONT_ATLAS_SIZE){
				cursor = glm::ivec2(0, 0);
				//SOIL_save_image("Atlas.bmp", SOIL_SAVE_TYPE_BMP, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE,1,&pixels[0]);
				atlases.push_back(new Texture("Atlas" + std::to_string(atlases.size() + 1), pixels, glm::ivec2(FONT_ATLAS_SIZE, FONT_ATLAS_SIZE), GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE,true));
				pixels.clear();
				pixels.resize(FONT_ATLAS_SIZE*FONT_ATLAS_SIZE);
			}
		}
			
		for (int j = face->glyph->bitmap.rows-1; j >=0; --j){
			for (unsigned int i = 0; i < face->glyph->bitmap.width; ++i){//Invert the pixels vertically.
				pixels[((cursor.y + j)*(FONT_ATLAS_SIZE)) + (cursor.x + i)] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + i];
			}
		}
		
		glm::vec4 params = glm::vec4((float)cursor.x / (float)FONT_ATLAS_SIZE, (float)cursor.y / (float)FONT_ATLAS_SIZE, (float)face->glyph->bitmap.width / (float)FONT_ATLAS_SIZE, (float)face->glyph->bitmap.rows / (float)FONT_ATLAS_SIZE);
		characters.insert(std::pair<GLubyte, FontCharacter*>(c, new FontCharacter(atlases.size(),params, glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x>>6)));
		cursor.x += face->glyph->bitmap.width+FONT_ATLAS_PADDING;
		
		if (c == 'H'){//Hack fraud method of finding the top bearing of the entire font by finding the top bearing of one of the tallest characters.
			topBearing = face->glyph->bitmap_top;
			this->height = glm::max(this->height, face->glyph->bitmap.rows);
			//std::cout <<"H params: "<< params.x << "," << params.y << "," << params.z << "," << params.w << std::endl;
		}
	}
	atlases.push_back(new Texture("Atlas"+std::to_string(atlases.size()+1), pixels, glm::ivec2(FONT_ATLAS_SIZE,FONT_ATLAS_SIZE), GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE,true));
}
Font::~Font(){
	for (unsigned int i = 0; i<atlases.size(); ++i)
		delete atlases[i];
}
FontCharacter* Font::Character(GLubyte ch){
	if (characters.count(ch)>0){
		return characters.find(ch)->second;
	}
	return nullptr;
}