#include "Common.h"
#include "ShaderProgram.h"

using namespace FlappyBird;

ShaderProgram::ShaderProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
	vertexShader = Compile( GL_VERTEX_SHADER, strVertexShader.c_str() );
	fragmentShader = Compile( GL_FRAGMENT_SHADER, strFragmentShader.c_str() );

	if ( vertexShader && fragmentShader )
    {
        program = glCreateProgram();
        
        glAttachShader( program, vertexShader );
        glAttachShader( program, fragmentShader );

		GLint status;
		glLinkProgram( program );
		glGetProgramiv( program, GL_LINK_STATUS, &status );
		LOG( "status: %i", status);
		GLint length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		if (length > 0)
		{
			GLchar *log = (GLchar *)malloc(length);
			glGetProgramInfoLog(program, length, &length, log);
			LOG(log);
			free(log);
		}
    }
}

ShaderProgram::~ShaderProgram()
{
	glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
	glDeleteProgram( program );

}

GLuint ShaderProgram::Compile( GLenum type, const char* source )
{
	GLuint shader = glCreateShader( type );
	glShaderSource( shader, 1, &source, nullptr );
    glCompileShader( shader );
	return shader;
}

void ShaderProgram::Apply()
{
	glUseProgram( program );
}

void ShaderProgram::Finish()
{
	glUseProgram( 0 );
}

GLuint ShaderProgram::GetAttribLocation( const std::string& attribName ) const
{
	GLuint ret;
	ret = glGetAttribLocation( program, attribName.c_str() );
	return ret;
}

GLuint ShaderProgram::GetUniformLocation( const std::string& uniformName ) const
{
	GLuint ret;
	ret = glGetUniformLocation( program , uniformName.c_str() );
	return ret;
}