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

map<int, pair<string, string>> P; //�����洢���в���ʽ
map<pair<int, string>, pair<string, int>> Action; //�洢LR�������е�Action����
map<pair<int, char>, int> Goto; //�洢LR�������е�Goto����
set<string> T, N; //�ֱ�����ս���ż������ս���ż�

int main()
{
	string filename = "LR1_analyze_chart.txt", w;
	initial_fun(filename);//��ʼ��

	cout << "Please input the sequence:" << endl;
	cin >> w;

	LR_analyze(w, 0);//LR����

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
	if (!f1.is_open()) {//����ʧ��
		cout << "initialization failed." << endl;
		system("pause");
		exit(-1);
	}
	else {
		string n, t, first, second;
		int num = 0, state1 = 0, state2 = 0;
		string temp1, temp2;
		f1 >> num;
		for (int i = 0; i < num; i++) {//��ʼ�����ս����
			f1 >> n;
			N.insert(n);
		}
		f1 >> num;
		for (int i = 0; i < num; i++) {//��ʼ���ս����
			f1 >> t;
			T.insert(t);
		}
		f1 >> num;
		for (int i = 0; i < num; i++) {//��ʼ������ʽ��
			f1 >> first >> second;
			P.insert({ i, {first, second} });
		}
		f1 >> num;
		for (int i = 0; i < num; i++) {//��ʼ��Action����
			f1 >> state1 >> temp1 >> temp2 >> state2;
			Action.insert({ {state1, temp1}, {temp2, state2} });
		}
		f1 >> num;
		for (int i = 0; i < num; i++) {//��ʼ��Goto����
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
	vector<int> State;//״̬ջ
	vector<string> Symbol;//����ջ
	//��ʼ��
	string a;
	int state = 0;
	State.push_back(S);
	Symbol.push_back("#");
	w = w + '$';
	int ip = 0;

	do {
		//��S��ջ��״̬��a��ip��ָ��ķ���
		state = State[State.size() - 1];
		a = w[ip];
		if (w[ip] == 'n' && w[ip + 1] == 'u' && w[ip + 2] == 'm') {
			a = "num";
		}//�ж�ip�Ƿ�ָ��num��

		cout << setw(100) << setfill('-');
		cout << "\n";
		cout << setw(10) << setfill(' ') << "ջ: ";
		cout << "State: ";
		for (int i = 0; i < State.size(); i++)
			cout << State[i] << " ";

		cout << "Symbol : ";
		for (int i = 0; i < Symbol.size(); i++)
			cout << Symbol[i] << " ";
		cout << endl;

		cout << setw(10) << "����: " << w.substr(ip) << endl;

		auto action = Action.find({ state, a });

		if (action == Action.end()) {//��action�����ѯʧ��ʱ
			cout << setw(10) << "��������: ERROR! No Action!" << endl;
			error();
		}
		auto act_s = action->second;
		string opt = act_s.first;
		int new_s = act_s.second;

		if (opt == "s") {
			//��a��S'�ֱ�ѹ�����ջ��״̬ջ��ջ����
			Symbol.push_back(a);
			State.push_back(new_s);
			//�ƽ�ip��ʹ��ָ����һ���������
			if (a == "num") ip += 3;
			else ip++;

			cout << setw(10) << "��������: Shift " << new_s << endl;
		}
		else if (opt == "r") {
			auto temp = P.find(new_s);
			string f = temp->second.first, s = temp->second.second;
			//��ջ������|��|�����ţ�
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
			//��S���ǵ�ǰջ��״̬����A��goto[S', A]�ֱ�ѹ�����ջ��״̬վ��ջ��
			int t_s = State[State.size() - 1];
			auto go = Goto.find({ t_s ,f[0] });
			int next_s = go->second;

			Symbol.push_back(f);
			State.push_back(next_s);
			//�������ʽA->��
			cout << setw(10) << "��������: reduce by " << f << " -> " << s << endl;
		}
		else if (opt == "acc") {
			cout << setw(10) << "��������: ACC" << endl;
			return;
		}
		else {
			cout << setw(10) << "��������: ERROR! No Action!" << endl;
			error();
		}

	} while (true);
}