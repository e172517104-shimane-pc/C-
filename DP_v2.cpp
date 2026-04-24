
#include "DxLib.h"


#define MAX_ST 100


int num_x = 0;		//一つ目の数字
int num_y = 0;		//二つ目の数字
int num_i = 0;		//文字数を管理
int calc_h = 0;		//現在の演算子を格納
int calc_old = 0;	//ひとつ前の演算子を格納
int div_0 = 0;		//0で割ったかのフラグ
int up_x_len = 0;	//num_xの小数点以下の長さ
int up_y_len = 0;	//num_yの小数点以下の長さ

//文字列の長さを返す関数
int str_len(char* snt_ary) {
	int num_len = 0;

	for (num_len = 0; 0 < snt_ary[num_len] && num_len < MAX_ST; num_len++) {

	}

	return num_len;
}

//10の引数乗を返す関数
int exp_10(int num_ex) {
	int num_re = 1;

	for (int i = 0; i < num_ex; i++) {
		num_re *= 10;
	}

	return num_re;
}

//画面の初期化
void init_d3() {

	unsigned int Cr;
	unsigned int x_set = 128;
	unsigned int y_set = 80;
	char string[10] = { "abcde" };
	int len_c;

	char c_set[4][5][4] = { {{"7"}, {"8"}, {"9"}, {"/"}, {"AC"}},
							{{"4"}, {"5"}, {"6"}, {"*"}, {"DEL"}},
							{{"1"}, {"2"}, {"3"}, {"-"}, {"%"}},
							{{"0"}, {"."}, {"="}, {"+"}, {0}} };


	Cr = GetColor(255, 255, 255);    // 白色の値を取得

	for (int i = 1; i < 5; i++) {
		DrawLine(x_set * i, 160, x_set * i, 480, Cr);    // 線を描画
	}

	for (int i = 2; i < 6; i++) {
		DrawLine(0, y_set * i, 640, y_set * i, Cr);    // 線を描画
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			len_c = str_len(c_set[i][j]);
			DrawFormatString(60 + 128 * j - 4 * (len_c - 1), 180 + 80 * i, Cr, "%s", c_set[i][j]);
		}
	}

	DrawString(60 + 128 * 4 - 5 * 2, 180 + 80 * 3, "終了", Cr);

	return;
}

void draw_disp(char* snt_ary, char* snt_ary2) {
	int num_c = 0;
	int wids = 0;
	unsigned int Cr;

	Cr = GetColor(255, 255, 255);

	init_d3();

	if (div_0 == 0) {
		// 0で割っていないとき
		num_c = str_len(snt_ary);
		wids = GetDrawStringWidth(snt_ary, num_c);
		DrawFormatString(600 - wids, 80, Cr, "%s", snt_ary);
		num_c = str_len(snt_ary2);
		wids = GetDrawStringWidth(snt_ary2, num_c);
		DrawFormatString(600 - wids, 60, Cr, "%s", snt_ary2);
	}
	else if (div_0 == 1) {
		// 0で割っているとき
		div_0 = 0;
		num_c = 0;
		for (int i = 0; i < MAX_ST; i++) {
			snt_ary[i] = 0;
		}

		DrawString(600 - 188, 80, "0で割ることはできません", Cr);
		num_c = str_len(snt_ary2);
		wids = GetDrawStringWidth(snt_ary2, num_c);
		DrawFormatString(600 - wids, 60, Cr, "%s", snt_ary2);
	}

	return;
}

//num_xを文字列の中に入れていく関数
void into_str3(char* snt_ary, int snt_start, int snt_fin) {

	int num_alt = num_x;
	int num_step = 0;
	int flag_0 = 0;

	//snt_ary 初期化
	for (int i = snt_start; i < snt_fin; i++) {
		snt_ary[i] = 0;
	}

	//num_alt を一桁ずつ入れていく
	//0のときにも1回は処理してほしいのでdo-while文を用いている
	do {
		//小数点以下に数字を入れたとき、または扱う桁が小数点を超えたときフラグを立てる
		if (num_alt % 10 > 0 || num_step >= up_x_len) {
			flag_0 = 1;
		}
		//フラグが立っているときnum_xの一桁目を文字列に入れる
		if (flag_0 == 1) {
			if (num_step < snt_fin - snt_start) {
				for (int i = snt_fin - 1; snt_start < i; i--) {
					snt_ary[i] = snt_ary[i - 1];
				}
			}
			else
			{
				for (int i = MAX_ST - 1; snt_start < i; i--) {
					snt_ary[i] = snt_ary[i - 1];
				}
			}
			snt_ary[snt_start] = num_alt % 10 + 48;
		}
		else
		{
			num_step--;
			up_x_len--;
			num_x /= 10;
		}
		//num_xの一桁目を削除
		num_alt = num_alt / 10;
		//扱っている桁数の情報をインクリメント
		num_step++;
		//扱っている桁が1の位のとき、かつ小数点以下に数字を入れていた時ピリオドを文字列に追加
		if (num_step == up_x_len && flag_0 == 1) {
			/**/
			if (num_step < snt_fin - snt_start) {
				for (int i = snt_fin - 1; snt_start < i; i--) {
					snt_ary[i] = snt_ary[i - 1];
				}
			}
			else {
				for (int i = MAX_ST - 1; snt_start < i; i--) {
					snt_ary[i] = snt_ary[i - 1];
				}
			}

			snt_ary[snt_start] = 46;
			num_step++;
		}

	} while (num_alt > 0 || num_step < up_x_len);

	//num_xが1より小さい場合先頭に 0 を付与する
	if (exp_10(up_x_len) > num_x) {
		/*
		for (int i = snt_fin - 1; snt_start < i; i--) {
			snt_ary[i] = snt_ary[i - 1];
		}
		*/
		if (num_step < snt_fin - snt_start) {
			for (int i = snt_fin - 1; snt_start < i; i--) {
				snt_ary[i] = snt_ary[i - 1];
			}
		}
		else {
			for (int i = MAX_ST - 1; snt_start < i; i--) {
				snt_ary[i] = snt_ary[i - 1];
			}
		}
		snt_ary[snt_start] = 48;
	}

	return;
}

//積、商があるか
int calc_checker(char* snt_ary) {
	int flag_calc = 0;

	for (int i = 0; i < MAX_ST && snt_ary[i]; i++) {
		if (snt_ary[i] == 42 || snt_ary[i] == 47) {
			flag_calc = 1;
			break;
		}
	}

	return flag_calc;
}

//和、差があるか
int calc_checker2(char* snt_ary) {
	int flag_calc = 0;

	for (int i = 0; i < MAX_ST && snt_ary[i]; i++) {
		if (snt_ary[i] == 43 || snt_ary[i] == 45) {
			flag_calc = 1;
			break;
		}
	}

	return flag_calc;
}

//空白を埋める
void snt_setter(char* snt_ary) {
	int snt_index = 0;//\0を発見した場所
	int flag_o = 0;//0の時while文を回す
	while (snt_index < MAX_ST && flag_o == 0) {
		//もし\0に到達したら
		if (snt_ary[snt_index] == 0) {
			//snt_indexから先を見るためにfor文を回す
			for (int i = snt_index; i < MAX_ST; i++) {
				//もし演算子を見つけたら
				if (0 < snt_ary[i] && snt_ary[i] < 48) {
					//[snt_index + j] <- [i + j] に移していく
					for (int j = 0; i + j < MAX_ST; j++) {
						snt_ary[snt_index + j] = snt_ary[i + j];
						snt_ary[i + j] = 0;
					}
					//再走
					break;
				}
				//もし最後まで数字、記号を見つけられないときは終了
				if (i == MAX_ST - 1) {
					flag_o = 1;
				}
			}
		}
		//snt_indexをインクリメント
		snt_index++;
	}
	return;
}

//全体の計算をしていく関数
void calc_act3(char* snt_ary) {

	int num_alt = 0;
	int flag_c = 0;
	int ans_len = 0;
	int snt_where = 0;
	int wids = 0;
	unsigned int Cr;

	Cr = GetColor(255, 255, 255);    // 白色の値を取得

	flag_c = calc_checker(snt_ary);

	num_x = 0;
	num_y = 0;
	div_0 = 0;

	//calc_old = calc_h;

	//掛け算または割り算
	while (flag_c) {
		calc_h = 0;
		num_x = 0;
		num_y = 0;
		num_i = 0;

		//文字列から数値を取得
		while (calc_h != 42 && calc_h != 47) {
			snt_where = num_i;
			num_x = 0;
			up_x_len = 0;
			up_y_len = 0;
			for (; 47 < snt_ary[num_i] && snt_ary[num_i] < 58; num_i++) {
				num_x = num_x * 10 + snt_ary[num_i] - 48;
			}
			if (snt_ary[num_i] == 46) {
				num_i++;
				for (; 47 < snt_ary[num_i] && snt_ary[num_i] < 58; num_i++) {
					num_x = num_x * 10 + snt_ary[num_i] - 48;
					up_x_len++;
				}
			}
			calc_h = snt_ary[num_i++];
		}
		for (; 47 < snt_ary[num_i] && snt_ary[num_i] < 58; num_i++) {
			num_y = num_y * 10 + snt_ary[num_i] - 48;
		}
		if (snt_ary[num_i] == 46) {
			num_i++;
			for (; 47 < snt_ary[num_i] && snt_ary[num_i] < 58; num_i++) {
				num_y = num_y * 10 + snt_ary[num_i] - 48;
				up_y_len++;
			}
		}

		if (calc_h == 47) {
			if (num_y != 0) {
				while (num_x < 100000000) {
					num_x *= 10;
					up_x_len++;
				}
				num_x = num_x / num_y;
			}
			else {
				div_0 = 1;
			}
		}
		else if (calc_h == 42) {
			num_x = num_x * num_y;
		}

		calc_old = calc_h;
		up_x_len = up_x_len + up_y_len;//積商なら小数点以下の桁数は和になる

		//num_x　を文字列に挿入
		into_str3(snt_ary, snt_where, num_i);

		//空白を埋める
		snt_setter(snt_ary);

		flag_c = calc_checker(snt_ary);
	}

	//足し算または引き算
	while (calc_checker2(snt_ary)) {

		num_x = 0;
		num_y = 0;
		num_i = 0;
		up_x_len = 0;
		up_y_len = 0;

		for (num_i = 0; 47 < snt_ary[num_i] && snt_ary[num_i] < 58; num_i++) {
			num_x = num_x * 10 + snt_ary[num_i] - 48;
		}
		if (snt_ary[num_i] == 46) {
			num_i++;
			for (; 47 < snt_ary[num_i] && snt_ary[num_i] < 58; num_i++) {
				num_x = num_x * 10 + snt_ary[num_i] - 48;
				up_x_len++;
			}
		}
		calc_h = snt_ary[num_i++];
		for (; 47 < snt_ary[num_i] && snt_ary[num_i] < 58; num_i++) {
			num_y = num_y * 10 + snt_ary[num_i] - 48;
		}
		if (snt_ary[num_i] == 46) {
			num_i++;
			for (int i = 1; 47 < snt_ary[num_i] && snt_ary[num_i] < 58; num_i++) {
				num_y = num_y * 10 + snt_ary[num_i] - 48;
				up_y_len++;
			}
		}
		if (up_x_len < up_y_len) {
			for (int i = 0; up_x_len < up_y_len; up_x_len++) {
				num_x *= 10;
			}
		}
		else if (up_y_len < up_x_len) {
			for (int i = 0; up_y_len < up_x_len; up_y_len++) {
				num_y *= 10;
			}
		}

		if (calc_h == 43) {
			num_x = num_x + num_y;
		}
		else if (calc_h == 45) {
			num_x = num_x - num_y;
		}

		calc_old = calc_h;

		// num_x　を文字列に挿入
		into_str3(snt_ary, 0, num_i);

		//空白を埋める
		snt_setter(snt_ary);
	}

	return;
}

//=を押された際の処理内容
void act_return(char* snt_ary, char* snt_ary2, int num_c) {
	int wids = 0;
	unsigned int Cr;

	Cr = GetColor(255, 255, 255);

	for (int i = 0; i < MAX_ST; i++) {
		snt_ary2[i] = snt_ary[i];
	}

	if (calc_old == 47) {
		if (num_y != 0) {
			num_x = num_x / num_y;
			up_x_len += up_y_len;
			into_str3(snt_ary, 0, MAX_ST);
		}
	}
	else if (calc_old == 42) {
		num_x = num_x * num_y;
		up_x_len += up_y_len;
		into_str3(snt_ary, 0, MAX_ST);
	}
	else if (calc_old == 43) {
		num_x = num_x + num_y;
		into_str3(snt_ary, 0, MAX_ST);
	}
	else if (calc_old == 45) {
		num_x = num_x - num_y;
		into_str3(snt_ary, 0, MAX_ST);
	}
	else {
		calc_act3(snt_ary);
	}

	draw_disp(snt_ary, snt_ary2);

	return;
}

void press_percent(char* snt_ary) {
	int num_index = 0;
	int num_start = 0;
	int num_p = 0;
	int up_p_len = 0;

	while (0 < snt_ary[num_index])
	{
		num_p = 0;
		up_p_len = 0;
		num_start = num_index;

		for (; 47 < snt_ary[num_index] && snt_ary[num_index] < 58; num_index++) {
			num_p = num_p * 10 + snt_ary[num_index] - 48;
		}
		if (snt_ary[num_index] == 46) {
			num_index++;
			for (; 47 < snt_ary[num_index] && snt_ary[num_index] < 58; num_index++) {
				num_p = num_p * 10 + snt_ary[num_index] - 48;
				up_p_len++;
			}
		}
		num_index++;
	}

	num_x = num_p;
	up_x_len = up_p_len + 2;
	calc_old = 0;
	into_str3(snt_ary, num_start, num_index-1);

	return;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	ChangeWindowMode(TRUE);

	unsigned int Cr;
	unsigned int x_set = 128;
	unsigned int y_set = 80;
	char num_set = 0;
	char buf[256] = { 0 };


	char c_set[4][5][4] = { {{"7"}, {"8"}, {"9"}, {"/"}, {"AC"}},
							{{"4"}, {"5"}, {"6"}, {"*"}, {"DEL"}},
							{{"1"}, {"2"}, {"3"}, {"-"}, {"%"}},
							{{"0"}, {"."}, {"="}, {"+"}, {0}} };

	//文字列用の配列などの準備
	char snt_ary[MAX_ST] = { 0 };
	char snt_ary2[MAX_ST] = { 0 };
	int num_c = 0;
	int num_c2 = 0;
	int wids = 0;


	Cr = GetColor(255, 255, 255);    // 白色の値を取得

	int mouse_x, mouse_y;

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);

	init_d3();

	while (1) {
		WaitKey();				// キー入力待ち


		//画面の初期化
		ClearDrawScreen();
		init_d3();

		// マウスの位置を取得
		GetMousePoint(&mouse_x, &mouse_y);


		if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)
		{
			// 押されている場合の処理
			mouse_x = mouse_x / 128;
			mouse_y = mouse_y / 80;

			if (mouse_y < 2) {
				break;
			}

			if (mouse_y > 1 && mouse_x < 5) {



				if (mouse_x == 4 && mouse_y == 2) {
					//AC の処理
					for (int i = 0; i < MAX_ST; i++) {
						snt_ary[i] = 0;
					}
					for (int i = 0; i < MAX_ST; i++) {
						snt_ary2[i] = 0;
					}
					num_c = 0;
					draw_disp(snt_ary, snt_ary2);

					calc_old = 0;
				}
				else if (mouse_x == 4 && mouse_y == 3) {

					//DEL の処理
					if (num_c > 0) {
						snt_ary[--num_c] = 0;
					}
					draw_disp(snt_ary, snt_ary2);

					calc_old = 0;
				}
				else if (mouse_x == 2 && mouse_y == 5) {

					// = の処理

					act_return(snt_ary, snt_ary2, num_c);
					num_c = str_len(snt_ary);
				}
				else if (mouse_x == 4 && mouse_y == 4) {
					// % キー
					press_percent(snt_ary);
					draw_disp(snt_ary, snt_ary2);
					num_c = str_len(snt_ary);
				}
				else if (mouse_x == 4 && mouse_y == 5) {
					//終了キー
					break;
				}
				else {
					//数字を押した時の処理
					snt_ary[num_c++] = c_set[mouse_y - 2][mouse_x][0];
					draw_disp(snt_ary, snt_ary2);

					calc_old = 0;
				}
			}
		}
		else
		{
			// マウスが押されていない

			num_set = 0;
			GetHitKeyStateAll(buf);

			if (buf[KEY_INPUT_ESCAPE] == 1) { break; }
			else if (buf[KEY_INPUT_0] || buf[KEY_INPUT_NUMPAD0]) { num_set = 48; }
			else if (buf[KEY_INPUT_1] || buf[KEY_INPUT_NUMPAD1]) { num_set = 49; }
			else if (buf[KEY_INPUT_2] || buf[KEY_INPUT_NUMPAD2]) { num_set = 50; }
			else if (buf[KEY_INPUT_3] || buf[KEY_INPUT_NUMPAD3]) { num_set = 51; }
			else if (buf[KEY_INPUT_4] || buf[KEY_INPUT_NUMPAD4]) { num_set = 52; }
			else if (buf[KEY_INPUT_5] || buf[KEY_INPUT_NUMPAD5]) { num_set = 53; }
			else if (buf[KEY_INPUT_6] || buf[KEY_INPUT_NUMPAD6]) { num_set = 54; }
			else if (buf[KEY_INPUT_7] || buf[KEY_INPUT_NUMPAD7]) { num_set = 55; }
			else if (buf[KEY_INPUT_8] || buf[KEY_INPUT_NUMPAD8]) { num_set = 56; }
			else if (buf[KEY_INPUT_9] || buf[KEY_INPUT_NUMPAD9]) { num_set = 57; }
			else if (buf[KEY_INPUT_SEMICOLON] == 1 || buf[KEY_INPUT_ADD] == 1) { num_set = 43; }
			else if (buf[KEY_INPUT_COLON] == 1 || buf[KEY_INPUT_MULTIPLY] == 1) { num_set = 42; }
			else if (buf[KEY_INPUT_MINUS] == 1 || buf[KEY_INPUT_SUBTRACT] == 1) { num_set = 45; }
			else if (buf[KEY_INPUT_SLASH] == 1 || buf[KEY_INPUT_DIVIDE] == 1) { num_set = 47; }
			else if (buf[KEY_INPUT_PERIOD] == 1 || buf[KEY_INPUT_DECIMAL] == 1) { num_set = 46; }
			else if (buf[KEY_INPUT_NUMPADENTER] == 1 || buf[KEY_INPUT_RETURN] == 1) {
				// = の処理
				act_return(snt_ary, snt_ary2, num_c);
				num_c = str_len(snt_ary);
			}
			else if (buf[KEY_INPUT_BACK] == 1) {
				if (num_c > 0) {
					snt_ary[--num_c] = 0;
				}
				draw_disp(snt_ary, snt_ary2);
			}
			else if (buf[KEY_INPUT_DELETE] == 1) {
				//AC の処理
				for (int i = 0; i < MAX_ST; i++) {
					snt_ary[i] = 0;
				}
				for (int i = 0; i < MAX_ST; i++) {
					snt_ary2[i] = 0;
				}
				num_c = 0;
				draw_disp(snt_ary, snt_ary2);
			}

			if (0 < num_set) {
				snt_ary[num_c++] = num_set;

				calc_old = 0;
				draw_disp(snt_ary, snt_ary2);
			}

		}
	}


	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}