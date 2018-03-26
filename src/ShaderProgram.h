#pragma once

namespace FlappyBird
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string &strVertexShader, const std::string &strFragmentShader);
		virtual ~ShaderProgram();

	protected:
		void Apply();
		void Finish();

		GLuint Compile( GLenum type, const char* source );
		GLuint GetAttribLocation( const std::string& attribName ) const;
		GLuint GetUniformLocation( const std::string& uniformName ) const;

	protected:
		GLenum program;
		GLint vertexShader;
		GLint fragmentShader;
	};
}