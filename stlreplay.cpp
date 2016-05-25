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
	float mX, mY;							// 위치
	float mR = 1.0f, mG = 0.0f, mB = 0.5f;		// 색상
};
//--------------함수 선언 구간---------------------------
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
//--------------컨테이너 / 전역변수 선언 구간-----------
vector<Point> point;					//좌푝값 담는 벡터
vector<Point>::iterator piter;
vector<Point>::iterator piterForStep;
vector<double> pointTime;					//시간값 담는 벡터
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
	//초기화 함수들 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);	// 디스플레이 모드 설정 
	glutInitWindowPosition(100, 100);				// 윈도우의 위치지정 
	glutInitWindowSize(900, 600);					// 윈도우의 크기 지정
	glutCreateWindow("실습1");						// 윈도우 생성 (윈도우 이름) 
	glutMouseFunc(Mouse);							//마우스 입력함수
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);						// 출력 함수의 지정
	glutReshapeFunc(Reshape);
	Init();
	glutTimerFunc(100, TimerFunction, 1);
	glutMainLoop();
}
// 윈도우 출력 함수

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	 // 바탕색을 'blue' 로 지정
	glClear(GL_COLOR_BUFFER_BIT);			 // 설정된 색으로 젂체를 칠하기

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	//여기에 반복문으로 그려줌.
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
	glFlush(); // 화면에 출력하기
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
	//--------------리플레이 온--------
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

	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(100, TimerFunction, 1); // 타이머함수 재 설정f
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
	glutAddMenuEntry("리플레이 모드", 1);		//replayon 변수 트루로.
	glutAddMenuEntry("리플레이 저장", 2);
	glutAddMenuEntry("일반 모드", 3);
	glutAddMenuEntry("단계별 재생",4);
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
		//파일 불러오는 함수
		break;
	case 2:
		inputFile();
		//파일에 저장하는 함수
		break;
	case 3:
		changeNormalMode();
		//일반모드 전환함수
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