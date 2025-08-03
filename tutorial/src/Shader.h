#pragma once 
#include <string>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	// caching system for uniforms later 
public: 
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	
	void SetUniform(const std::string& name, float v0, float v1, float f2, float f3);

private:
	
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int GetUniformLocation(const std::string& name);

};