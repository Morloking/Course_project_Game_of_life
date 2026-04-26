#include "field.h"

#include <cstdlib> //for function system clear

#include <iostream>
#include <fstream> //for reading file

#include <windows.h> // for function sleep

namespace game_of_life {
	Field::Field()
	{
		readStartFile(); //===============1).только инициализация из файла в конструкторе
	}

	void Field::readStartFile() {
		std::ifstream file("start_state.txt");
		if (file.is_open()) {
			file >> rows >> columns;
			//установка размера вспомогательного вектора, который будет хранить предыдущее состояние в игре
			prevIsAlive.resize(rows);
			for (int i = 0; i < rows; ++i) {
				prevIsAlive[i].resize(columns);
			}
			//установка размера основного вектора и его инициализация значениями из файла
			vIsAlive.resize(rows);
			for (int i = 0; i < rows; ++i) {
				vIsAlive[i].resize(columns);
			}
			int currRow{ 0 }; int currColumn{ 0 };
			while (file >> currRow >> currColumn) { 
				if (currRow >= 0 && currRow < rows && currColumn >= 0 && currColumn < columns) { //====================3)проверка границ
					vIsAlive[currRow][currColumn] = true;
				}
				else {
					std::cerr << "Error: invalid coordinates\n";
				}
			}
			file.close();
		}
		else {									// ======================2).если файл не открылся, сообщение об ошибке и выход
			std::cerr << "Error: can't open file\n"; 
			return;
		}
		
	}

	void Field::runGame() {
		printFields();
		while (!checkOver()) {
			updateFields();
		}
		gameOver(reasonDead);
	}

	void Field::printFields() {
		for (int i = 0; i < vIsAlive.size(); ++i) {
			for (int j = 0; j < vIsAlive[i].size(); ++j) {
				if (vIsAlive[i][j] == true) std::cout << '*';
				else std::cout << '-';
				std::cout << " ";
			}
			std::cout << "\n";
		}
		printCurrState();
	}

	void Field::updateFields() {
		prevIsAlive = vIsAlive; //сохраняем предыдущее состояние
		++generation; //увеличиваем поколение

		// алгоритм изменения состояния ячейки  и алгоритм прохода по ячейкам поля
		for (int i = 0; i < prevIsAlive.size(); ++i) {
			for (int j = 0; j < prevIsAlive[i].size(); ++j) {

				int count = countingNeighbours(i,j,prevIsAlive);//==================4).убрал дублирование
				//если клетка мёртвая
				if (prevIsAlive[i][j] == false) {
					if (count == 3) { vIsAlive[i][j] = true; } //оживление мёртвой клетки, если есть 3 живых соседа
				}
				//если клетка живая
				else {
					if (count == 3 || count == 2) { vIsAlive[i][j] = true; } //2 или 3 живых соседа - клетка живёт
					else { vIsAlive[i][j] = false; } //в ином случае - умирает от одиночества или перенаселённости
				}
			}
		}

		Sleep(1000); // задержка
		std::system("cls"); //очистка консоли
		printFields(); //печать полей
	}

	int Field::countingNeighbours(int i, int j, const std::vector<std::vector<bool>>& field) {
		int count{ 0 };
		for (int str = i - 1; str <= i + 1; ++str) {
			if (str < 0 || str >= rows) continue;
			for (int stlb = j - 1; stlb <= j + 1; ++stlb) {
				if (stlb < 0 || stlb >= columns || (str == i && stlb == j)) continue;
				if (field[str][stlb] == true) { ++count; }
			}
		}
		return count;
	}

	bool Field::checkOver() {
		// алгоритм выхода из игры по правилу 1 
		bool foundAlive = false;
		foundAlive = countingAliveCells('l');
		// алгоритм выхода из игры по правилу 2
		bool stable = (prevIsAlive == vIsAlive);
		if (!foundAlive) { reasonDead = 'a'; return true; } // +запись причины смерти
		else if (stable) { reasonDead = 'b'; return true; } // +запись причины смерти
		return (!foundAlive || stable);
	}

	void Field::printCurrState() {
		std::cout << "Generation: " << generation << ". "
			<< "Alive cells: " << countingAliveCells('b') << "\n";
	}

	int Field::countingAliveCells(char mode) {
		//b = базовый полный подсчёт(base), l = лёгкий подсчёт (light), возвращающий 0 - false или 1 - true
		if (mode == 'b') {
			int count{ 0 };
			for (int i = 0; i < vIsAlive.size(); ++i) {
				for (int j = 0; j < vIsAlive[i].size(); ++j) {
					if (vIsAlive[i][j] == true) { ++count; }
				}
			}
			return count;
		}

		else if (mode == 'l') {
			for (int i = 0; i < vIsAlive.size(); ++i) {
				for (int j = 0; j < vIsAlive[i].size(); ++j) {
					if (vIsAlive[i][j] == true) { return 1; }
				}
			}
			return 0;
		}
		return 0;
	}

	void Field::gameOver(char reason) {
		switch (reason)
		{
		case 'a':
			std::cout << "All cells are dead. Game over " << "\n";
			break;
		case 'b':
			std::cout << "The world has stagnated. Game over " << "\n";
			break;
		}
	}

}
