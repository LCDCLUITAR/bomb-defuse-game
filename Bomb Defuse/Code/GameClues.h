#pragma once
class GameClues{
protected:
	int barcodeAnswer[4];
public:
	GameClues();
	~GameClues();

	int getLockNum(int[]);
};

int randomNumGen();