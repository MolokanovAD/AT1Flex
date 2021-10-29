#include "Recognizer.h"

Recognizer::Recognizer(const std::string& fname) :filename(fname), metColon(false), mainGoalRead(false),time(std::chrono::steady_clock::duration::zero()),linesTotal(0) {
	currentLine.first = true;
	currentLine.second=std::shared_ptr<std::string>(new std::string(""));
}


void Recognizer::reset()
{
	mainGoalRead = false;
	metColon = false;
	currentLine.first = true;
	currentGoals.clear();
	currentLine.second = std::shared_ptr<std::string>(new std::string(""));
}
void Recognizer::push_goal_to_result(const std::shared_ptr<std::string>& s) {
	auto j = resultGoals.begin();
	for (; j < resultGoals.end(); j++) {
		if (**j == *s)
			break;
	}
	if (j == resultGoals.end())
		resultGoals.push_back(s);
}
void Recognizer::push_goals() {
	if (currentGoals.size() > 1) {
		mainGoals.push_back(currentGoals[0]);
		//Если цель из нового списка требований уже есть
		//в результирующем массиве, то ее вставлять не надо
		for (auto i = currentGoals.begin() + 1; i < currentGoals.end(); i++) {
			push_goal_to_result(*i);
		}
	}
	else if(currentGoals.size() == 1){
		//если строка состоит из одной цели, значит у этой
		//цели нет списка требований => тоже включаем ее в результат
		push_goal_to_result(currentGoals[0]);
	}
}

bool Recognizer::checkEqual() {
	if (!currentGoals.empty()) {
		auto last_goal = --currentGoals.cend();
		for (auto i = currentGoals.cbegin(); i < last_goal; i++) {
			if (**last_goal == **i)
				return true;
		}
	}
	return false;
}

void Recognizer::fix_result() {
	for (auto j : mainGoals) {
		for (auto i = resultGoals.cbegin(); i < resultGoals.cend(); i++) {
			if (**i == *j) {
				resultGoals.erase(i);
				break;
			}
		}
	}
}

void Recognizer::write_result() {
	std::string out1 = filename + ".check";
	std::string out2 = filename + ".result";
	std::ofstream outFile1(out1);
	std::ofstream outFile2(out2);
	for (auto i : lines) {
		outFile1 << "String \"" << *i.second << (i.first ? "\" is correct" : "\" is not correct") << std::endl;
	}
	for (auto i : resultGoals) {
		outFile2 << *i << std::endl;
	}
}

void Recognizer::run() {
	std::ifstream stream(filename);
	if (!stream.is_open())
		throw std::runtime_error("Wrong filename");
	yyFlexLexer lexer(&stream);
	int value = 5;
	while (value != END_OF_FILE) {
		value = lexer.yylex();
		switch (value)
		{
		case WRONG_NAME: {
			currentLine.first = false;
			complement_string(lexer.YYText());
			break;
		}
		case GOAL: {
			if (currentLine.first) {
				if (!mainGoalRead) {
					//Если еще не прочитана главная цель, отмечаем это
					//и вставляем в массив целей из текущей строки
					mainGoalRead = true;
					currentGoals.push_back(std::shared_ptr<std::string>(new std::string(lexer.YYText())));
				}
				else if (!metColon) {
					//если главная цель прочитана и встречена еще одна без двоеточия между ними,
					//строка считается некорректной
					currentLine.first = false;
				}
				else {
					//если главная цель прочитана и двоеточие уже было,
					//записываем текущую цель в массив целей данной строки
					currentGoals.push_back(std::shared_ptr<std::string>(new std::string(lexer.YYText())));
				}
				if (currentLine.first)
					currentLine.first = !checkEqual();
			}
			complement_string(lexer.YYText());
			break;
		}
		case COLON: {
			if (currentLine.first) {
				if (metColon || currentGoals.size() != 1) {
					//если двоеточие уже было считано или оно встречено не после
					//единственной(главной) цели, строка некорректна
					currentLine.first = false;
				}
				metColon = true;
			}
			complement_string(lexer.YYText());
			break;
		}
		case END_OF_FILE:
		case END_OF_STRING: {
			if (currentLine.first) {
				//если строка корректна проверяем дублирование и наличие хотя бы одной цели
				if(checkEqual() || isEmpty())
					currentLine.first = false;
				if (currentLine.first)
					push_goals();
			}
			lines.push_back(currentLine);
			reset();
			break;
		}
		default: {	complement_string(lexer.YYText()); }
		}
	}
	fix_result();
}
void Recognizer::timing() {
	std::ifstream stream(filename);
	if (!stream.is_open())
		throw std::runtime_error("Wrong filename");
	yyFlexLexer lexer(&stream);
	int value = 5;
	auto t = std::chrono::steady_clock::now();
	while (value != END_OF_FILE) {
		t = std::chrono::steady_clock::now();
		value = lexer.yylex();
		switch (value)
		{
		case WRONG_NAME: {
			currentLine.first = false;
			complement_string(lexer.YYText());
			time += std::chrono::steady_clock::now() - t;
			break;
		}
		case GOAL: {
			if (currentLine.first) {
				if (!mainGoalRead) {
					//Если еще не прочитана главная цель, отмечаем это
					//и вставляем в массив целей из текущей строки
					mainGoalRead = true;
				}
				else if (!metColon) {
					//если главная цель прочитана и встречена еще одна без двоеточия между ними,
					//строка считается некорректной
					currentLine.first = false;
				}
				if (currentLine.first)
					currentLine.first = !checkEqual();
				if (currentLine.first)
					currentGoals.push_back(std::shared_ptr<std::string>(new std::string(lexer.YYText())));
			}
			complement_string(lexer.YYText());
			time += std::chrono::steady_clock::now() - t;
			break;
		}
		case COLON: {
			if (currentLine.first) {
				if (metColon || currentGoals.size() != 1) {
					//если двоеточие уже было считано или оно встречено не после
					//единственной(главной) цели, строка некорректна
					currentLine.first = false;
				}
				metColon = true;
			}
			complement_string(lexer.YYText());
			time += std::chrono::steady_clock::now() - t;
			break;
		}
		case END_OF_FILE:
		case END_OF_STRING: {
			if (currentLine.first) {
				//если строка корректна проверяем дублирование и наличие хотя бы одной цели
				if (checkEqual() || isEmpty())
					currentLine.first = false;
				time += std::chrono::steady_clock::now() - t;
				if (currentLine.first)
					push_goals();
			}
			else
				time += std::chrono::steady_clock::now() - t;
			lines.push_back(currentLine);
			linesTotal++;
			reset();
			break;
		}
		default: {	complement_string(lexer.YYText()); time += std::chrono::steady_clock::now() - t; }
		}
	}
	fix_result();
}