#include <windows.h>
#include <iostream>
#include <ctime>
#include <conio.h>
#include<iomanip>

using namespace std;

// https://play.google.com/store/apps/details?id=com.kadechkin.games.meze

void main()
{
	srand(time(0)); rand();

	system("mode con cols=80 lines=40");
	system("title Super Maze!");

	// hide standard console cursor
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 100;

	SetConsoleCursorInfo(h, &cci);

	const int width = 60;
	const int height = 20;
	int maze[height][width];

	// http://www.quizful.net/post/enum-types-c

	enum maze_objects { HALL, WALL, GOLD, VRAG, MEDICINE, ENERGY };
	enum direction { DOWN = 80, UP = 72, LEFT = 75, RIGHT = 77 };
	enum colors { BLUE = 9, RED = 12, YELLOW = 14, DARKGREEN = 2, LIGHTBLUE = 11, WHITE = 7, PURPLE = 13 };

	int coins = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int random = rand() % 6;
			maze[y][x] = random;

			if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
				maze[y][x] = WALL;
			//
			//// enter and exit
			if (x == 0 && y == 2 || x == 1 && y == 2
				|| x == width - 1 && y == height - 3
				|| x == width - 2 && y == height - 3)
				maze[y][x] = HALL;

			/////////////////////////////////////////////////////////

			if (maze[y][x] == WALL)
			{
				SetConsoleTextAttribute(h, DARKGREEN);
				cout << (char)178;
			}
			else if (maze[y][x] == GOLD)
			{
				SetConsoleTextAttribute(h, YELLOW);
				cout << (char)250;
				coins++;
			}
			//else if (maze[y][x] == VRAG)
			//{
			//	int r = rand() % 100; // 0...9
			//	if (r == 0)
			//	{
			//		SetConsoleTextAttribute(h, RED);
			//		cout << (char)1;
			//	}
			//	else
			//	{
			//		maze[y][x] = HALL;
			//		cout << " ";
			//	}
			//}
			else if (maze[y][x] == MEDICINE)
			{
				int t = rand() % 100; // 0...9
				if (t > 0 && t <= 5)
				{
					SetConsoleTextAttribute(h, LIGHTBLUE);
					cout << (char)2;
				}
				else
				{
					maze[y][x] = HALL;
					cout << " ";
				}
			}
			else if (maze[y][x] == ENERGY)
			{
				int t = rand() % 100; // 0...9
				if (t > 5 && t <= 11)
				{
					SetConsoleTextAttribute(h, PURPLE);
					cout << (char)3;
				}
				else
				{
					maze[y][x] = HALL;
					cout << " ";
				}
			}
			else
			{
				cout << " ";
			}

		}
		cout << "\n";
	}

	COORD pers = { 0, 2 };
	SetConsoleCursorPosition(h, pers);
	SetConsoleTextAttribute(h, BLUE);
	cout << (char)1;

	unsigned int gold_count = 0;
	COORD statistic = { 63, 1 };

	SetConsoleCursorPosition(h, statistic);
	SetConsoleTextAttribute(h, YELLOW);
	cout << "Gold count: " <<setw(3) << gold_count;

	int health_points = 80;
	COORD health = { 63, 3 };
	SetConsoleCursorPosition(h, health);
	SetConsoleTextAttribute(h, LIGHTBLUE);
	cout << "Health:" << setw(3)  << health_points;

	int energy_points = 500;
	COORD energy = { 63, 5 };
	SetConsoleCursorPosition(h, energy);
	SetConsoleTextAttribute(h, PURPLE);
	cout << "Energy:" << setw(3) << energy_points;

	const int start_enemy_count = 10;

	int enemy_count = 0;

	COORD enemies[start_enemy_count] = {};
	DWORD enemy_times[start_enemy_count] = {};

	while (enemy_count < start_enemy_count)
	{
		short y = rand() % (height - 2) + 1;
		short x = rand() % (width - 2) + 1;
		//if (pers.X == x && pers.Y == y)
		//{
		//	health_points -= 25;
		//}
		if (maze[y][x] == HALL || pers.X == x && pers.Y == y)
		{
			continue;
		}
		enemies[enemy_count].X = x;
		enemies[enemy_count].Y = y;
		enemy_times[enemy_count] = GetTickCount();

		SetConsoleCursorPosition(h, enemies[enemy_count]);
		SetConsoleTextAttribute(h, RED);
		cout << (char)1;

		enemy_count++;
	}
	while (true)
	{
		if (_kbhit())
		{
			int direct = _getch();
			if (direct == 224)
				direct = _getch();

			SetConsoleCursorPosition(h, pers);
			cout << " ";
			if (maze[pers.Y][pers.X] == MEDICINE)
			{
				if (health_points == 100)
				{
					SetConsoleCursorPosition(h, pers);
					SetConsoleTextAttribute(h, LIGHTBLUE);
					cout << (char)2;
				}
				else
				{
					health_points += 5;
					SetConsoleCursorPosition(h, pers);
					cout << " ";
				}
			}
			if (maze[pers.Y][pers.X] == ENERGY)
			{
					energy_points += 25;
					SetConsoleCursorPosition(h, pers);
					cout << " ";
			}

			if (direct == RIGHT && maze[pers.Y][pers.X + 1] != WALL)
			{
				pers.X++;
				energy_points--;
			}
			if (direct == LEFT && maze[pers.Y][pers.X - 1] != WALL)
			{
				pers.X--;
				energy_points--;
			}
			if (direct == UP && maze[pers.Y - 1][pers.X] != WALL)
			{
				pers.Y--;
				energy_points--;
			}
			if (direct == DOWN && maze[pers.Y + 1][pers.X] != WALL)
			{
				pers.Y++;
				energy_points--;
			}
			if (maze[pers.Y][pers.X] == GOLD)
			{
				gold_count++;
			}

			if (pers.X == width - 1 && pers.Y == height - 3)
			{
				system("cls");
				SetConsoleTextAttribute(h, WHITE);
				MessageBox(NULL, "      ПОБЕДА!\n  Найден выход!", "Победа!", MB_OK);
				break;
			}
			if (gold_count == coins)
			{
				system("cls");
				SetConsoleTextAttribute(h, WHITE);
				MessageBox(NULL, "      ПОБЕДА!\n  Биткоины собраны!", "Победа!", MB_OK);
				break;
			}
			if (health_points <= 0)
			{
				system("cls");
				SetConsoleTextAttribute(h, WHITE);
				MessageBox(NULL, "      Game Over!\n", "Вы проиграли!", MB_OK);
				break;
			}
			if (energy_points == 0)
			{
				system("cls");
				SetConsoleTextAttribute(h, WHITE);
				MessageBox(NULL, "      Поражение!\n", "Закончилась энергия!", MB_OK);
				break;
			}
			SetConsoleCursorPosition(h, energy);
			SetConsoleTextAttribute(h, PURPLE);
			cout << "Energy:" << setw(3) << energy_points;

			SetConsoleCursorPosition(h, health);
			SetConsoleTextAttribute(h, LIGHTBLUE);
			cout << "Health:" << setw(3) << health_points;

			SetConsoleCursorPosition(h, pers);
			SetConsoleTextAttribute(h, BLUE);
			cout << (char)1;

			SetConsoleCursorPosition(h, statistic);
			SetConsoleTextAttribute(h, YELLOW);
			cout << "Gold count: " << setw(3) << gold_count;


			char gold[200] = {};
			_itoa_s(gold_count, gold, 199, 10);
		}
		//SetConsoleTitle(gold);

		for (int i = 0; i < start_enemy_count; i++)
		{
			if (GetTickCount() - enemy_times[i] > 500)
			{

				SetConsoleCursorPosition(h, enemies[i]);
				cout << " ";

				if (maze[enemies[i].Y][enemies[i].X] == ENERGY)
				{
					SetConsoleCursorPosition(h, enemies[i]);
					SetConsoleTextAttribute(h, PURPLE);
					cout << (char)3;
				}
				if (maze[enemies[i].Y][enemies[i].X] == GOLD)
				{
					SetConsoleCursorPosition(h, enemies[i]);
					SetConsoleTextAttribute(h, YELLOW);
					cout << (char)250;
				}
				if (maze[enemies[i].Y][enemies[i].X] == MEDICINE)
				{
					SetConsoleCursorPosition(h, enemies[i]);
					SetConsoleTextAttribute(h, LIGHTBLUE);
					cout << (char)2;
				}

				int move_rand = rand() % 5;
				switch (move_rand)
				{
				case 1:
				{
					if (maze[enemies[i].Y][enemies[i].X + 1] != WALL)
					{
						enemies[i].X++;
					}
					break;
				}
				case 2:
				{
					if (maze[enemies[i].Y][enemies[i].X - 1] != WALL)
					{
						enemies[i].X--;
					}
					break;
				}
				case 3:
				{
					if (maze[enemies[i].Y + 1][enemies[i].X] != WALL)
					{
						enemies[i].Y++;
					}
					break;
				}
				case 4:
				{
					if (maze[enemies[i].Y - 1][enemies[i].X] != WALL)
					{
						enemies[i].Y--;
					}
					break;
				}
				}

				enemy_times[i] = GetTickCount();

				SetConsoleCursorPosition(h, enemies[i]);
				SetConsoleTextAttribute(h, RED);
				cout << (char)1;

				SetConsoleCursorPosition(h, statistic);
				SetConsoleTextAttribute(h, YELLOW);
				cout << "Gold count: " << setw(3) << gold_count;

				SetConsoleCursorPosition(h, health);
				SetConsoleTextAttribute(h, LIGHTBLUE);
				cout << "Health:" << setw(3) << health_points;




			}
		}

	}

}

////////////////////////////////////////////////////////////////////
// как отправить значение интовой переменной в заголовок консоли: //
////////////////////////////////////////////////////////////////////


//#include <windows.h>
//#include <iostream>
//using namespace std;
//
//void main()
//{
//	int coins = 25;
//
//	// string representation of coins variable
//	char* title = new char[200];
//	_itoa_s(coins, title, 199, 10);
//
//	SetConsoleTitleA(title);
//
//	system("pause");
//}
