#pragma once
#include "Piece.h"
#include "ComparePieces.h"
#include <iostream>
#include <algorithm>

class Individual
{
public:
	Individual() = default;
	Individual(int paperLength, int paperWidth, const std::vector<std::pair<Piece, int>>& piecesInfo);
	Individual(const Individual& individual);

	bool operator ==(const Individual& individual);
	bool operator !=(const Individual& individual);

	friend std::ostream& operator<<(std::ostream& out, const Individual& individual);

	/* Evaluate fitness */
	void ComputeFitness();

	/* Evaluate unfitness */
	void ComputeUnfitness();

	void Mutation();
	void Improve();
	void SortPieces();

private:
	void GeneratePieces(int amount, Piece piece);

public:
	std::vector<Piece> Pieces;
	int PaperLength, PaperWidth;
	double Fitness, Unfitness;
};

