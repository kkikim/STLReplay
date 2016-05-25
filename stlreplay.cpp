#include <GL/glut.h>		// includes gl.h glu.h
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <fstream>
using namespace std;
class Point
{
public:
	Point(float x, float y) :mX(x), mY(y)
	{}
	void draw()
	{
		glColor4f(mR, mG, mB, 1.0f);
		glVertex2f(mX, mY);
	}
	void setColor(float r, float g, float b)
	{
		mR = r;
		mG = g;
		mB = b;
	}
	float getX(){ return mX; }
	float getY(){ return mY; }
private:
	float mX, mY;							// ��ġ
	float mR = 1.0f, mG = 0.0f, mB = 0.5f;		// ����
};
//--------------�Լ� ���� ����--------------------
GLvoid inputFile();
GLvoid outputFile();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Init();
GLvoid MenuFunc(int button);
//-------------------------------------------------
vector<Point> point;					//�¾i�� ��� ����
vector<Point>::iterator piter;
vector<double> pointTime;					//�ð��� ��� ����
vector<double>::iterator Titer;
default_random_engine dre;
uniform_real_distribution<GLfloat> ui(0.2, 0.95);
chrono::system_clock::time_point start = chrono::system_clock::now();
void main(int argc, char *argv[])
{
	//�ʱ�ȭ �Լ��� 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);	// ���÷��� ��� ���� 
	glutInitWindowPosition(100, 100);				// �������� ��ġ���� 
	glutInitWindowSize(900, 600);					// �������� ũ�� ����
	glutCreateWindow("�ǽ�1");						// ������ ���� (������ �̸�) 
	glutMouseFunc(Mouse);							//���콺 �Է��Լ�
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);						// ��� �Լ��� ����
	glutReshapeFunc(Reshape);
	Init();
	glutTimerFunc(100, TimerFunction, 1);
	glutMainLoop();
}
// ������ ��� �Լ�

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	 // �������� 'blue' �� ����
	glClear(GL_COLOR_BUFFER_BIT);			 // ������ ������ ��ü�� ĥ�ϱ�

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	//���⿡ �ݺ������� �׷���.
	if (point.size() != 0)
	{
		for (piter = point.begin(); piter != point.end(); ++piter)
		{
			piter->draw();
		}
	}
	glEnd();
	glFlush(); // ȭ�鿡 ����ϱ�
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		chrono::duration<double> sec = chrono::system_clock::now() - start;
		cout << "x : " << x << " , " << "y : " << y;
		cout << "         Time : " << sec.count() << endl;
		point.push_back(Point(x, y));
		pointTime.push_back(sec.count());
	}
	glutPostRedisplay();
}
void TimerFunction(int value)
{
	if (point.size() != 0)
	{
		for (piter = point.begin(); piter != point.end(); ++piter)
		{
			piter->setColor(ui(dre), ui(dre), ui(dre));
		}
	}
	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(100, TimerFunction, 1); // Ÿ�̸��Լ� �� ����f
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	}

}

GLvoid Init()
{
	point.reserve(500);
	pointTime.reserve(500);
	int mainMenu = glutCreateMenu(MenuFunc);
	glutAddMenuEntry("���÷��� ���", 1);
	glutAddMenuEntry("���÷��� ����", 2);
	glutAddMenuEntry("�Ϲ� ���", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0.0, 900.0, 600, 0, -1.0, 1.0);
	glutPostRedisplay();
}
GLvoid MenuFunc(int button)
{
	switch (button)
	{
	case 1:
		outputFile();
		//���� �ҷ����� �Լ�
		break;
	case 2:
		inputFile();
		//���Ͽ� �����ϴ� �Լ�
		break;
	case 3:
		//�Ϲݸ�� ��ȯ�Լ�
		break;
	}
	glutPostRedisplay();
}
GLvoid inputFile()
{
	vector<Point>::iterator piter2 = point.begin();
	vector<double>::iterator Titer2 = pointTime.begin();
	ofstream out("Data.txt");
	for (piter2, Titer2; piter2 != point.end(); ++piter2, ++Titer2)
	{
		out << piter2->getX() << "\t" << piter2->getY() << "\t" << *Titer2 << endl;
	}
}
GLvoid outputFile()
{
	ifstream in("Data.txt");
	double a, b, c;
	point.clear();
	pointTime.clear();
	while (!in.eof())
	{
		in >> a >> b >> c;
		point.push_back(Point(a, b));
		pointTime.push_back(c);
		if (in.eof())
			break;
		cout << a << "\t" << b << "\t" << c << endl;
		//tempinfo.push_back(temp);
	}
}
