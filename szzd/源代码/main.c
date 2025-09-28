#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 函数声明
void init_game(int* min, int* max, int* bomb_num);
int get_legal_guess(int min, int max, int player_idx);
int judge_result(int guess, int bomb, int* min, int* max);
int ask_replay();
int rock_paper_scissors();
int choose_robot_mode();
void offline_multiplayer(int* min, int* max, int bomb_num);

int main() {
	srand((unsigned int)time(NULL));
	int min, max, bomb_num;
	int user_guess, comp_guess;
	int is_over = 0;
	int is_player_turn = 0;
	int replay = 0;
	int robot_mode = 0;
	
	do {
		robot_mode = choose_robot_mode();
		init_game(&min, &max, &bomb_num);
		is_over = 0;
		
		if (robot_mode == 1) {
			// 机器人模式：单人vs电脑
			is_player_turn = rock_paper_scissors();
			while (is_over == 0) {
				if (is_player_turn == 1) {
					printf("\n===== 你的回合 =====");
					user_guess = get_legal_guess(min, max, 1);
					if (judge_result(user_guess, bomb_num, &min, &max) == 1) {
						printf("你踩雷！电脑胜！\n");
						is_over = 1;
					} else {
						is_player_turn = 0;
					}
				} else {
					printf("\n===== 电脑回合 =====");
					comp_guess = (min + max) / 2;
					printf("当前范围[%d-%d]，电脑猜：%d\n", min, max, comp_guess);
					if (judge_result(comp_guess, bomb_num, &min, &max) == 1) {
						printf("电脑踩雷！你胜！\n");
						is_over = 1;
					} else {
						is_player_turn = 1;
					}
				}
			}
		} else {
			// 非机器人模式：线下多人
			printf("\n===== 线下多人模式！猜中炸弹者淘汰 =====\n");
			offline_multiplayer(&min, &max, bomb_num);
		}
		
		replay = ask_replay();
	} while (replay == 1);
	
	printf("\n游戏结束，再见！\n");
	return 0;
}

// 选择是否开启机器人
int choose_robot_mode() {
	int choice;
	printf("===== 猜拳数字炸弹 =====\n");
	while (1) {
		printf("需机器人（电脑）当对手？1=是，0=否（线下多人）：");
		if (scanf("%d", &choice) != 1 || (choice != 0 && choice != 1)) {
			printf("输入错！输1或0！\n");
			while (getchar() != '\n');
			continue;
		}
		return choice;
	}
}

// 线下多人逻辑
void offline_multiplayer(int* min, int* max, int bomb_num) {
	int player_count = 0;
	int current_player = 1;
	int guess;
	int is_over = 0;
	
	// 输入玩家人数
	while (1) {
		printf("输入玩家人数（≥2）：");
		if (scanf("%d", &player_count) != 1 || player_count < 2) {
			printf("输入错！人数≥2！\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	
	// 轮流猜数
	while (is_over == 0) {
		printf("\n===== 玩家%d回合 =====", current_player);
		guess = get_legal_guess(*min, *max, current_player);
		
		if (judge_result(guess, bomb_num, min, max) == 1) {
			printf("玩家%d踩雷！淘汰！\n", current_player);
			player_count--;
			if (player_count == 1) {
				printf("恭喜玩家%d获胜！\n", (current_player % (player_count + 1)) + 1);
				is_over = 1;
			}
		}
		
		// 切换玩家
		current_player++;
		if (current_player > (player_count + (is_over ? 0 : 1))) {
			current_player = 1;
		}
	}
}

// 初始化游戏：范围1-100，生成炸弹
void init_game(int* min, int* max, int* bomb_num) {
	*min = 1;
	*max = 100;
	*bomb_num = rand() % (*max - *min + 1) + *min;
	printf("===== 炸弹生成！范围[%d-%d] =====\n", *min, *max);
}

// 获取合法猜数（适配多人）
int get_legal_guess(int min, int max, int player_idx) {
	int guess;
	while (1) {
		printf("\n玩家%d，猜[%d-%d]的数：", player_idx, min, max);
		if (scanf("%d", &guess) != 1) {
			printf("输入错！输数字！\n");
			while (getchar() != '\n');
			continue;
		}
		if (guess < min || guess > max) {
			printf("超范围！猜[%d-%d]！\n", min, max);
			continue;
		}
		return guess;
	}
}

// 判定猜数结果
int judge_result(int guess, int bomb, int* min, int* max) {
	if (guess == bomb) {
		return 1;
	} else if (guess > bomb) {
		printf("猜大了！范围更新为[%d-%d]\n", *min, guess - 1);
		*max = guess - 1;
		return 0;
	} else {
		printf("猜小了！范围更新为[%d-%d]\n", guess + 1, *max);
		*min = guess + 1;
		return 0;
	}
}

// 询问是否重玩
int ask_replay() {
	int choice;
	while (1) {
		printf("\n再来一局？1=是，0=否：");
		if (scanf("%d", &choice) != 1 || (choice != 0 && choice != 1)) {
			printf("输入错！输1或0！\n");
			while (getchar() != '\n');
			continue;
		}
		return choice;
	}
}

// 石头剪刀布定先后（仅机器人模式用）
int rock_paper_scissors() {
	int player_choice, comp_choice;
	printf("\n===== 石头剪刀布：定猜数先后 =====\n");
	
	while (1) {
		// 玩家出拳
		while (1) {
			printf("出拳：1=石头，2=剪刀，3=布：");
			if (scanf("%d", &player_choice) != 1 || player_choice < 1 || player_choice > 3) {
				printf("出拳错！输1、2或3！\n");
				while (getchar() != '\n');
				continue;
			}
			break;
		}
		
		// 电脑出拳
		comp_choice = rand() % 3 + 1;
		printf("电脑出拳：");
		switch (comp_choice) {
			case 1: printf("石头\n"); break;
			case 2: printf("剪刀\n"); break;
			case 3: printf("布\n"); break;
		}
		
		// 判定胜负
		if (player_choice == comp_choice) {
			printf("平局！再来！\n");
		} else if ((player_choice == 1 && comp_choice == 2) || 
			(player_choice == 2 && comp_choice == 3) || 
			(player_choice == 3 && comp_choice == 1)) {
			printf("你赢！你先猜！\n");
			return 1;
		} else {
			printf("电脑赢！电脑先猜！\n");
			return 0;
		}
	}
}
