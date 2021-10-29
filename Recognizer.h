#pragma once
#include <FlexLexer.h>
#include <vector>
#include <fstream>
#include <chrono>
enum NAME {
	END_OF_FILE = -1,
	WRONG_NAME,
	GOAL,
	COLON,
	END_OF_STRING,
	SPACES
};
typedef std::vector<std::shared_ptr<std::string>> goal_vector;
typedef std::pair<bool, std::shared_ptr<std::string>> line;
class Recognizer {
private:
	std::string filename;
	//������� ������ � �������� ����, � ���� � ������� ��������� ������������
	line currentLine;
	//��������� �� ������� ����
	bool mainGoalRead;
	//���� �� ��������� ���������
	bool metColon;
	//��� ������� ���� �� ���� �������������� �����
	goal_vector mainGoals;
	//��� ��������� ����, �� ������� ����� ��������� �����
	goal_vector resultGoals;
	//��� ���� �� ������� ������
	goal_vector currentGoals;
	//��� �������������� ������
	std::vector<line> lines;
	std::chrono::steady_clock::duration time;
	long long linesTotal;
	void push_goal_to_result(const std::shared_ptr<std::string>&);
	void reset();
	void fix_result();
	void push_goals();
	void complement_string(const std::string& str) { *currentLine.second += str; }
	bool checkEqual();
	bool isEmpty() { return !currentGoals.size(); }
public:
	Recognizer(const std::string&);
	std::chrono::steady_clock::duration showTime() { return time; }
	long long linesChecked() { return linesTotal; }
	void run();
	void timing();
	void write_result();
};

