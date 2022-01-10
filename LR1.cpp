#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<iomanip>

using namespace std;

void initial_fun(string filename);
void LR_analyze(string w, int S);
void error();

map<int, pair<string, string>> P; //用来存储所有产生式
map<pair<int, string>, pair<string, int>> Action; //存储LR分析表中的Action表项
map<pair<int, char>, int> Goto; //存储LR分析表中的Goto表项
set<string> T, N; //分别代表终结符号集、非终结符号集

int main()
{
	string filename = "LR1_analyze_chart.txt", w;
	initial_fun(filename);//初始化

	cout << "Please input the sequence:" << endl;
	cin >> w;

	LR_analyze(w, 0);//LR分析

	cout << "\n\n";
	cout << setw(55) << setfill('-') <<"WELL DONE!";
	cout << setw(45) << setfill('-');
	cout << "\n" << endl;
	system("pause");

	return 0;
}

void initial_fun(string filename)
{
	ifstream f1;
	f1.open(filename, ios::in);
	if (!f1.is_open()) {//读入失败
		cout << "initialization failed." << endl;
		system("pause");
		exit(-1);
	}
	else {
		string n, t, first, second;
		int num = 0, state1 = 0, state2 = 0;
		string temp1, temp2;
		f1 >> num;
		for (int i = 0; i < num; i++) {//初始化非终结符集
			f1 >> n;
			N.insert(n);
		}
		f1 >> num;
		for (int i = 0; i < num; i++) {//初始化终结符集
			f1 >> t;
			T.insert(t);
		}
		f1 >> num;
		for (int i = 0; i < num; i++) {//初始化产生式集
			f1 >> first >> second;
			P.insert({ i, {first, second} });
		}
		f1 >> num;
		for (int i = 0; i < num; i++) {//初始化Action表项
			f1 >> state1 >> temp1 >> temp2 >> state2;
			Action.insert({ {state1, temp1}, {temp2, state2} });
		}
		f1 >> num;
		for (int i = 0; i < num; i++) {//初始化Goto表项
			char temp;
			f1 >> state1 >> temp >> state2;
			Goto.insert({ {state1, temp}, state2 });
		}
	}
}

void error()
{
	cout << "\n\n";
	cout << setw(59) << setfill('-') << "AN ERROR OCCURRED.";
	cout << setw(41) << setfill('-');
	cout << "\n" << endl;
	system("pause");
	exit(-1);
}

void LR_analyze(string w, int S)
{
	vector<int> State;//状态栈
	vector<string> Symbol;//符号栈
	//初始化
	string a;
	int state = 0;
	State.push_back(S);
	Symbol.push_back("#");
	w = w + '$';
	int ip = 0;

	do {
		//令S是栈顶状态，a是ip所指向的符号
		state = State[State.size() - 1];
		a = w[ip];
		if (w[ip] == 'n' && w[ip + 1] == 'u' && w[ip + 2] == 'm') {
			a = "num";
		}//判断ip是否指向“num”

		cout << setw(100) << setfill('-');
		cout << "\n";
		cout << setw(10) << setfill(' ') << "栈: ";
		cout << "State: ";
		for (int i = 0; i < State.size(); i++)
			cout << State[i] << " ";

		cout << "Symbol : ";
		for (int i = 0; i < Symbol.size(); i++)
			cout << Symbol[i] << " ";
		cout << endl;

		cout << setw(10) << "输入: " << w.substr(ip) << endl;

		auto action = Action.find({ state, a });

		if (action == Action.end()) {//当action表项查询失败时
			cout << setw(10) << "分析动作: ERROR! No Action!" << endl;
			error();
		}
		auto act_s = action->second;
		string opt = act_s.first;
		int new_s = act_s.second;

		if (opt == "s") {
			//把a和S'分别压入符号栈和状态栈的栈顶；
			Symbol.push_back(a);
			State.push_back(new_s);
			//推进ip，使它指向下一个输入符号
			if (a == "num") ip += 3;
			else ip++;

			cout << setw(10) << "分析动作: Shift " << new_s << endl;
		}
		else if (opt == "r") {
			auto temp = P.find(new_s);
			string f = temp->second.first, s = temp->second.second;
			//从栈顶弹出|β|个符号；
			if (s == "num") {
				Symbol.pop_back();
				State.pop_back();
			}
			else {
				for (int i = 0; i < s.length(); i++) {
					Symbol.pop_back();
					State.pop_back();
				}
			}
			//令S‘是当前栈顶状态，把A和goto[S', A]分别压入符号栈和状态站的栈顶
			int t_s = State[State.size() - 1];
			auto go = Goto.find({ t_s ,f[0] });
			int next_s = go->second;

			Symbol.push_back(f);
			State.push_back(next_s);
			//输出产生式A->β
			cout << setw(10) << "分析动作: reduce by " << f << " -> " << s << endl;
		}
		else if (opt == "acc") {
			cout << setw(10) << "分析动作: ACC" << endl;
			return;
		}
		else {
			cout << setw(10) << "分析动作: ERROR! No Action!" << endl;
			error();
		}

	} while (true);
}