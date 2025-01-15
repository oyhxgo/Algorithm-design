#include <bits/stdc++.h>
using namespace std;
#define INF 10000000
//考虑到对每一个X都有一个各位数相加的属性和位数最大值和最小值
//所以我们创建结构体
struct x {
	int sum;
	int Max;
	int Min;
	int ans;//最小圈乘次数
} S[INF];
unsigned long long mans;//圈乘问题表达式子计算的最大值

int countsum(unsigned long long num) { //返回各位数之和
	int sum = 0;
	while (num > 0) {
		sum += (num % 10);
		num /= 10;
	}
	return sum;
}
//找出最大最小数字,找出最大最小值，接收max和min时，max应设置为-1，min设置为10
void search_max_min(unsigned long long num, int &Max, int &Min) { //不用min和max怕和函数混了
	Max = -1;
	Min = 10;
	while (num > 0) {
		int t = num % 10;
		if (t < Min)
			Min = t;
		if (t > Max)
			Max = t;
		num /= 10;
	}
}
void init(unsigned long long X) {
	//计算位数
	int m = ceil(log(X + 1) / log(10.0));
	//利用结论计算最大值
	mans = 81 * m + 9;

	//防止一位都没有
	if (mans < 171)
		mans = 171;
	//初始化最小圈乘次数和最大最小值
	for (unsigned long long i = 1; i <= mans; i++) {
		S[i].ans = INF; //最小圈乘次数，此时没被计算，所以设成无穷大，表示由X转化到i需要的最小步骤
		S[i].sum = 0;
		S[i].Max = 0;
		S[i].Min = INF;
	}
	//提前开始计算
	for (unsigned long long i = 1; i <= mans; i++) {
		//先计算sum
		S[i].sum = countsum(i);
		//在计算两个最值
		search_max_min(i, S[i].Max, S[i].Min);
	}
//	//存储好X的相关信息
	S[0].ans = 0;//X转化到X需要0步
	S[0].sum = countsum(X);
	search_max_min(X, S[0].Max, S[0].Min);
}
//动态规划求解
int solu(unsigned long long X, unsigned long long K) {
	//做好准备工作的初始化
	init(X);
	//如果X和K相同则需要0次转换
	if (X == K)
		return 0;
	//如果K大于最大值，则不可能实现，no answer
	if (K > mans)
		return -1;

	bool flag = true;
	while (flag) { //最优值不再更新时退出
		flag = false;
		for (unsigned long long i = 0; i <= mans; i++) {
			if (S[i].ans < INF) { //S[i]是否计算过,必须加这个判断，因为没有计算过的不能参与计算

				for (unsigned long long j = 0; j <= mans; j++) {
					if (S[j].ans < INF) { //j已计算过
						unsigned long long a = S[i].sum * S[j].Max + S[j].Min;
						if (S[a].ans > S[i].ans + S[j].ans + 1) { //圈乘个数更少，更新
							S[a].ans = S[i].ans + S[j].ans + 1;
							flag = true;
						}
					}
				}
			}
		}
	}
	if (S[K].ans < INF)//最终答案计算出来则输出
		return S[K].ans;
	else//如果没计算过，说明没找到答案
		return -1;
}
int main() {
	ifstream in("#oper.txt");//源文件的位置，插入流
	ofstream out("#operout.txt");
	if (!in.is_open()) {
		cout << "输入文件错误：请将输入文件放在和源代码同一个目录之下！！";
		exit(0);
	} else {
		cout << "输入程序正常运行" << endl;
	}
	if (out.is_open()) {
		cout << "输出程序正常运行，开始计算...." << endl;
	} else {
		cout << "输出文件错误:无法打开文件。" << endl;
	}
	string goin;
	while (getline(in, goin) && (goin != "0 0")) {
		stringstream ss(goin);
		unsigned long long X, K;
		if (ss >> X >> K) {
			int ans = solu(X, K);
			//控制台输出
			if (ans == -1) {
				cout << "No answer!" << endl;
				out << "No answer!" << endl;
			} else {
				cout << ans << endl;
				out << ans << endl;
			}
		} else {
			cout << "读取该行数字失败" << endl;
			out << "读取该行数字失败" << endl;
		}
	}
	in.close();//关闭输入
	out.close();

//	int ans = solu(9876543210123456789, 1);
//	if (ans == -1)
//		cout << "No answer!" << endl;
//	else
//		cout << ans << endl;
}


//防伪码！！！
/*
?* ┌─┐?? ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
?* │Esc│?? │ F1│ F2│ F3│ F4│ │ F5│ F6│ F7│ F8│ │ F9│F10│F11│F12│ │P/S│S mans│P/B│? ┌┐??? ┌┐??? ┌┐
?* └─┘?? └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘? └┘??? └┘??? └┘
?* ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
?* │~ `│!?1│@?2│#?3│$?4│%?5│^?6│&?7│*?8│(?9│)?0│_ -│+ =│ BacSp │ │Ins│Hom│PUp│ │N mans│ / │ * │ - │
?* ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
?* │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{ [│} ]│ | \ │ │Del│End│PDn│ │?7 │?8 │?9 │?? │
?* ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤ + │
?* │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│ Enter? │?????????????? │?4 │?5 │?6 │?? │
?* ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤???? ┌───┐???? ├───┼───┼───┼───┤
?* │ Shift? │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│? Shift?? │???? │ ↑ │???? │?1 │?2 │?3 │?? │
?* ├─────┬──┴─┬─┴──┬┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤ E││
?* │ Ctrl│??? │Alt │???????? Space???????? │ Alt│??? │??? │Ctrl│ │ ← │ ↓ │ → │ │???0?? │ . │←─┘│
?* └─────┴────┴────┴───────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
?*/
