#include "Common.h"
#include "LineShaderProgram.h"

#include "Application.h"

using namespace FlappyBird;

const char vertexShaderStr[] =
	"uniform mat4 mMVP;\n"

	"varying vec2 texCoords;\n"
	"varying vec4 colors;\n"

	"attribute vec2 mVerts;\n"
	"attribute vec2 mTexCoords;\n"
	"attribute vec4 mColors;\n"

	"void main()\n"
	"{\n"
	"	gl_Position = mMVP * vec4(mVerts, 0.0, 1.0);\n"

	"	texCoords = vec2(mTexCoords.x, mTexCoords.y);\n"
	"	colors = mColors;\n"
	"}";

const char fragmentShaderStr[] =
	"varying vec2 texCoords;\n"
	"varying vec4 colors;\n"
	"uniform sampler2D tex;\n"

	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(tex, texCoords) * colors;\n"
	"}";


LineShaderProgram::LineShaderProgram():
ShaderProgram(vertexShaderStr, fragmentShaderStr),
mProjection(0)
{

	//attribPosition0 = GetAttribLocation("a_position0");
	//attribPosition1 = GetAttribLocation("a_position1");
	//attribNormal = GetAttribLocation("a_normal");
	//attribTextcoord = GetAttribLocation("a_textcoord");
	//attribLineWidth = GetUniformLocation("lineWidth");

	//mVerts.resize(1024);
	//mTexCoords.resize(1024);
	//mColors.resize(1024);
	//mIndices.resize(1024);
}

void LineShaderProgram::SetProjection(const Matrix& matrix)
{
	mProjection = matrix;
}

LineShaderProgram::~LineShaderProgram()
{
	mVerts.clear();
	mTexCoords.clear();
	mColors.clear();
	mIndices.clear();
}

void LineShaderProgram::SetColor( const Color& _color )
{
	color = _color;
}

void LineShaderProgram::SetWidth( const float& _width )
{
	width = _width;
}

void LineShaderProgram::SetTexture( const std::string& filename )
{
	FILE* image_file = Application::GetInstance()->OpenFile(filename, "rb");
	char* data;
	char file_info[4];
	if (image_file == nullptr)
		return;

	size_t bytesRead;

	fseek(image_file, 18, SEEK_CUR);

	bytesRead = fread(&file_info, 1, 4, image_file);
	int image_width = to_int32(file_info, 4);
	bytesRead = fread(&file_info, 1, 4, image_file);
	int image_height = to_int32(file_info, 4);

	fseek(image_file, 28, SEEK_CUR);

	int image_area = image_width*image_height;
	data = new char[image_area * 3];
	fread(data, 1, image_area*3, image_file);

	fclose(image_file);
	//if(!image_file.good())
	//{
	//    return;
	//}
	
	//image_file.seekg(18,image_file.cur);//skip beginning of header
	//image_file.read(file_info,4);//width
	//int image_width = to_int32(file_info,4);//convert raw data to integer
	//image_file.read(file_info,4);//height
	//int image_height = to_int32(file_info,4);//convert raw data to integer
	//image_file.seekg(28,image_file.cur);//skip rest of header
	//int image_area = image_width*image_height;//calculate area now since it'll be used 3 times
	//data = new char[image_area*3];//set the buffer size
	//image_file.read(data, image_area*3);//get the pixel matrix
	//image_file.close();
	//next we need to rearrange the color values from BGR to RGB

	//std::ifstream image_file;
 //   char* data;
 //   char file_info[4];
 //   image_file.open(filename.c_str(), std::ios::binary);

 //   if(!image_file.good())
 //   {
 //       return;
 //   }

 //   image_file.seekg(18,image_file.cur);//skip beginning of header
 //   image_file.read(file_info,4);//width
 //   int image_width = to_int32(file_info,4);//convert raw data to integer
 //   image_file.read(file_info,4);//height
 //   int image_height = to_int32(file_info,4);//convert raw data to integer
 //   image_file.seekg(28,image_file.cur);//skip rest of header
 //   int image_area = image_width*image_height;//calculate area now since it'll be used 3 times
 //   data = new char[image_area*3];//set the buffer size
 //   image_file.read(data, image_area*3);//get the pixel matrix
 //   image_file.close();
    //next we need to rearrange the color values from BGR to RGB
    for(int i=0; i<image_area; ++i)//iterate through each cell of the matrix
    {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];
        data[index] = R;
        data[index+2] = B;
    }
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
    delete[] data;
}

void LineShaderProgram::Begin()
{
}

void LineShaderProgram::DrawLine(const Vector2& p1, const Vector2& p2)
{
	float angle = atan2(p2.y - p1.y, p2.x - p1.x);
	float offsetx = sinf(angle) * (width / 2);
	float offsety = cosf(angle) * (width / 2);

	size_t curIndex = mVerts.size();

	Vector4 vcolor = Vector4(color.r, color.g, color.b, color.a);

//	mVerts[vcount++] = p1;
//	mVerts[vcount++] = p1;
	//mVerts.push_back(Vector2(p1 - Vector2(offsetx, offsety)));
	//mVerts.push_back(Vector2(p1 + Vector2(offsetx, offsety)));
	//mVerts.push_back(Vector2(p2 + Vector2(offsetx, offsety)));
	//mVerts.push_back(Vector2(p2 - Vector2(offsetx, offsety)));
//	mVerts[vcount++] = p2;
//	mVerts[vcount++] = p2;

	mVerts.push_back(Vector2(p1 + Vector2(-offsetx, offsety)));
	mVerts.push_back(Vector2(p1 + Vector2(offsetx, -offsety)));
	mVerts.push_back(Vector2(p2 + Vector2(offsetx, -offsety)));
	mVerts.push_back(Vector2(p2 + Vector2(-offsetx, offsety)));

	//mVerts.push_back(Vector2(0, 0));
	//mVerts.push_back(Vector2(10,0));
	//mVerts.push_back(Vector2(10,10));
	//mVerts.push_back(Vector2(0,10));

	//mTexCoords.push_back(Vector2(0, 0));
	//mTexCoords.push_back(Vector2(0, 1));
	//mTexCoords.push_back(Vector2(1, 1));
	//mTexCoords.push_back(Vector2(1, 0));

	mTexCoords.push_back(Vector2(0.5, 0));
	mTexCoords.push_back(Vector2(0.5, 1));
	mTexCoords.push_back(Vector2(0.5, 1));
	mTexCoords.push_back(Vector2(0.5, 0));

	mColors.push_back(vcolor);
	mColors.push_back(vcolor);
	mColors.push_back(vcolor);
	mColors.push_back(vcolor);

	mIndices.push_back(curIndex + 0);
	mIndices.push_back(curIndex + 2);
	mIndices.push_back(curIndex + 1);

	mIndices.push_back(curIndex + 0);
	mIndices.push_back(curIndex + 3);
	mIndices.push_back(curIndex + 2);


	curIndex = mVerts.size();

	float xLedge = sinf(angle - PI / 2) * (width / 2);
	float yLedge = cosf(angle - PI / 2) * (width / 2);

	mVerts.push_back(Vector2(p1 + Vector2(-offsetx, offsety) + Vector2(xLedge, -yLedge)));
	mVerts.push_back(Vector2(p1 + Vector2(offsetx, -offsety) + Vector2(xLedge, -yLedge)));
	mVerts.push_back(Vector2(p1 + Vector2(offsetx, -offsety)));
	mVerts.push_back(Vector2(p1 + Vector2(-offsetx, offsety)));

	mTexCoords.push_back(Vector2(0, 0));
	mTexCoords.push_back(Vector2(0, 1));
	mTexCoords.push_back(Vector2(0.5, 1));
	mTexCoords.push_back(Vector2(0.5, 0));

	mColors.push_back(vcolor);
	mColors.push_back(vcolor);
	mColors.push_back(vcolor);
	mColors.push_back(vcolor);

	mIndices.push_back(curIndex + 0);
	mIndices.push_back(curIndex + 2);
	mIndices.push_back(curIndex + 1);

	mIndices.push_back(curIndex + 0);
	mIndices.push_back(curIndex + 3);
	mIndices.push_back(curIndex + 2);


	curIndex = mVerts.size();

	xLedge = sinf(angle - PI / 2) * (width / 2);
	yLedge = cosf(angle - PI / 2) * (width / 2);

	mVerts.push_back(Vector2(p2 + Vector2(-offsetx, offsety) + Vector2(-xLedge, yLedge)));
	mVerts.push_back(Vector2(p2 + Vector2(offsetx, -offsety) + Vector2(-xLedge, yLedge)));
	mVerts.push_back(Vector2(p2 + Vector2(offsetx, -offsety)));
	mVerts.push_back(Vector2(p2 + Vector2(-offsetx, offsety)));

	mTexCoords.push_back(Vector2(0, 0));
	mTexCoords.push_back(Vector2(0, 1));
	mTexCoords.push_back(Vector2(0.5, 1));
	mTexCoords.push_back(Vector2(0.5, 0));

	mColors.push_back(vcolor);
	mColors.push_back(vcolor);
	mColors.push_back(vcolor);
	mColors.push_back(vcolor);

	mIndices.push_back(curIndex + 0);
	mIndices.push_back(curIndex + 2);
	mIndices.push_back(curIndex + 1);

	mIndices.push_back(curIndex + 0);
	mIndices.push_back(curIndex + 3);
	mIndices.push_back(curIndex + 2);

	/*vertex[vertexCount++] = { p2[0], p2[1], p2[2], p1[0], p1[1], p1[2], vline_vertexOffset[0][0], vline_vertexOffset[0][1], vline_texCoord[0][0], vline_texCoord[0][1] };
	vertex[vertexCount++] = { p2[0], p2[1], p2[2], p1[0], p1[1], p1[2], vline_vertexOffset[1][0], vline_vertexOffset[1][1], vline_texCoord[1][0], vline_texCoord[1][1] };
	vertex[vertexCount++] = { p2[0], p2[1], p2[2], p1[0], p1[1], p1[2], vline_vertexOffset[2][0], vline_vertexOffset[2][1], vline_texCoord[2][0], vline_texCoord[2][1] };
	vertex[vertexCount++] = { p2[0], p2[1], p2[2], p1[0], p1[1], p1[2], vline_vertexOffset[3][0], vline_vertexOffset[3][1], vline_texCoord[3][0], vline_texCoord[3][1] };
	vertex[vertexCount++] = { p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], vline_vertexOffset[4][0], vline_vertexOffset[4][1], vline_texCoord[4][0], vline_texCoord[4][1] };
	vertex[vertexCount++] = { p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], vline_vertexOffset[5][0], vline_vertexOffset[5][1], vline_texCoord[5][0], vline_texCoord[5][1] };
	vertex[vertexCount++] = { p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], vline_vertexOffset[6][0], vline_vertexOffset[6][1], vline_texCoord[6][0], vline_texCoord[6][1] };
	vertex[vertexCount++] = { p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], vline_vertexOffset[7][0], vline_vertexOffset[7][1], vline_texCoord[7][0], vline_texCoord[7][1] };*/

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount / 2);
	//printOpenGLError();
	
	//glBegin(GL_TRIANGLE_STRIP);
	//glNormal3fv(vline_vertexOffset[0]);
	//glTexCoord2fv(vline_texCoord[0]);
	//glVertexAttrib4f(otherAttribLocation, p2[0], p2[1], p2[2], 1.0f);
	//glVertex3fv(p1);
	//glNormal3fv(vline_vertexOffset[1]);
	//glTexCoord2fv(vline_texCoord[1]);
	//glVertexAttrib4f(otherAttribLocation, p2[0], p2[1], p2[2], 1.0f);
	//glVertex3fv(p1);
	//glNormal3fv(vline_vertexOffset[2]);
	//glTexCoord2fv(vline_texCoord[2]);
	//glVertexAttrib4f(otherAttribLocation, p2[0], p2[1], p2[2], 1.0f);
	//glVertex3fv(p1);
	//glNormal3fv(vline_vertexOffset[3]);
	//glTexCoord2fv(vline_texCoord[3]);
	//glVertexAttrib4f(otherAttribLocation, p2[0], p2[1], p2[2], 1.0f);
	//glVertex3fv(p1);
	//glNormal3fv(vline_vertexOffset[4]);
	//glTexCoord2fv(vline_texCoord[4]);
	//glVertexAttrib4f(otherAttribLocation, p1[0], p1[1], p1[2], 1.0f);
	//glVertex3fv(p2);
	//glNormal3fv(vline_vertexOffset[5]);
	//glTexCoord2fv(vline_texCoord[5]);
	//glVertexAttrib4f(otherAttribLocation, p1[0], p1[1], p1[2], 1.0f);
	//glVertex3fv(p2);
	//glNormal3fv(vline_vertexOffset[6]);
	//glTexCoord2fv(vline_texCoord[6]);
	//glVertexAttrib4f(otherAttribLocation, p1[0], p1[1], p1[2], 1.0f);
	//glVertex3fv(p2);
	//glNormal3fv(vline_vertexOffset[7]);
	//glTexCoord2fv(vline_texCoord[7]);
	//glVertexAttrib4f(otherAttribLocation, p1[0], p1[1], p1[2], 1.0f);
	//glVertex3fv(p2);
	//glEnd();
}

void LineShaderProgram::End()
{
	Flush();
}

void LineShaderProgram::Flush()
{
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glDepthMask(GL_FALSE);
	//glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	Apply();

	GLuint location;

	location = GetUniformLocation("mMVP");
	glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&mProjection.value[0]);

	printOglError();

	location = GetAttribLocation("mVerts");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_TRUE, 0, &mVerts[0]);

	printOglError();

	location = GetAttribLocation("mTexCoords");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_TRUE, 0, &mTexCoords[0]);
	printOglError();

	location = GetAttribLocation("mColors");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_TRUE, 0, &mColors[0]);
	printOglError();

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_SHORT, &mIndices[0]);

	glBindTexture(GL_TEXTURE_2D, 0);

	Finish();

	mVerts.clear();
	mTexCoords.clear();
	mColors.clear();
	mIndices.clear();
}