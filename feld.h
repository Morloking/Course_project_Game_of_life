#pragma once
#include <vector>

namespace game_of_life {

class Field
{

public:
	Field(); 
	~Field();
private:

	//general functions
	void readStartFile();
	void runGame();
	void printFields();
	void printCurrState();
	int countingAliveCells(char mode);
	void updateFields();
	bool checkOver();
	void gameOver(char reason);

	//internal info
	int rows{ 0 };
	int columns{ 0 };
	std::vector <std::vector<bool>> vIsAlive { };
	std::vector <std::vector<bool>> prevIsAlive{ };
	//external info
	int generation{ 1 };
	char reasonDead{ 'a' };

};

}	
