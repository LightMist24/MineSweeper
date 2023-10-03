#include <iostream>		//其作用是：让初学者有一个方便的命令行输入输出试验环境
#include <easyx.h>		//easyx图形库
#include <conio.h>		//键盘按键信息获取
#include <stdlib.h>		//C语言系统函数
#include <ctime>		//电脑时间
#include <windows.h>	//Windows API 函数，创建窗口、读写文件、管理内存、进程与线程等

//导入编制的头文件
#include "Tools_image.h"

#define MAP_WIDTH 500	//地图宽度
#define MAP_HEIGHT 319	//地图高度
#define ROW 30		//雷块横坐标个数
#define COL 16		//雷块纵坐标个数
#define IMGW 16		//雷块的边长
#define MINE_NUM 99 //雷块数量
#define WIN_NUM	ROW * COL - MINE_NUM	//胜利数

bool isOver = false;
bool isWin = false;
bool timer = false;

//定义图像指针
IMAGE
	BackGround,				//背景
	HeadBG,					//顶部背景
	MineBG,					//雷区背景
	ScoreBG,				//计分板背景
	Key_Smile,				//按钮-笑脸
	Key_Smile_putDown,		//按钮-笑脸-按下
	Key_Smile_O,			//按钮-笑脸-张嘴
	Key_Smile_Win,			//按钮-笑脸-胜利
	Key_Smile_Fail,			//按钮-笑脸-胜利
	Default,				//默认
	putDown,				//按下
	Flag,					//旗帜
	Mine,					//雷块
	Question_Mark;			//问号

IMAGE
	//定义图片数组
	img_Score[11] = { 0 };
	//S0, S1, S2, S3, S4, S5, S6, S7, S8, S9,			//计分板数字

IMAGE
	img_Number[9] = { 0 };
	//N1, N2, N3, N4, N5, N6, N7, N8;					//雷块数字

//图片加载
void loadPicture()
{
	//不可交互
	loadimage(&BackGround, _T("./images/BackGround.png"));				//背景
	loadimage(&HeadBG, _T("./images/HeadBG.png"));						//顶部背景
	loadimage(&MineBG, _T("./images/MineBG.png"));						//雷区背景
	loadimage(&ScoreBG, _T("./images/ScoreBG.png"));					//计分板背景
	//可交互
	loadimage(&Key_Smile, _T("./images/Key-Smile/Key-Smile.png"));						//按钮-笑脸
	loadimage(&Key_Smile_putDown, _T("./images/Key-Smile/Key-Smlie-putDown.png"));		//按钮-笑脸-按下
	loadimage(&Key_Smile_O, _T("./images/Key-Smile/Key-Smile-O.png"));					//按钮-笑脸-张嘴
	loadimage(&Key_Smile_Win, _T("./images/Key-Smile/Key-Smile-Win.png"));				//按钮-笑脸-胜利
	loadimage(&Key_Smile_Fail, _T("./images/Key-Smile/Key-Smile-Fail.png"));			//按钮-笑脸-失败
	//雷块
	loadimage(&Default, _T("./images/MineState/Default.png"));				 //默认  16 * 16 面积
	loadimage(&putDown, _T("./images/MineState/putDown.png"));
	//计分板数字
	loadimage(img_Score + 0, _T("./images/ScoreNum/0.png"));		//11 * 21
	loadimage(img_Score + 1, _T("./images/ScoreNum/1.png"));
	loadimage(img_Score + 2, _T("./images/ScoreNum/2.png"));
	loadimage(img_Score + 3, _T("./images/ScoreNum/3.png"));
	loadimage(img_Score + 4, _T("./images/ScoreNum/4.png"));
	loadimage(img_Score + 5, _T("./images/ScoreNum/5.png"));
	loadimage(img_Score + 6, _T("./images/ScoreNum/6.png"));
	loadimage(img_Score + 7, _T("./images/ScoreNum/7.png"));
	loadimage(img_Score + 8, _T("./images/ScoreNum/8.png"));
	loadimage(img_Score + 9, _T("./images/ScoreNum/9.png"));
	loadimage(img_Score + 10, _T("./images/ScoreNum/-.png"));		//负号
	//雷标签
	loadimage(&Flag, _T("./images/MineSign/Flag.png"));
	loadimage(&Mine, _T("./images/MineSign/Mine.png"));
	loadimage(&Question_Mark, _T("./images/MineSign/Question-Mark.png"));
	//雷块数字
	loadimage(img_Number + 1, _T("./images/MineNum/N1.png"));
	loadimage(img_Number + 2, _T("./images/MineNum/N2.png"));
	loadimage(img_Number + 3, _T("./images/MineNum/N3.png"));
	loadimage(img_Number + 4, _T("./images/MineNum/N4.png"));
	loadimage(img_Number + 5, _T("./images/MineNum/N5.png"));
	loadimage(img_Number + 6, _T("./images/MineNum/N6.png"));
	loadimage(img_Number + 7, _T("./images/MineNum/N7.png"));
	loadimage(img_Number + 8, _T("./images/MineNum/N8.png"));
}

//游戏数据初始化
void initMap(int map[COL][ROW])
{
	//把map清零，释放内存
	memset(map, 0, ROW * COL * sizeof(int));
	//给地图随机埋下99个雷，雷用 -1 表示
	for (int i = 0; i < MINE_NUM; )
	{
		//(rand() + clock())
		int r = rand() % COL; //区间 [0; 30)
		int c = rand() % ROW; //区间 [0; 16)  因为数组的下标索引从 0 开始。
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;			//当雷生成时，i 计数才自增。
		}
	}
	//雷所在的九宫格(雷居中)所有的格子数都 + 1  (雷除外)
	for (int i = 0; i < COL; i++)
	{
		for (int k = 0; k < ROW; k++)
		{
			if (map[i][k] == -1)
			{
				//遍历雷所在的九宫格
				for (int r = i - 1; r <= i + 1; r++)
				{
					for (int c = k - 1; c <= k + 1; c++)
					{
						//注意数组越界问题
						if ((r >= 0 && r < COL && c >= 0 && c < ROW) && map[r][c] != -1)
						{
							map[r][c]++;
						}
					}
				}
			}
		}
	}

	//把所有格子都用遮罩图盖起来，给每个格子都加密，比如都加上 20
	for (int i = 0; i < COL; i++)
	{
		for (int k = 0; k < ROW; k++)
		{
			map[i][k] += 20;
		}
	}
}

//数组地图
void showMap(int map[COL][ROW], int score[3], int timeFree[3])
{
	std::cout << "Score Label:" << std::endl;
	for (int i = 0; i < 3; i++)
	{
		std::cout << score[i] << " ";
	}
	std::cout << "\t\t\t";
	for (int i = 0; i < 3; i++)
	{
		std::cout << timeFree[i] << " ";
	}
	std::cout << "s" << std::endl;
	std::cout << "Arrays Date:" << std::endl;
	for (int i = 0; i < COL; i++)
	{
		for (int k = 0; k < ROW; k++)
		{
			if (map[i][k] == -1)
			{
				std::cout << map[i][k] << " ";
			}
			else
			{
				std::cout << " " << map[i][k] << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//游戏绘制
void gameDraw(int map[COL][ROW])
{
	int left = 13;
	int top = 13;
	int right = 446;
	//绘制场景
	putimage(0, 0, &BackGround); //500 * 319 面积					背景
	putimage(8, 8, &HeadBG); //486 * 37 面积							顶部背景
	putimage(MAP_WIDTH / 2 - 14, 13, &Key_Smile); //26 * 26 面积		笑脸
	putimage(8, 50, &MineBG); //486 * 262							雷区背景
	putimage(left, top, &ScoreBG); //41 * 25						计分板背景（左）
	putimage(right, top, &ScoreBG); //X:500 - 13 - 41				计分板背景（右）
	//雷块底绘制
	for (int i = 52; i < 307; i += 16) //高 / 16   262/16
	{
		for (int j = 10; j < 486; j += 16) //宽 / 30   486/30
		{
			putimage(j, i, &putDown);
		}
	}
	////计分板绘制(模拟时使用，无实际意义)
	//	//左计分板
	//for (int i = 0; i < 3; i++)
	//{
	//	putimage(left + 2, top + 2, img_Score + 9);
	//	left += 13;
	//}
	//	//右计分板
	//for (int i = 0; i < 3; i++)
	//{
	//	putimage(right + 2, top + 2, img_Score + 0);
	//	right += 13;
	//}
}

//计分板加载
void scoreLabel(int score[3])
{
	int left = 13;
	int top = 13;
	int right = 446;
	//计分板数字
	for (int i = 0; i < 3; i++)
	{
		if (score[0] < 0 && i == 0)		//负号显示
		{
			putimage(left + 2, top + 2, img_Score + 10);
		}
		else if (i == 0)	//百位数
		{
			putimage(left + 2, top + 2, img_Score + score[i]);
		}
		else if (i == 1)	//十位数
		{
			putimage(left + 2, top + 2, img_Score + score[i]);
		}
		else if (i == 2)	//个位数
		{
			putimage(left + 2, top + 2, img_Score + score[i]);
		}
		left += 13;
	}
}

//游戏时间加载
void timeLabel(int timeFree[3])
{
	int left = 13;
	int top = 13;
	int right = 446;
	//计分板数字
	for (int i = 0; i < 3; i++)
	{
		if (i == 0)			//百位数
		{
			putimage(right + 2, top + 2, img_Score + timeFree[i] % 10);
		}
		else if (i == 1)	//十位数
		{
			putimage(right + 2, top + 2, img_Score + timeFree[i] % 10);
		}
		else if (i == 2)	//个位数
		{
			putimage(right + 2, top + 2, img_Score + timeFree[i] % 10);
		}
		right += 13;
	}
}

//雷图像加载
void mineLoad(int map[COL][ROW])
{
	//雷块绘制
	for (int i = 0; i < COL; i++)
	{
		for (int k = 0; k < ROW; k++)
		{
			//空白格子和数字
			if (map[i][k] > 0 && map[i][k] <= 8)	//数字
			{
				transparentimg(k * IMGW + 13, i * IMGW + 55, img_Number + map[i][k]);
			}
			else if (map[i][k] >= 19 && map[i][k] <= 28)	//默认雷块
			{
				putimage(k * IMGW + 11, i * IMGW + 53, &Default);
			}
			else if (map[i][k] == -1)	//雷块
			{
				putimage(k * IMGW + 10, i * IMGW + 52, &putDown);
				transparentimg(k * IMGW + 12, i * IMGW + 54, &Mine);
			}
			else if (map[i][k] == -2)	//红雷
			{
				putimage(k * IMGW + 10, i * IMGW + 52, &putDown);
				setimgbkcolor(k * IMGW + 12, i * IMGW + 54, &Mine, 255, 0, 30);
			}
			else if (map[i][k] >= 29 && map[i][k] <= 38)	//旗帜
			{
				putimage(k * IMGW + 11, i * IMGW + 53, &Default);
				transparentimg(k * IMGW + 13, i * IMGW + 55, &Flag);
			}
			else if (map[i][k] == 50)	//粉色旗帜
			{
				putimage(k * IMGW + 11, i * IMGW + 53, &Default);
				setimgbkcolor(k * IMGW + 13, i * IMGW + 55, &Flag, 0, 8, 0);
			}
			else if (map[i][k] >= 39 && map[i][k] <= 49)	//问号
			{
				putimage(k * IMGW + 11, i * IMGW + 53, &Default);
				transparentimg(k * IMGW + 13, i * IMGW + 55, &Question_Mark);
			}
		}
	}
}

//openNull声明
void openNull(int map[COL][ROW], int row, int col);
//judge声明
void judge(int map[COL][ROW], int row, int col);

//鼠标操作
void mouseEvent(int map[COL][ROW], int score[3], int timeFree[3])
{
	while (true)
	{
		//定义消息结构体(鼠标消息，按键消息，字符消息)
		ExMessage msg;
		//如果有消息就会返回true，并且给msg赋值
		if (peekmessage(&msg, EX_MOUSE))
		{
			//获取鼠标点击坐标
			int x = msg.x;
			int y = msg.y;
			//把鼠标的坐标转成数组的下标
			int c = (msg.x - 12) / IMGW;
			int r = (msg.y - 54) / IMGW;
			if (msg.message == WM_LBUTTONDOWN)	//鼠标左键按下
			{
				//笑脸张嘴
				putimage(MAP_WIDTH / 2 - 14, 13, &Key_Smile_O);
				if (x >= 10 && y >= 52 && x <= 486 && y <= 307)
				{
					//打开雷块
					if (map[r][c] >= 19 && map[r][c] <= 28 || map[r][c] >= 39 && map[r][c] <= 49)
					{
						timer = true;
						if (map[r][c] == 19)
						{
							map[r][c] -= 21;
						}
						else if (map[r][c] >= 19 && map[r][c] <= 28)
						{
							map[r][c] -= 20;
						}
						else if (map[r][c] >= 39 && map[r][c] <= 49)
						{
							map[r][c] -= 40;
							if (map[r][c] == -1)
							{
								map[r][c] -= 1;
							}
						}
						openNull(map, r, c);
						judge(map, r, c);
						showMap(map, score, timeFree);
						break;
					}
				}

				//笑脸互动 //笑脸坐标:  MAP_WIDTH / 2 - 14, 13,
				else if (x >= MAP_WIDTH / 2 - 14 && y >= 13 && x <= MAP_WIDTH / 2 + 14 && y <= 39)
				{
					putimage(MAP_WIDTH / 2 - 14, 13, &Key_Smile_putDown);
					Sleep(150);	//暂停 0.15 秒
					putimage(MAP_WIDTH / 2 - 14, 13, &Key_Smile);
					initMap(map);
					//重置计分板显示
					for (int i = 0; i < 3; i++)
					{
						score[0] = MINE_NUM / 100;
						score[1] = (MINE_NUM / 10) % 10;
						score[2] = MINE_NUM % 10;
					}
					scoreLabel(score);
					system("cls");
					//重置计时显示
					timer = false;
					for (int i = 0; i < 3; i++)
					{
						timeFree[0] = 0;
						timeFree[1] = 0;
						timeFree[2] = 0;
					}
					timeLabel(timeFree);
					showMap(map, score, timeFree);
					break;
				}
			}
			//鼠标右键点下
			else if (msg.message == WM_RBUTTONDOWN)	//鼠标右键点下
			{
				if (map[r][c] >= 19 && map[r][c] <= 28)
				{
					map[r][c] += 10;
					//插旗与计分板减法计数关系
						//0与-1的递接
					if (score[0] == 0 && score[1] == 0 && score[2] == 0)
					{
						score[0]--;
					}
						//负数
					if (score[0] < 0)
					{
						if (score[2] >= 0 && score[2] < 9)
						{
							score[2]++;
						}
						else if (score[1] == 9 && score[2] == 9)	//如果值为-99，就退出循环。
						{
							break;
						}
						else
						{
							score[1]++;
							score[2] = 0;
						}
					}
						//正数
					else if (score[2] > 0 && score[2] <= 9)
					{
						score[2]--;
					}
					else
					{
						if (score[1] > 0 && score[1] <= 9)
						{
							score[1]--;
							if (score[1] >= 0)
							{
								score[2] = 9;
							}
						}
						else
						{
							score[0]--;
							if (score[0] >= 0)
							{
								score[1] = 9;
								score[2] = 9;
							}
						}
					}
					scoreLabel(score);
					//judge(map, r, c);		//如果打开此过程要把judge里的注释代码打开
					showMap(map, score, timeFree);
					break;
				}
				else if (map[r][c] >= 29 && map[r][c] <= 38)
				{
					map[r][c] += 10;
					//拔旗与计分板加法计数关系
						//负数
					if (score[0] < 0)
					{
						if (score[1] == 0 && score[2] == 1)
						{
							score[0]++;
							score[2]--;
						}
						else if (score[2] > 0 && score[2] <= 9)
						{
							score[2]--;
						}
						else
						{
							if (score[1] > 0 && score[1] <=9)
							{
								score[1]--;
								score[2] = 9;
							}
						}
					}
						//正数
					else if (score[0] >= 0 && score[1] >= 0 && score[2] >= 0)
					{
						if (score[2] >= 0 && score[2] < 9)
						{
							score[2]++;
						}
						else
						{
							if (score[1] >= 0 && score[1] < 9)
							{
								score[1]++;
								score[2] = 0;
							}
							else
							{
								score[0]++;
								score[1] = 0;
								score[2] = 0;
							}
						}
					}
					else
					{
						score[0]++;
					}
					scoreLabel(score);
					showMap(map, score, timeFree);
					break;
				}
				else if (map[r][c] >= 39 && map[r][c] <= 49)
				{
					map[r][c] -= 20;
					showMap(map, score, timeFree);
					break;
				}
			}
		}
		//张嘴时间
		if (clock() % 150 == 0)
		{
			putimage(MAP_WIDTH / 2 - 14, 13, &Key_Smile);
		}
		//停表计时
		if (timer)
		{
			if (clock() % 1000 == 0)
			{
				//时间一到，game over.
				if (timeFree[0] == 9 && timeFree[1] == 9 && timeFree[2] == 9)
				{
					isOver = true;
					break;
				}
				//计时版显示
				else if (timeFree[2] >= 0 && timeFree[2] < 9)
				{
					timeFree[2]++;
				}
				else
				{
					if (timeFree[1] >= 0 && timeFree[1] < 9)
					{
						timeFree[1]++;
						timeFree[2] = 0;
					}
					else
					{
						timeFree[0]++;
						timeFree[1] = 0;
						timeFree[2] = 0;
					}
				}
				timeLabel(timeFree);
				showMap(map, score, timeFree);	//一秒计时器
			}
		}
	}
}

//锁链空白全打开
void openNull(int map[COL][ROW], int row, int col)
{
	//判断一下当前点击的是不是空白
	if (map[row][col] == 0)
	{
		//遍历以它为中心的九宫格
		for (int i = row - 1; i <= row + 1; i++)
		{
			for (int k = col - 1; k <= col + 1; k++)
			{
				if ((i >= 0 && i < COL && k >= 0 && k < ROW) && map[i][k] >= 19 && map[i][k] <= 28)
				{
					map[i][k] -= 20;
					openNull(map, i, k);	//递归
				}
			}
		}
	}
}

//判断游戏结束
void judge(int map[COL][ROW], int row, int col)
{
	int curOpenGridcnt = 0;
	int flagMine = 0;
	//如果点到了雷，就game over 点一个雷，其他的雷都显示出来
	if (map[row][col] == -2)	/*map[row][col] == -1 ||*/
	{
		for (int i = 0; i < COL; i++)
		{
			for (int k = 0; k < ROW; k++)
			{
				if (map[i][k] == 19)
				{
					map[i][k] -= 20;
				}
				else if (map[i][k] > 29 && map[i][k] <= 38)	//map[i][k] != 29
				{
					map[i][k] = 50;
				}
			}
		}
		putimage(MAP_WIDTH / 2 - 14, 13, &Key_Smile_Fail);
		isOver = true;
	}
	//赢了，应该点开的格子都点开了，就算是胜利了 curOpenGridcnt == ROW * COL - MINE_NUM
	else if (map[row][col] != -1)
	{
		//遍历雷块
		for (int i = 0; i < COL; i++)
		{
			for (int k = 0; k < ROW; k++)
			{
				if (curOpenGridcnt == WIN_NUM /*|| flagMine == MINE_NUM*/)
				{
					break;
				}
				else if (map[i][k] == 0 || map[i][k] == 1 || map[i][k] == 2 || map[i][k] == 3 || map[i][k] == 4 || map[i][k] == 5 || map[i][k] == 6 || map[i][k] == 7 || map[i][k] == 8)
				{
					curOpenGridcnt++;
					std::cout << "curOpenGridcnt: ";
					std::cout << curOpenGridcnt << " " << std::endl;
				}
				/*else if (map[i][k] == 29)
				{
					flagMine++;
					cout << "flagMine：";
					cout << flagMine << " " << endl;
				}*/
			}
			if (curOpenGridcnt == WIN_NUM /*|| flagMine == MINE_NUM*/)
			{
				break;
			}
		}
		std::cout << std::endl;
		if (curOpenGridcnt == WIN_NUM)
		{
			//雷块底绘制
			for (int i = 52; i < 307; i += 16) //高 / 16   262/16
			{
				for (int j = 10; j < 486; j += 16) //宽 / 30   486/30
				{
					putimage(j, i, &putDown);
				}
			}
			//雷块插旗
			for (int i = 0; i < COL; i++)
			{
				for (int k = 0; k < ROW; k++)
				{
					if (map[i][k] == 19)
					{
						map[i][k] = 29;
					}
				}
			}
			putimage(MAP_WIDTH / 2 - 14, 13, &Key_Smile_Win);
			isWin = true;
		}
		////插旗胜利
		//else if (flagMine == MINE_NUM)
		//{
		//	for (int i = 0; i < COL; i++)
		//	{
		//		for (int k = 0; k < ROW; k++)
		//		{
		//			if (map[i][k] != -1 && map[i][k] != 29)
		//			{
		//				map[i][k] = 0;
		//			}
		//		}
		//	}
		//	// 雷块底绘制
		//	for (int i = 52; i < 307; i += 16) //高 / 16   262/16
		//	{
		//		for (int j = 10; j < 486; j += 16) //宽 / 30   486/30
		//		{
		//			putimage(j, i, &putDown);
		//		}
		//	}
		//	isWin = true;
		//}
	}
}

//游戏结束
void showJudge(int map[COL][ROW], int score[3], int timeFree[3])
{
	if (isOver)
	{
		//定义按键内存指针
		ExMessage R;
		//创建MessageBox窗口
		int ret = MessageBox(GetHWnd(), _T("\tYOU ARE FAILED,\n\tTRY AGAIN?"), _T("FAIL"), MB_RETRYCANCEL | MB_ICONERROR);
		if (ret == IDRETRY || peekmessage(&R, EX_KEY == ('r' || 'R')))	//按键R(r)或space
		{
			initMap(map);
			gameDraw(map);
			//雷回归定义
			for (int i = 0; i < 3; i++)
			{
				score[0] = MINE_NUM / 100;
				score[1] = (MINE_NUM / 10) % 10;
				score[2] = MINE_NUM % 10;
			}
			scoreLabel(score);
			//重置计时显示
			timer = false;
			for (int i = 0; i < 3; i++)
			{
				timeFree[0] = 0;
				timeFree[1] = 0;
				timeFree[2] = 0;
			}
			system("cls");	//清除输出界面
			timeLabel(timeFree);
			showMap(map, score, timeFree);
			isOver = false;
		}
		else if (ret == IDCANCEL)
		{
			exit(666);
		}
	}
	else if (isWin)
	{
		//创建MessageBox窗口
		int rets = MessageBox(GetHWnd(), _T("\tYOU WIN,\n\tTHE MINESWEEPER EXPECT TO SEE YOU AGAIN!\n\n\tGo to live every minute of it."), _T("WIN"), MB_OK | MB_ICONINFORMATION);
		if (rets == IDOK)
		{
			exit(114514);
		}
	}
}

//主程序
int main()
{
	//绘制图形窗口
	initgraph(MAP_WIDTH, MAP_HEIGHT/*, EX_SHOWCONSOLE*/);
	//setbkcolor(WHITE);
	cleardevice(); //刷新背景
	/*
		srand函数设定随机数种子，就是随机函数rand所根据的参数。
		而rand函数是随机的，因此需要对函数进行运算来得到自己所需要的数据范围。
	*/
	//设置随机数种子
	srand((unsigned)time(NULL));
	//定义计分板数组
	int score[3] = { MINE_NUM / 100, (MINE_NUM / 10) % 10, MINE_NUM % 10 };
	int timeFree[3] = { 0 };
	//定义地图数组
	int map[COL][ROW] = { 0 };
	//初始化数据
	initMap(map);
	//把数据转成对应的图形界面
	loadPicture();
	//把数组里面的数据，转换成对应的图片，贴到图形窗口上
	gameDraw(map);
	timeLabel(timeFree);
	scoreLabel(score);
	showMap(map, score, timeFree);
	//操作系统
	while (true)
	{
		//雷块底绘制
		for (int i = 52; i < 307; i += 16) //高 / 16   262/16
		{
			for (int j = 10; j < 486; j += 16) //宽 / 30   486/30
			{
				putimage(j, i, &putDown);
			}
		}
		mineLoad(map);
		mouseEvent(map, score, timeFree);
		mineLoad(map);
		showJudge(map, score, timeFree);
	}
	//getchar();
	closegraph(); //关闭图形窗口
	return 0;
}