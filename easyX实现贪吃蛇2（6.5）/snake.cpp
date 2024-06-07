#include<iostream>
#include<graphics.h>
#include<conio.h>//_kbhit()
using namespace std;

#define WIDTH 40  //横向上格子的数量
#define HEIGHT 30 //竖向上格子的数量
#define SIZE 20   //格子的大小

int Blocks[WIDTH][WIDTH] = { 0 };//初始化二维数组
char moveDirection,oldmoveDirection;//蛇的移动方向
int foodX, foodY;//食物的坐标
int isFalse = 0;

void intiGame();
void show();//每次运行重新绘制蛇和食物的图像
void updateWithInput();
void updateWithoutInput();
void moveSnake();//有关蛇的移动

int main()
{
	intiGame();
	IMAGE img;
	loadimage(&img, _T("../贪吃蛇.jpg"));
	putimage((WIDTH * SIZE - 313)/2, (HEIGHT * SIZE - 234) / 2, &img);

	cout << "             贪吃蛇大作战！            " << endl;
	cout << "***************************************" << endl;
	cout << "*           输入1开始游戏！           *" << endl;
	cout << "*     注意：请提前切换为英文输入法    *" << endl;
	cout << "***************************************" << endl;
	int num = 0;
	cin >> num;
	if (num == 1)
	{
		while (1)
			{
				show();//进行绘制
				updateWithoutInput();//和输入无关的更新 
				updateWithInput();//和输入有关的更新
			}
	}
	else cout << "输入错误" << endl;
	
	return 0;
}
void intiGame()//初始化
{

	//初始化界面
	initgraph(WIDTH * SIZE, HEIGHT * SIZE, 1);
	setbkcolor(LIGHTGRAY);
	cleardevice();

	//画线
	setlinecolor(WHITE);
	int i = 0;
	for (i = SIZE; i < WIDTH * SIZE; i += SIZE)
		line(i, 0, i, HEIGHT * SIZE);//竖线width为间距
	for (i = SIZE; i < HEIGHT * SIZE; i += SIZE)
		line(0, i, WIDTH * SIZE, i);//横线height为间距
	
	//画蛇头
	Blocks[WIDTH / 2][HEIGHT / 2] = 1;//// 画面中间画蛇头，数字为1
	for (i = 1; i < 4; i++)// 向左依次4个蛇身，数值依次为2,3,4,5
		Blocks[WIDTH / 2 - i][HEIGHT / 2] = i + 1;

	//初始化蛇的运动方向
	moveDirection = 'd';//向右走
	oldmoveDirection = moveDirection;

	//初始化随机食物位置
	foodX = rand() % (WIDTH - 2) + 1;
	foodY = rand() % (HEIGHT - 2) + 1;// 1到38，即最外圈不会生成食物

}
void show()//绘制函数
{
	BeginBatchDraw();

	//遍历格子，设置蛇身颜色
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
	//食物的颜色
	setfillcolor(LIGHTGREEN);
	fillrectangle(foodX* SIZE, foodY * SIZE, (foodX + 1) * SIZE, (foodY + 1) * SIZE);

	//失败退出游戏
	if (isFalse) //如果游戏失败
	{
		setbkmode(TRANSPARENT); //文字字体透明    
		settextcolor(RGB(255, 0, 0));//设定文字颜色
		settextstyle(80, 0, _T("宋体")); //设定文字大小、样式
		outtextxy(240, 220, _T("游戏失败")); //输出文字内容
	}

	FlushBatchDraw();

	if (isFalse)
		return;
}
void updateWithInput()//有按键输入
{
	if (_kbhit() && isFalse == 0)//如果有按键输入，并且不失败
	{
		char input = _getch(); //  获得按键输入
		if (input == 'a' || input == 's' || input == 'd' || input == 'w')
		{
			moveDirection = input;//设定移动方向

			//防止反键退出
			if (moveDirection == 'a' && oldmoveDirection == 'd')
				moveDirection = 'd';
			else if (moveDirection == 'd' && oldmoveDirection == 'a')
				moveDirection = 'a';
			else if (moveDirection == 's' && oldmoveDirection == 'w')
				moveDirection = 'w';
			else if (moveDirection == 'w' && oldmoveDirection == 's')
				moveDirection = 's';
			oldmoveDirection = moveDirection;

			moveSnake();//调用小蛇移动函数
		}
	}
}
void updateWithoutInput()//无按键输入
{
	if (isFalse)
		return;

	static int waitIndex = 1;//解决移动延迟问题
	waitIndex++;
	if (waitIndex == 15) //数字越小，速度越快
	{
		moveSnake();
		waitIndex = 1;
	}
	//Sleep(200);
}
void moveSnake()//小蛇移动函数
{
	/*先每一个格子加1，然后将对应方向上的格子赋值为1，
	删除最后一个格子（最大的），如吃到了食物则不必删除*/

	//日常蛇身全部加一
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (Blocks[i][j] != 0)
				Blocks[i][j]++;
		}
	}
	//找旧蛇头//找旧蛇尾
	int oldHeadX, oldHeadY;
	int oldTailX, oldTailY;
	int MAX = 0;
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (Blocks[i][j] == 2) {//旧蛇头
				oldHeadX = i;
				oldHeadY = j;
			}
			if (MAX < Blocks[i][j]) {//旧蛇尾
				MAX = Blocks[i][j];
				oldTailX = i;
				oldTailY = j;
			}
		}
	}

	//根据蛇的移动方向来赋值新蛇头的xy
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

	//判断游戏是否失败
	if (newHeadX >= WIDTH || newHeadX < 0 || newHeadY >= HEIGHT || newHeadY < 0 || Blocks[newHeadX][newHeadY] != 0)
		// 依次是：撞到右边，  撞到左边，     撞到下边，            撞到上边，        吃到蛇身
		//由于此处判断是否吃到蛇身时，用的是newHeadX和newHeadY，所以判断语句需要放在对新蛇头赋值之前
	{
		isFalse = 1;
		return;
	}

	//更新蛇头
	Blocks[newHeadX][newHeadY] = 1;

	//对吃到食物以及对蛇尾的处理
	if (newHeadX == foodX && newHeadY == foodY)
	{
		//如果吃掉食物，刷新下一个食物
		foodX = rand() % (WIDTH - 2) + 1;
		foodY = rand() % (HEIGHT - 2) + 1;
	}
	else
	{
		//删掉旧蛇尾
		Blocks[oldTailX][oldTailY] = 0;
	}
}
