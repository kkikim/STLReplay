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
//--------------�Լ� ���� ����---------------------------
GLvoid inputFile();
GLvoid outputFile();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Init();
GLvoid MenuFunc(int button);
GLvoid changeNormalMode();
GLvoid stepPlay();
//------------------------------------------------------
//--------------�����̳� / �������� ���� ����-----------
vector<Point> point;					//�¾i�� ��� ����
vector<Point>::iterator piter;
vector<Point>::iterator piterForStep;
vector<double> pointTime;					//�ð��� ��� ����
vector<double>::iterator Titer;
default_random_engine dre;
uniform_real_distribution<GLfloat> ui(0.2, 0.95);
chrono::system_clock::time_point start = chrono::system_clock::now();
bool replayOn = false;
bool Stop = false;
bool Step = false;
//-------------------------------------------------------
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
	cout << replayOn << endl;
	if (!replayOn)
	{
		if (point.size() != 0)
		{
			for (piter = point.begin(); piter != point.end(); ++piter)
			{
				piter->draw();
			}
		}
	}
	else if (replayOn)
	{
		if (point.size() != 0)
		{
			for (auto & pt : point)
			{
				pt.draw();
				
				if (!Stop) 
				{
					if (&(*piter) == &pt)
						break;
				}
			}
		}
	}
	else if (Step)
	{
		if (point.size() != 0)
		{
			for (auto & pt : point)
			{
				pt.draw();

				if (!Stop) 
				{
					if (&(*piter) == &pt)
						break;
				}
			}
		}
	}
	glEnd();
	glFlush(); // ȭ�鿡 ����ϱ�
}
GLvoid Mouse(int button, int state, int x, int y)
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
	if (!point.empty())
	{
		for (auto & pt : point)
		{
			pt.setColor(ui(dre), ui(dre), ui(dre));
			
			if (replayOn && !Stop) {
				if (&(*piter) == &pt)
					break;
			}
		}
	}
	//--------------���÷��� ��--------
	if (replayOn)
	{
		chrono::duration<double> sec = chrono::system_clock::now() - start;
		if (Titer != pointTime.end())
		{
			if (sec.count() >= *Titer)
			{
				cout << "zz" << endl;
				//piter->draw();
				cout << *Titer << endl;
				if (piter != point.end())
					++piter;
				if (Titer != pointTime.end())
					++Titer;
				if (Titer == pointTime.end())
					Stop = true;
			}
		}
	}

	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(100, TimerFunction, 1); // Ÿ�̸��Լ� �� ����f
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
	case 'X':
		if (piter != point.end())
			++piter;
		if (piter == point.end())
			Stop = true;

		break;
	}

}

GLvoid Init()
{
	point.reserve(500);
	pointTime.reserve(500);
	int mainMenu = glutCreateMenu(MenuFunc);
	glutAddMenuEntry("���÷��� ���", 1);		//replayon ���� Ʈ���.
	glutAddMenuEntry("���÷��� ����", 2);
	glutAddMenuEntry("�Ϲ� ���", 3);
	glutAddMenuEntry("�ܰ躰 ���",4);
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
		changeNormalMode();
		//�Ϲݸ�� ��ȯ�Լ�
		break;
	case 4:
		stepPlay();
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
	while (in >> a >> b >> c)
	{
		//in >> a >> b >> c;
		point.push_back(Point(a, b));
		pointTime.push_back(c);
		cout << a << "\t" << b << "\t" << c << endl;
		if (in.eof())
			break;
		//tempinfo.push_back(temp);
	}
	for (auto time : pointTime)
	{
		cout << time;
	}

	cout << endl;
	replayOn = true;
	start = chrono::system_clock::now();
	Titer = pointTime.begin();
	piter = point.begin();
}
GLvoid changeNormalMode()
{
	replayOn = false;
	point.clear();
	pointTime.clear();
}
GLvoid stepPlay()
{
	Step = true;
	ifstream in("Data.txt");
	double a, b, c;
	point.clear();
	pointTime.clear();
	while (in >> a >> b >> c)
	{
		//in >> a >> b >> c;
		point.push_back(Point(a, b));
		pointTime.push_back(c);
		cout << a << "\t" << b << "\t" << c << endl;
		if (in.eof())
			break;
		//tempinfo.push_back(temp);
	}
	cout << endl;
	replayOn = false;
	start = chrono::system_clock::now();
	Titer = pointTime.begin();
	piter = point.begin();
	piterForStep = point.begin();
	Stop = false;
}