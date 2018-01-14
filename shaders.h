#ifndef SHADERS_H
#define SHADERS_H

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

GLuint VAO;
GLuint VBO;

class Shaders
{
private:
	int wysokoscOkna;
	int szerokoscOkna;
	int polozenieOknaX;
	int polozenieOknaY;
	GLuint programZShaderami;

public:
	Shaders(void);
	Shaders(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY);
	~Shaders(void);
	void stworzenieOkna(int argc, char** argv);
	void inicjalizacjaGlew();
	static void wyswietl();
	void stworzenieVAO();
	void stworzenieVBO();
	void storzenieProgramu();
	void dodawanieDoProgramu(GLuint programZShaderami, const char* tekstShadera, GLenum typShadera);

};

#endif SHADERS_H
