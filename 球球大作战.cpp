/*      球球大作战.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
        作者：蔡强
		日期：20200604
*/

#include "pch.h"
#include <iostream>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
#include<math.h>

#define HEIGHT      640
#define WIDTH       1024
#define MAP_HEIGHT  HEIGHT*3
#define MAP_WIDTH   WIDTH*3
#define Food_NUMS   600
#define AINUMS      100



struct Food
{
	int x;
	int y;
	int r;
	int flag;
	DWORD color;
};
 Food food[Food_NUMS ];
 Food player;
 Food AI[AINUMS];
//定义贴图
IMAGE map(MAP_WIDTH, MAP_HEIGHT);
//设置一个中心点坐标，其值随玩家移动不断变化
POINT centrol;
void initcentrol()
{
	//以人物为中心
	centrol.x = player.x - WIDTH / 2;
	centrol.y = player.y - HEIGHT / 2;
	//判断中心越界
	if (centrol.x < 0)centrol.x = 0;
	if (centrol.y < 0)centrol.y = 0;
	if (centrol.x > MAP_WIDTH - WIDTH)centrol.x = MAP_WIDTH - WIDTH;
	if (centrol.y > MAP_HEIGHT - HEIGHT)centrol.y = MAP_HEIGHT - HEIGHT;
}
void initmap()     //初始化
{
	srand((unsigned)time(NULL));

	//初始化食物
	for (int i = 0; i < Food_NUMS; i++)    
	{
		food[i].x = rand() % MAP_HEIGHT;
		food[i].y = rand() % MAP_WIDTH;
		food[i].r = rand() % 5 + 1;
		food[i].flag = 1;
		food[i].color =RGB(rand() % 256, rand() % 256, rand() % 256);
	}
	//初始化玩家
	player.x = rand() % MAP_HEIGHT;
	player.y = rand() % MAP_WIDTH;
	player.r = rand() % 10 + 10;
	player.flag = 1;
	player.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	//初始化AI 
	for (int i = 0; i < AINUMS; i++)
	{
		AI[i].x = rand() % MAP_HEIGHT;
		AI[i].y = rand() % MAP_WIDTH;
		AI[i].r = rand() % 20 + 20;
		AI[i].flag = 1;
		AI[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
	

}

void GameDraw()
{
	SetWorkingImage(&map);  //设置工作区
	setbkcolor(WHITE);      //设置背景颜色
	cleardevice();          //清屏
	//绘制食物
	for (int i = 0; i < Food_NUMS; i++)
	{
		if (food[i].flag == 1)
		{
			setfillcolor(food[i].color);      //设置填充颜色
			fillcircle(food[i].x, food[i].y, food[i].r);    //有边框的填充圆
		}
	}
	//绘制玩家
	setfillcolor(player.color);
	fillcircle(player.x, player.y, player.r);
	settextcolor(player.color);
	settextstyle(20, 0, "黑体");
	outtextxy(player.x - 30, player.y, "玩家控制");
	//绘制AI 
	for (int i = 0; i < AINUMS; i++)
	{
		if (AI[i].flag == 1)
		{
			setfillcolor(AI[i].color);
			fillcircle(AI[i].x, AI[i].y, AI[i].r);
		}
	}

	initcentrol();
	SetWorkingImage();
	putimage(0,0, WIDTH, HEIGHT,&map, centrol.x, centrol.y);     //绘制图片到屏幕

}
void game_move()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		if(player.y - player.r > 0)
		{
			player.y -= 3;
		}
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		if(player.y + player.r < MAP_HEIGHT)
		{
			player.y += 3;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (player.x + player.r < MAP_WIDTH)
		{
			player.x += 3;
		}
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if (player.x - player.r > 0)
		{
			player.x -= 3;
		}
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		player.r += 5;
	}

}
double distance(Food b1,Food b2)
{
	return (double)sqrt((b1.x - b2.x)*(b1.x - b2.x) +(b1.y - b2.y)*(b1.y - b2.y));
}
void eat()
{
	//吃食物
	for (int i = 0; i < Food_NUMS; i++)
	{
		if(food[i].flag == 1 && distance(player,food[i]) < player.r)
		{
			food[i].flag = 0;
			player.r += food[i].r / 4;
		}
	}
	//吃AI或者被AI吃
	for (int i = 0; i < AINUMS; i++)
	{
		if (AI[i].flag == 1 && distance(player, AI[i]) < player.r && AI[i].r < player.r)
		{
			AI[i].flag = 0;
			player.r += AI[i].r / 4;
		}
		else if (AI[i].flag == 1 && distance(player, AI[i]) < player.r && AI[i].r > player.r)
		{
			player.flag = 0;
			//settextstyle(10,10,);
			setbkmode(TRANSPARENT);
			settextcolor(player.color);
			settextstyle(30, 0, "黑体");
			outtextxy(player.x, player.y, "游戏失败！！！");
			Sleep(5000);
			closegraph();
			exit(0);
		}
	}
}
int main()
{
	initgraph(WIDTH,HEIGHT);    //初始化图形环境
	//loadimage(NULL, _T("F:\\test.jpg"));
	initmap();
	while (1)
	{
		
		GameDraw();
		game_move();
		eat();
	}
	system("pause");
	return 1;

}

