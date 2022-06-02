#pragma once
#include <random>
#include <iomanip>

class Piece
{
public:
	Piece() = default;
	Piece(int length, int width, int type, int value);
	Piece(const Piece& piece);

	bool operator ==(const Piece& piece);
	bool operator !=(const Piece& piece);

	friend std::istream& operator>>(std::istream& in, Piece& piece);
	friend std::ostream& operator<<(std::ostream& out, const Piece& piece);

	void GenerateCoordinates(int paperLength, int paperWidth);

public:
	int Type, Value;
	int Length, Width;
	bool IsCut;

	double X, Y;
};