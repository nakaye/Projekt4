#include "shaders.h"

Shaders::Shaders()
{
	wysokoscOkna = 768;
	szerokoscOkna = 1024;
	polozenieOknaX = 100;
	polozenieOknaY = 100;
}

Shaders::Shaders(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY)
{
	this->wysokoscOkna = wysokoscOkna;
	this->szerokoscOkna = szerokoscOkna;
	this->polozenieOknaX = polozenieOknaX;
	this->polozenieOknaY = polozenieOknaY;
}

Shaders::~Shaders(void)
{
	std::cout << "To ja, twoj destruktor!" << std::endl;
	system("pause");
}

void Shaders::stworzenieOkna(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(polozenieOknaX, polozenieOknaY);
	glutCreateWindow("okno");
}

void  Shaders::wyswietl()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
}

void Shaders::inicjalizacjaGlew(){
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK){
		std::cerr << "Nie udalo sie" << glewGetErrorString(wynik) << std::endl;
		system("pause");
		exit(1);
	}
}

void Shaders::stworzenieVAO(){
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void Shaders::stworzenieVBO(){
	GLfloat Wierzcholki[9] = { 0.0f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f };
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Wierzcholki), Wierzcholki, GL_STATIC_DRAW);
}

void Shaders::storzenieProgramu(){
	programZShaderami = glCreateProgram();
	if (programZShaderami == 0){
		std::cerr << "Bład podczas tworzrnia programu shaderow" << std::endl;
		system("pause");
		exit(1);
	}

	char * vertexShader =
		"#version 330\n"
		"layout (location = 0) in vec3 polozenie;"
		"void main()"
		"{"
		"gl_Position = vec4(0.5 * polozenie.x, 0.5 * polozenie.y, polozenie.z, 1.0);"
		"}";

	char * fragmentShader = "#version 330\n"
		"out vec4 kolor;"
		"void main()"
		"{"
		"kolor = vec4(0.8, 0.8, 0.0, 1.0);"
		"}";

	dodawanieDoProgramu(programZShaderami, vertexShader, GL_VERTEX_SHADER);
	dodawanieDoProgramu(programZShaderami, fragmentShader, GL_FRAGMENT_SHADER);

	GLint linkowanieOK = 0;
	glLinkProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_LINK_STATUS, &linkowanieOK);
	if (linkowanieOK == GL_FALSE){

		GLint dlugoscLoga = 0;
		glGetShaderiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar>log(dlugoscLoga);
		glGetShaderInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);
		std::cerr << "Blad podczas linkowania programou shaderow" << std::endl;
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); i++)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteProgram(programZShaderami);
		system("pause");
		exit(1);
	}

	GLint walidacjaOK = 0;
	glLinkProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_VALIDATE_STATUS, &walidacjaOK);
	if (walidacjaOK == GL_FALSE){
		GLint dlugoscLoga = 0;
		glGetShaderiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar>log(dlugoscLoga);
		glGetShaderInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);
		std::cerr << "Blad" << std::endl;
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); i++)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteProgram(programZShaderami);
		system("pause");
		exit(1);
	}
	glUseProgram(programZShaderami);
}


void Shaders::dodawanieDoProgramu(GLuint programZShaderami, const char* tekstShadera, GLenum typShadera){
	GLuint shader = glCreateShader(typShadera);
	if (shader == 0){
		std::cerr << "Bład podczas tworzrnia shadera" << std::endl;
		system("pause");
		exit(1);
	}

	const GLchar* tekstShaderaTab[1];
	tekstShaderaTab[0] = tekstShadera;
	GLint dlugoscShadera[1];
	dlugoscShadera[0] + strlen(tekstShadera);
	glShaderSource(shader, 1, tekstShaderaTab, dlugoscShadera);
	glCompileShader(shader);
	GLint kompilacjaOK = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &kompilacjaOK);
	if (kompilacjaOK == GL_FALSE)
	{
		GLint dlugoscLoga = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetShaderInfoLog(shader, dlugoscLoga, NULL, &log[0]);
		std::cerr << std::endl;
		std::cerr << "Blad podczas kompilacji shadera." << std::endl;
		std::cerr << std::endl;
		std::cerr << "log: ";
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteShader(shader);
		system("pause");
		exit(1);
	}
	glAttachShader(programZShaderami, shader);
}

int main(int argc, char** argv)
{
	Shaders trojkat(786, 1024, 100, 100);
	trojkat.stworzenieOkna(argc, argv);

	trojkat.inicjalizacjaGlew();
	trojkat.stworzenieVAO();
	trojkat.stworzenieVBO();
	glutDisplayFunc(trojkat.wyswietl);
	trojkat.storzenieProgramu();

	glClearColor(1.0f, 0.8f, 0.9f, 0.0f);

	glutMainLoop();
	return 0;


}
