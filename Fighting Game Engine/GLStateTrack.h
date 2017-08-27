#pragma once
#include "CommonUtilIncludes.hpp"
namespace GLState
{
	extern GLuint activeTexture;
	extern GLuint boundShader;
	extern GLuint boundVAO;
	extern GLuint boundFramebuffer;
	extern std::vector<GLuint> *boundTextures;
	extern std::map<GLenum, bool> glFeatures;
	void Init();
	void Cleanup();

	bool Set(GLenum feature, bool enable);
	bool ActiveTexture(GLuint pos);
	bool UseProgram(GLuint id);
	bool BindVertexArray(GLuint id);
	bool BindTexture(GLuint id);
	bool BindFramebuffer(GLuint id);
	bool BindTexture(GLuint id, GLuint pos);
}


