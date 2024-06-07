#include<iostream>
#include<graphics.h>
#include<conio.h>//_kbhit()
using namespace std;

#define WIDTH 40  //�����ϸ��ӵ�����
#define HEIGHT 30 //�����ϸ��ӵ�����
#define SIZE 20   //���ӵĴ�С

int Blocks[WIDTH][WIDTH] = { 0 };//��ʼ����ά����
char moveDirection,oldmoveDirection;//�ߵ��ƶ�����
int foodX, foodY;//ʳ�������
int isFalse = 0;

void intiGame();
void show();//ÿ���������»����ߺ�ʳ���ͼ��
void updateWithInput();
void updateWithoutInput();
void moveSnake();//�й��ߵ��ƶ�

int main()
{
	intiGame();
	IMAGE img;
	loadimage(&img, _T("../̰����.jpg"));
	putimage((WIDTH * SIZE - 313)/2, (HEIGHT * SIZE - 234) / 2, &img);

	cout << "             ̰���ߴ���ս��            " << endl;
	cout << "***************************************" << endl;
	cout << "*           ����1��ʼ��Ϸ��           *" << endl;
	cout << "*     ע�⣺����ǰ�л�ΪӢ�����뷨    *" << endl;
	cout << "***************************************" << endl;
	int num = 0;
	cin >> num;
	if (num == 1)
	{
		while (1)
			{
				show();//���л���
				updateWithoutInput();//�������޹صĸ��� 
				updateWithInput();//�������йصĸ���
			}
	}
	else cout << "�������" << endl;
	
	return 0;
}
void intiGame()//��ʼ��
{

	//��ʼ������
	initgraph(WIDTH * SIZE, HEIGHT * SIZE, 1);
	setbkcolor(LIGHTGRAY);
	cleardevice();

	//����
	setlinecolor(WHITE);
	int i = 0;
	for (i = SIZE; i < WIDTH * SIZE; i += SIZE)
		line(i, 0, i, HEIGHT * SIZE);//����widthΪ���
	for (i = SIZE; i < HEIGHT * SIZE; i += SIZE)
		line(0, i, WIDTH * SIZE, i);//����heightΪ���
	
	//����ͷ
	Blocks[WIDTH / 2][HEIGHT / 2] = 1;//// �����м仭��ͷ������Ϊ1
	for (i = 1; i < 4; i++)// ��������4��������ֵ����Ϊ2,3,4,5
		Blocks[WIDTH / 2 - i][HEIGHT / 2] = i + 1;

	//��ʼ���ߵ��˶�����
	moveDirection = 'd';//������
	oldmoveDirection = moveDirection;

	//��ʼ�����ʳ��λ��
	foodX = rand() % (WIDTH - 2) + 1;
	foodY = rand() % (HEIGHT - 2) + 1;// 1��38��������Ȧ��������ʳ��

}
void show()//���ƺ���
{
	BeginBatchDraw();

	//�������ӣ�����������ɫ
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (Blocks[i][j] != 0)
				setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 1));
			else
				setfillcolor(LIGHTGRAY);
			fillrectangle(i * SIZE, j * SIZE, (i + 1) * SIZE, (j + 1) * SIZE);
		}
	}
	//ʳ�����ɫ
	setfillcolor(LIGHTGREEN);
	fillrectangle(foodX* SIZE, foodY * SIZE, (foodX + 1) * SIZE, (foodY + 1) * SIZE);

	//ʧ���˳���Ϸ
	if (isFalse) //�����Ϸʧ��
	{
		setbkmode(TRANSPARENT); //��������͸��    
		settextcolor(RGB(255, 0, 0));//�趨������ɫ
		settextstyle(80, 0, _T("����")); //�趨���ִ�С����ʽ
		outtextxy(240, 220, _T("��Ϸʧ��")); //�����������
	}

	FlushBatchDraw();

	if (isFalse)
		return;
}
void updateWithInput()//�а�������
{
	if (_kbhit() && isFalse == 0)//����а������룬���Ҳ�ʧ��
	{
		char input = _getch(); //  ��ð�������
		if (input == 'a' || input == 's' || input == 'd' || input == 'w')
		{
			moveDirection = input;//�趨�ƶ�����

			//��ֹ�����˳�
			if (moveDirection == 'a' && oldmoveDirection == 'd')
				moveDirection = 'd';
			else if (moveDirection == 'd' && oldmoveDirection == 'a')
				moveDirection = 'a';
			else if (moveDirection == 's' && oldmoveDirection == 'w')
				moveDirection = 'w';
			else if (moveDirection == 'w' && oldmoveDirection == 's')
				moveDirection = 's';
			oldmoveDirection = moveDirection;

			moveSnake();//����С���ƶ�����
		}
	}
}
void updateWithoutInput()//�ް�������
{
	if (isFalse)
		return;

	static int waitIndex = 1;//����ƶ��ӳ�����
	waitIndex++;
	if (waitIndex == 15) //����ԽС���ٶ�Խ��
	{
		moveSnake();
		waitIndex = 1;
	}
	//Sleep(200);
}
void moveSnake()//С���ƶ�����
{
	/*��ÿһ�����Ӽ�1��Ȼ�󽫶�Ӧ�����ϵĸ��Ӹ�ֵΪ1��
	ɾ�����һ�����ӣ����ģ�����Ե���ʳ���򲻱�ɾ��*/

	//�ճ�����ȫ����һ
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (Blocks[i][j] != 0)
				Blocks[i][j]++;
		}
	}
	//�Ҿ���ͷ//�Ҿ���β
	int oldHeadX, oldHeadY;
	int oldTailX, oldTailY;
	int MAX = 0;
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (Blocks[i][j] == 2) {//����ͷ
				oldHeadX = i;
				oldHeadY = j;
			}
			if (MAX < Blocks[i][j]) {//����β
				MAX = Blocks[i][j];
				oldTailX = i;
				oldTailY = j;
			}
		}
	}

	//�����ߵ��ƶ���������ֵ����ͷ��xy
	int newHeadX = oldHeadX;
	int newHeadY = oldHeadY;
	switch (moveDirection) {
	case 'a':
		newHeadX -= 1;
		break;
	case 'd':
		newHeadX += 1;
		break;
	case 'w':
		newHeadY -= 1;
		break;
	case 's':
		newHeadY += 1;
		break;
	}

	//�ж���Ϸ�Ƿ�ʧ��
	if (newHeadX >= WIDTH || newHeadX < 0 || newHeadY >= HEIGHT || newHeadY < 0 || Blocks[newHeadX][newHeadY] != 0)
		// �����ǣ�ײ���ұߣ�  ײ����ߣ�     ײ���±ߣ�            ײ���ϱߣ�        �Ե�����
		//���ڴ˴��ж��Ƿ�Ե�����ʱ���õ���newHeadX��newHeadY�������ж������Ҫ���ڶ�����ͷ��ֵ֮ǰ
	{
		isFalse = 1;
		return;
	}

	//������ͷ
	Blocks[newHeadX][newHeadY] = 1;

	//�ԳԵ�ʳ���Լ�����β�Ĵ���
	if (newHeadX == foodX && newHeadY == foodY)
	{
		//����Ե�ʳ�ˢ����һ��ʳ��
		foodX = rand() % (WIDTH - 2) + 1;
		foodY = rand() % (HEIGHT - 2) + 1;
	}
	else
	{
		//ɾ������β
		Blocks[oldTailX][oldTailY] = 0;
	}
}
