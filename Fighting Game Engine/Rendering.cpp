#include "Rendering.h"
#include "Screen.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"
#include "FrameBuffer.h"
#include "Time.h"
#include "Material.h"
#include "Resource.h"
#include "GLStateTrack.h"
#include "PostEffect.h"
#define VE_AUX_BUFFER_AMOUNT 3
//Auxiliary buffers are framebuffers used for multipass post processing effects.
#define VE_WORLD_SCALE 3
//World scale controls how many world units map correspond to one pixel. Object positions are integers for consistency.
namespace Rendering{
	FrameBuffer* mainBuffer;
	std::vector<FrameBuffer*> auxBuffers;
	//I don't quite remember why i added support for multiple cameras. It's probably never going to be used.
	std::vector<Camera> cameras;
	glm::mat4 orthoMat;
	glm::mat4 screenMat;
	glm::vec4 tintColor;
}
void Rendering::Init(){
	mainBuffer = new FrameBuffer(Screen::size, 3, true, GL_RGBA, glm::vec4(0, 0, 0, 1));
	auxBuffers.reserve(VE_AUX_BUFFER_AMOUNT);
	for (unsigned int i = 0; i < VE_AUX_BUFFER_AMOUNT; ++i){
		auxBuffers.push_back(new FrameBuffer(Screen::size, 3,false,GL_RGBA16F));
	}
	orthoMat = glm::ortho(-960.0*VE_WORLD_SCALE, 960.0*VE_WORLD_SCALE, -540.0*VE_WORLD_SCALE, 540.0*VE_WORLD_SCALE, -1.0, 2.0);
	screenMat = glm::ortho(0.0, 1920.0, 1080.0, 0.0,-1.0,2.0);
	cameras.push_back(Camera(glm::vec2(0,0),&orthoMat));
}

void Rendering::Update(){
	tintColor = glm::vec4(1, 1, 1, 1);
	if (Screen::isDirty){
		mainBuffer->resolution = Screen::size;
		mainBuffer->Update();
		for (unsigned int i = 0; i < VE_AUX_BUFFER_AMOUNT; ++i){
			auxBuffers[i]->resolution = Screen::size;
			auxBuffers[i]->Update();
		}
	}
}

void Rendering::Cleanup(){
	delete mainBuffer;
	for (unsigned int i = 0; i < VE_AUX_BUFFER_AMOUNT; ++i){
		delete auxBuffers[i];
	}
}
//Default engine uniforms for passing time and screen parameters.
void Rendering::BindEngineUniforms(Shader* shader){
	glUniform4f(shader->UniformLocation("ve_time"), (GLfloat)Time::time, (GLfloat)Time::deltaTime, (GLfloat)1.0 / (GLfloat)Time::deltaTime, (GLfloat)Time::frameCount);
	glUniform4f(shader->UniformLocation("ve_screen"), (GLfloat)Screen::size.x, (GLfloat)Screen::size.y, Screen::invSize.x, Screen::invSize.y);
}
//The following functions mostly just convert calls with different kinds of data into a standard DrawScreenMesh call.
void Rendering::DrawScreenMesh(glm::vec4 rect, Mesh* mesh, std::vector<Texture*> textures, Material* mat, glm::vec4 params){
	std::vector<std::pair<GLuint, glm::vec4>> txtr;
	for (unsigned int i = 0; i < textures.size(); ++i)
		txtr.push_back(std::pair<GLuint, glm::vec4>(textures[i]->id, textures[i]->size));
	DrawScreenMesh(rect, mesh, txtr, mat, params);
}

void Rendering::DrawScreenMesh(glm::vec4 rect, Mesh* mesh, FrameBuffer* fb, Material* mat, glm::vec4 params){
	std::vector<std::pair<GLuint, glm::vec4>> textures;
	for (unsigned int i = 0; i < fb->textures.size(); ++i)
		textures.push_back(std::pair<GLuint, glm::vec4>(fb->textures[i],glm::vec4(fb->resolution,fb->invResolution)));
	DrawScreenMesh(rect, mesh, textures, mat, params);
}

void Rendering::DrawScreenMesh(glm::vec4 rect, Mesh* mesh, Material* mat){
	DrawScreenMesh(rect, mesh, std::vector<Texture*>(), mat);

}
//Post effects are a good enough place to explain the entire rendering process. It doesn't change much for the rest of the functions.
void Rendering::DrawPostEffect(PostEffect* pf){
	if (pf == nullptr)
		return;
	Mesh* cMesh = Resource::GetMesh("Meshes/Base/screenQuad.vm");
	GLState::BindVertexArray(cMesh->vao);
	tintColor = glm::vec4(1, 1, 1, 1);
	for (unsigned int pi = 0; pi < pf->elementChain.size(); ++pi){
		//glFinish();//Prevents IO on buffers that haven't had their effects finish rendering. Omitted for testing.
		if (pf->elementChain[pi].second == nullptr){
			GLState::BindFramebuffer(Rendering::mainBuffer->id);
			DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), nullptr, Rendering::auxBuffers[pf->elementChain[pi].first], Resource::GetMaterial("Materials/Base/Screen_FB.vmat"));
			continue;
		}
		if (pf->elementChain[pi].first == -1)
			GLState::BindFramebuffer(Rendering::mainBuffer->id);
		else 
			GLState::BindFramebuffer(Rendering::auxBuffers[pf->elementChain[pi].first]->id);

		Material* cMat = (pf->elementChain[pi].second);
		if (cMat == nullptr){
			std::cout << "Skipping PostEffect stage " << pi << std::endl;
			continue;
		}
		GLState::UseProgram(cMat->shader->id);
		int textureIndex = 0;
		//Bind uniforms specified in the material.
		for (auto iter = cMat->uniformFloats.begin(); iter != cMat->uniformFloats.end(); ++iter){
			glUniform1f(cMat->shader->UniformLocation(iter->first), iter->second);
		}
		for (auto iter = cMat->uniformVectors.begin(); iter != cMat->uniformVectors.end(); ++iter){
			glUniform4f(cMat->shader->UniformLocation(iter->first), iter->second.x, iter->second.y, iter->second.z, iter->second.w);
		}
		for (auto iter = cMat->uniformTextures.begin(); iter != cMat->uniformTextures.end(); ++iter){
			if (cMat->shader->UniformLocation(iter->first) > -1){//Only bind a texture and its other variables if it's going to be used in the shader
				GLState::BindTexture((iter->second.ref)->id, textureIndex);
				glUniform1i(cMat->shader->UniformLocation(iter->first), textureIndex);
				glUniform4f(cMat->shader->UniformLocation(iter->first + ("_params")), iter->second.params.x, iter->second.params.y, iter->second.params.z, iter->second.params.w);
				glUniform4f(cMat->shader->UniformLocation(iter->first + ("_size")), iter->second.ref->size.x, iter->second.ref->size.y, iter->second.ref->size.z, iter->second.ref->size.w);
				//Params are used for cropping the texture, size passes information about the texture itself.
				++textureIndex;
			}
		}
		//Bind the texture components of every buffer, so they can be accessible from the shader.
		BindBufferUniforms(cMat->shader, &textureIndex);
		BindEngineUniforms(cMat->shader);

		//Generate an MVP matrix for fullscreen projection

		glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3(0, 1080, 0));
		modelMat = glm::scale(modelMat, glm::vec3(1920, -1080, 1));

		glm::mat4 mvpmat = Rendering::screenMat*modelMat;

		glUniformMatrix4fv(cMat->shader->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(modelMat));
		glUniformMatrix4fv(cMat->shader->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(Rendering::screenMat));
		glUniformMatrix4fv(cMat->shader->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(mvpmat));

		//Apply material properties like ZWrite, ZTest, Blending, etc.
		if (cMat != nullptr)
			cMat->ApplyProperties();
		glDrawElements(GL_TRIANGLES, cMesh->elementAmount, GL_UNSIGNED_INT, 0);
	}
}

inline void Rendering::BindBufferUniforms(Shader* shad, int* index){
	//This entire structure optimizes the amount of bound textures. Uniforms return -1 if they're not in, or are unused by the shader.
	//Only increase the internal bound texture count if the uniform isn't -1.
	for (unsigned int i = 0; i<Rendering::mainBuffer->textures.size(); ++i){
		if (shad->UniformLocation("mainBuf_tex" + std::to_string(i)) > -1){
			GLState::BindTexture(Rendering::mainBuffer->textures[i], (*index));
			glUniform1i(shad->UniformLocation("mainBuf_tex" + std::to_string(i)), (*index));
			(*index) += 1;
		}
	}

	for (unsigned int buf = 0; buf < Rendering::auxBuffers.size(); ++buf){
		for (unsigned int i = 0; i<Rendering::auxBuffers[buf]->textures.size(); ++i){
			if (shad->UniformLocation("auxBuf" + std::to_string(buf) + "_tex" + std::to_string(i)) > -1){
				GLState::BindTexture(Rendering::auxBuffers[buf]->textures[i], (*index));
				glUniform1i(shad->UniformLocation("auxBuf" + std::to_string(buf) + "_tex" + std::to_string(i)), (*index));
				(*index) += 1;
			}
		}
	}
}

void Rendering::DrawScreenMesh(glm::vec4 rect, Mesh* mesh, std::vector<std::pair<GLuint,glm::vec4>> textures, Material* mat, glm::vec4 params){
	Shader* cShad;
	if (mat == nullptr)
		cShad = Resource::GetShader("Shaders/Base/Screen");
	else
		cShad = mat->shader;
	Mesh* cMesh;
	if (mesh == nullptr)
		cMesh = Resource::GetMesh("Meshes/Base/screenQuad.vm");
	else
		cMesh = mesh;

	GLState::BindVertexArray(cMesh->vao);
	GLState::UseProgram(cShad->id);
	int textureIndex = 0;
	if (mat != nullptr){
		for (auto iter = mat->uniformFloats.begin(); iter != mat->uniformFloats.end(); ++iter){
			
			glUniform1f(mat->shader->UniformLocation(iter->first), iter->second);
		}
		for (auto iter = mat->uniformVectors.begin(); iter != mat->uniformVectors.end(); ++iter){
			glUniform4f(mat->shader->UniformLocation(iter->first), iter->second.x, iter->second.y, iter->second.z, iter->second.w);
		}
		
		//Send textures in the material
		for (auto iter = mat->uniformTextures.begin(); iter != mat->uniformTextures.end(); ++iter){
			GLState::BindTexture((iter->second.ref)->id, textureIndex);
			glUniform1i(mat->shader->UniformLocation(iter->first), textureIndex);
			glUniform4f(mat->shader->UniformLocation(iter->first + ("_params")), iter->second.params.x, iter->second.params.y, iter->second.params.z, iter->second.params.w);
			glUniform4f(mat->shader->UniformLocation(iter->first + ("_size")), iter->second.ref->size.x, iter->second.ref->size.y, iter->second.ref->size.z, iter->second.ref->size.w);
			++textureIndex;
		}
	}
	//Then send the textures specified, potentially replacing texN textures in the material
	int dfInd=0;
	for (auto iter = textures.begin(); iter != textures.end(); ++iter){
		if (cShad->UniformLocation("tex" + std::to_string(dfInd)) > -1){
			GLState::BindTexture(iter->first, textureIndex);
			glUniform1i(cShad->UniformLocation("tex" + std::to_string(dfInd)), textureIndex);
			glUniform4f(cShad->UniformLocation("tex" + std::to_string(dfInd) + ("_params")), params.x, params.y, params.z, params.w);
			glUniform4f(cShad->UniformLocation("tex" + std::to_string(dfInd) + ("_size")), iter->second.x, iter->second.y, iter->second.z, iter->second.w);
			++textureIndex;
			++dfInd;
		}
	}

	glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3(rect.x, rect.y + rect.w, 0));
	modelMat = glm::scale(modelMat, glm::vec3(rect.z, -rect.w, 1));

	glm::mat4 mvpmat = Rendering::screenMat*modelMat;


	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(modelMat));
	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(Rendering::screenMat));
	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(mvpmat));
	BindEngineUniforms(cShad);
	
	glUniform4f(cShad->UniformLocation("ve_tintColor"), tintColor.x, tintColor.y, tintColor.z, tintColor.w);
	if (mat != nullptr)
		mat->ApplyProperties();
	else
		Resource::GetMaterial("Materials/Base/Screen.vmat")->ApplyProperties();

	glDrawElements(GL_TRIANGLES, cMesh->elementAmount, GL_UNSIGNED_INT, 0);
}

void Rendering::DrawMesh(Transform* transform, Mesh* mesh, Material* mat, Camera* camera){
	if (mesh==nullptr||!mesh->valid()){
		std::cout << "Attempting to draw invalid mesh: " << mesh->name << std::endl;
		return;
	}
	Camera* cCam = (camera == nullptr ? &cameras[0] : camera);
	GLState::BindVertexArray(mesh->vao);
		
	GLState::UseProgram(mat->shader->id);


	for (auto iter = mat->uniformFloats.begin(); iter != mat->uniformFloats.end(); ++iter){
		glUniform1f(mat->shader->UniformLocation(iter->first), iter->second);
	}
	for (auto iter = mat->uniformVectors.begin(); iter != mat->uniformVectors.end(); ++iter){
		glUniform4f(mat->shader->UniformLocation(iter->first), iter->second.x, iter->second.y, iter->second.z, iter->second.w);
	}
	int textureIndex = 0;
	for (auto iter = mat->uniformTextures.begin(); iter != mat->uniformTextures.end(); ++iter){
		GLState::BindTexture((iter->second.ref)->id, textureIndex);
		glUniform1i(mat->shader->UniformLocation(iter->first), textureIndex);
		glUniform4f(mat->shader->UniformLocation(iter->first + ("_params")), iter->second.params.x, iter->second.params.y, iter->second.params.z, iter->second.params.w);
		glUniform4f(mat->shader->UniformLocation(iter->first + ("_size")), iter->second.ref->size.x, iter->second.ref->size.y, iter->second.ref->size.z, iter->second.ref->size.w);
		++textureIndex;
	}

	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(transform->ModelMatrix()));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_view"), 1, false, glm::value_ptr(cCam->ViewMatrix()));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(*(cCam->projectionMatrix)));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(*(cCam->projectionMatrix)*cCam->ViewMatrix(transform->depth)*transform->ModelMatrix()));


	BindEngineUniforms(mat->shader);
	glUniform1f(mat->shader->UniformLocation("ve_depth"), transform->depth);

	mat->ApplyProperties();

	glDrawElements(GL_TRIANGLES, mesh->elementAmount,GL_UNSIGNED_INT,0);
}

void Rendering::DrawScreenText(glm::vec4 rect,GLuint size, std::string text, Font* font,TextAlignment alignment){
	if (text.size() == 0)
		return;
	float scale = (float)size / (float)TEXT_SIZE;
	Font* currentFont=(font==nullptr?Resource::GetFont("Fonts/Amble.ttf"):font);
	

	glm::ivec2 cursor = glm::ivec2(rect.x, rect.y);
	int currentLine = 0;
	std::vector<float> lineIndents;
	
	//Pre-parse text to figure out how much to indent the lines.
	if (alignment != TextAlignment::Left){
		lineIndents.reserve(3);
		for (auto ch = text.begin(); ch != text.end(); ){
			if (*ch == ' '){
				cursor.x += (int)glm::round(size*0.4);
				++ch;
				continue;
			}
			FontCharacter* fc = currentFont->Character(*ch);
			if (fc == nullptr){
				++ch;
				continue;
			}
			
			if (*ch == '\n' || (cursor.x + fc->size.x *scale > rect.x + rect.z)||(std::next(ch)==text.end())){
				++currentLine;
				lineIndents.push_back(((rect.x + rect.z) - (cursor.x + fc->size.x *scale))*((alignment == TextAlignment::Center) ? 0.5f : 1.0f));
				if (cursor.y + glm::round(currentFont->height*2.4) > rect.y + rect.w)
					break;
				cursor.y += (int)glm::round(currentFont->height*1.4);
				cursor.x = (int)glm::round(rect.x);
			}
			else{
				cursor.x += (int)glm::round((fc->advance)*scale);
				++ch;
			}
		}
		currentLine = 0;
		cursor = glm::ivec2(rect.x, rect.y);
		if (alignment != TextAlignment::Left&&lineIndents.size()>0)
			cursor.x += (int)glm::round(lineIndents[0]);
		
	}
	InitTextDrawing();
	//Loop stays more or less the same, except now we draw the characters
	for (auto ch = text.begin(); ch != text.end();){
		if (*ch == ' '){
			cursor.x += (int)glm::round(size*0.4);
			++ch;
			continue;
		}
		FontCharacter* fc = currentFont->Character(*ch);
		if (fc == nullptr){
			++ch;
			continue;
		}

		if (*ch == '\n' || (cursor.x + fc->size.x *scale > rect.x + rect.z)){
			++currentLine;
			if (cursor.y + glm::round(currentFont->height*2.4) > rect.y + rect.w)
				return;
			cursor.y += (int)glm::round(currentFont->height*1.4);
			cursor.x = (int)glm::round(rect.x);
			
			if (alignment != TextAlignment::Left)
				cursor.x += (int)glm::round(lineIndents[currentLine]);
		}
		else{
			glm::vec4 cr = glm::vec4(cursor.x + fc->bearing.x*scale, cursor.y + (currentFont->topBearing - fc->bearing.y)*scale, fc->size.x*scale, fc->size.y*scale);
			DrawTextCharacter(cr, fc->textureParams,currentFont->atlases.at(fc->atlasIndex));
			cursor.x += (int)glm::round((fc->advance)*scale);
			++ch;
		}
	}
}
void Rendering::InitTextDrawing(){
	//Initialize states for text drawing to minimize unnecessary state changes.
	Shader* cShad = Resource::GetShader("Shaders/Base/Screen_Text");
	Mesh* cMesh = Resource::GetMesh("Meshes/Base/screenQuad.vm");

	GLState::BindVertexArray(cMesh->vao);
	GLState::UseProgram(cShad->id);

	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(Rendering::screenMat));
	
	Resource::GetMaterial("Materials/Base/Screen.vmat")->ApplyProperties();
	
}
void Rendering::DrawTextCharacter(glm::vec4 rect,glm::vec4 params, Texture* tex){
	//Draw individual text characters while assuming things set by InitTextDrawing are still bound.
	//So much more optimization coule be put into this.
	Shader* cShad = Resource::GetShader("Shaders/Base/Screen_Text");
	Mesh* cMesh = Resource::GetMesh("Meshes/Base/screenQuad.vm");

	GLState::BindTexture(tex->id,0);
	glUniform1i(cShad->UniformLocation("tex0"), 0);
	glUniform4f(cShad->UniformLocation("tex0_params"), params.x,params.y,params.z,params.w);

	glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3(rect.x, rect.y + rect.w, 0));
	modelMat = glm::scale(modelMat, glm::vec3(rect.z, -rect.w, 1));

	glm::mat4 mvpmat = Rendering::screenMat*modelMat;


	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(modelMat));
	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(mvpmat));
	glDrawElements(GL_TRIANGLES, cMesh->elementAmount, GL_UNSIGNED_INT, 0);
}