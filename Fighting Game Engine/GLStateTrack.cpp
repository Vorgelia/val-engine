#include "GLStateTrack.h"
#include "Profiler.h"

//Keeps track of current OpenGL states and prevents duplicate changes for performance improvements.
namespace GLState
{
	GLuint activeTexture;
	GLuint boundShader;
	GLuint boundVAO;
	GLuint boundFramebuffer;

	std::vector<GLuint> *boundTextures;
	std::unordered_map<GLenum, bool> glFeatures;
}

void GLState::Init()
{
	boundTextures = new std::vector<GLuint>(32);
	for(unsigned int i = 0; i < 32; ++i)
		boundTextures->at(i) = (GLuint)0;
}

void GLState::Cleanup()
{
	delete boundTextures;
}

bool GLState::Set(GLenum feature, bool enable)
{
	if(glFeatures.find(feature) == glFeatures.end() || glFeatures[feature] != enable)
	{
		glFeatures[feature] = enable;
		if(enable)
			glEnable(feature);
		else
			glDisable(feature);

		Profiler::_stateChanges.x += 1;
		return true;
	}

	Profiler::_stateChanges.y += 1;
	return false;
}

bool GLState::ActiveTexture(GLuint pos)
{
	if(activeTexture != pos)
	{
		activeTexture = pos;
		glActiveTexture(GL_TEXTURE0 + pos);
		Profiler::_stateChanges.x += 1;
		return true;
	}

	Profiler::_stateChanges.y += 1;
	return false;
}
bool GLState::UseProgram(GLuint id)
{
	if(boundShader != id)
	{
		boundShader = id;
		glUseProgram(id);
		Profiler::_stateChanges.x += 1;
		return true;
	}
	
	Profiler::_stateChanges.y += 1;
	return false;
}
bool GLState::BindVertexArray(GLuint id)
{
	if(boundVAO != id)
	{
		boundVAO = id;
		glBindVertexArray(id);
		Profiler::_stateChanges.x += 1;
		return true;
	}

	Profiler::_stateChanges.y += 1;
	return false;
}
bool GLState::BindFramebuffer(GLuint id)
{
	if(boundFramebuffer != id)
	{
		boundFramebuffer = id;
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		Profiler::_stateChanges.x += 1;
		return true;
	}

	Profiler::_stateChanges.y += 1;
	return false;
}
bool GLState::BindTexture(GLuint id)
{
	if(boundTextures->at(activeTexture) != id)
	{
		boundTextures->at(activeTexture) = id;
		glBindTexture(GL_TEXTURE_2D, id);
		Profiler::_stateChanges.x += 1;
		return true;
	}

	Profiler::_stateChanges.y += 1;
	return false;
}
bool GLState::BindTexture(GLuint id, GLuint pos)
{
	if(boundTextures->at(pos) != id)
	{
		ActiveTexture(pos);
		return BindTexture(id);
	}

	Profiler::_stateChanges.y += 1;
	return false;
}