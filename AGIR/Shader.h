#pragma once

#include "Utilities.h"
#include <iostream>


class Shader
{
public:
	GLuint programID;

	Shader();
	Shader(const char *vertexshaderfile, const char *fragmentshaderfile);	
	~Shader();

	void createShader();
	void createShader(const char *vertexshaderfile, const char *fragmentshaderfile);

	private:

		/*
		* Override the Win32 filelength() function with
		* a version that takes a Unix-style file handle as
		* input instead of a file ID number, and which works
		* on platforms other than Windows.
		*/
		long filelength(FILE *file);

		/*
		* readShaderFile(filename) - read a shader source string from a file
		*/
		unsigned char* readShaderFile(const char *filename);

		void printError(const char *errtype, const char *errmsg);

};

