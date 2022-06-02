#include "Piece.h"

Piece::Piece(int length, int width, int type, int value) : 
	X(0), Y(0)
{
	Length = length;
	Width = width;
	Type = type;
	Value = value;
	IsCut = true;
}

Piece::Piece(const Piece& piece)
{
	Length = piece.Length;
	Width = piece.Width;
	Type = piece.Type;
	Value = piece.Value;
	IsCut = piece.IsCut;

	X = piece.X;
	Y = piece.Y;
}

bool Piece::operator==(const Piece& piece)
{
	return (IsCut == piece.IsCut && X == piece.X && Y == piece.Y);
}

bool Piece::operator!=(const Piece& piece)
{
	return (IsCut != piece.IsCut or X != piece.X or Y != piece.Y);
}

void Piece::GenerateCoordinates(int paperLength, int paperWidth)
{
	std::random_device rd;
	std::mt19937_64 generator(rd());

	/* Generate X */
	double lowerBoundLength = (double)Length / 2;
	double upperBoundLength = paperLength - lowerBoundLength;

	std::uniform_int_distribution<int> distrib(lowerBoundLength, upperBoundLength);

	X = distrib(generator);

	/* Generate Y */
	double lowerBoundWidth = (double)Width / 2;
	double upperBoundWidth = paperWidth - lowerBoundWidth;

	distrib = std::uniform_int_distribution<int>(lowerBoundWidth, upperBoundWidth);

	Y = distrib(generator);
}

std::istream& operator>>(std::istream& in, Piece& piece)
{
	in >> piece.Length >> piece.Width >> piece.Value;
	return in;
}

std::ostream& operator<<(std::ostream& out, const Piece& piece)
{
	out << "Type: " << std::fixed << std::setw(2) << piece.Type;
	out << "\tLength: " << std::fixed << std::setw(5) << piece.Length;
	out << "\tWidth: " << std::fixed << std::setw(5) << piece.Width;
	out << "\tX: " << std::fixed << std::setw(10) << piece.X;
	out << "\tY: " << std::fixed << std::setw(10) << piece.Y;
	out << "\tValue: " << std::fixed << std::setw(5) << piece.Value;
	out << "\tIsCut: " << piece.IsCut;

	return out;
}