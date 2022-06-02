#include "Individual.h"

Individual::Individual(int paperLength, int paperWidth,
	const std::vector<std::pair<Piece, int>>& piecesInfo) :
	Fitness(0), Unfitness(0)
{
	PaperLength = paperLength;
	PaperWidth = paperWidth;

	for (auto& [piece, amount] : piecesInfo)
	{
		GeneratePieces(amount, piece);
	}

	ComputeFitness();
	ComputeUnfitness();
}

Individual::Individual(const Individual& individual)
{
	Pieces.clear();

	PaperLength = individual.PaperLength;
	PaperWidth = individual.PaperWidth;

	for (int index = 0; index < individual.Pieces.size(); ++index)
	{
		Pieces.push_back(individual.Pieces[index]);
	}

	ComputeFitness();
	ComputeUnfitness();
}

void Individual::Mutation()
{
	std::random_device rd;
	std::mt19937_64 generator(rd());
	std::uniform_int_distribution<int> distrib(0, Pieces.size() - 1);

	int pos = distrib(generator);
	Pieces[pos].IsCut = false;
}

void Individual::Improve()
{
	if (Unfitness == 0)
		return;

	double oldUnfitness = -1;

	do
	{
		oldUnfitness = Unfitness;

		for (size_t index = 0; index < Pieces.size(); ++index)
		{
			if (Pieces[index].IsCut == true)
			{
				int pieceLength = Pieces[index].Length;
				int pieceWidth = Pieces[index].Width;

				// Move left
				if ((double)(pieceLength + 1) / 2 < Pieces[index].X)
				{
					double unfitnessBeforeMove = Unfitness;

					Pieces[index].X -= 0.5;

					ComputeUnfitness();

					// Undo move
					if (Unfitness > unfitnessBeforeMove)
					{
						Pieces[index].X += 0.5;
						Unfitness = unfitnessBeforeMove;
					}
				}

				// Move down
				if ((double)(pieceWidth + 1) / 2 < Pieces[index].Y)
				{
					double unfitnessBeforeMove = Unfitness;

					Pieces[index].Y -= 0.5;

					ComputeUnfitness();

					// Undo move
					if (Unfitness > unfitnessBeforeMove)
					{
						Pieces[index].Y += 0.5;
						Unfitness = unfitnessBeforeMove;
					}
				}

				// Move right
				if (Pieces[index].X + (double)(pieceLength + 1) / 2 < PaperLength)
				{
					double unfitnessBeforeMove = Unfitness;

					Pieces[index].X += 0.5;

					ComputeUnfitness();

					// Undo move
					if (Unfitness >= unfitnessBeforeMove)
					{
						Pieces[index].X -= 0.5;
						Unfitness = unfitnessBeforeMove;
					}
				}

				// Move up
				if (Pieces[index].Y + (double)(pieceWidth + 1) / 2 < PaperWidth)
				{
					double unfitnessBeforeMove = Unfitness;

					Pieces[index].Y += 0.5;

					ComputeUnfitness();

					// Undo move
					if (Unfitness >= unfitnessBeforeMove)
					{
						Pieces[index].Y -= 0.5;
						Unfitness = unfitnessBeforeMove;
					}
				}
			}
		}

	} while (oldUnfitness != Unfitness);
}

bool Individual::operator==(const Individual& individual)
{
	for (size_t index = 0; index < Pieces.size(); ++index)
	{
		if (Pieces[index] != individual.Pieces[index])
			return false;
	}

	return true;
}

bool Individual::operator!=(const Individual& individual)
{
	for (size_t index = 0; index < Pieces.size(); ++index)
	{
		if (Pieces[index] != individual.Pieces[index])
			return true;
	}

	return false;
}

void Individual::SortPieces()
{
	std::sort(Pieces.begin(), Pieces.end(), ComparePieces());
}

void Individual::ComputeFitness()
{
	Fitness = 0;

	for (const auto& piece : Pieces)
	{
		if (piece.IsCut == true)
		{
			Fitness += piece.Value;
		}
	}
}

void Individual::ComputeUnfitness()
{
	Unfitness = 0;

	for (size_t i = 0; i < Pieces.size() - 1; i++)
	{
		if (Pieces[i].IsCut == true)
		{
			for (size_t j = i + 1; j < Pieces.size(); j++)
			{
				if (Pieces[j].IsCut == true)
				{
					double firstValue = fabs(Pieces[i].X - Pieces[j].X);
					firstValue -= static_cast<double>(Pieces[i].Length + Pieces[j].Length) / 2;

					double secondValue = fabs(Pieces[i].Y - Pieces[j].Y);
					secondValue -= static_cast<double>(Pieces[i].Width + Pieces[j].Width) / 2;

					double maxValue = std::max(firstValue, secondValue);

					Unfitness += std::max(-maxValue, 0.0);
				}
			}
		}
	}
}

void Individual::GeneratePieces(int amount, Piece piece)
{
	for (int index = 0; index < amount; ++index)
	{
		piece.GenerateCoordinates(PaperLength, PaperWidth);
		Pieces.push_back(piece);
	}
}

std::ostream& operator<<(std::ostream& out, const Individual& individual)
{
	out << "Fitness: " << individual.Fitness << std::endl;
	out << "Unfitness: " << individual.Unfitness << std::endl;

	for (const auto& piece : individual.Pieces)
	{
		out << piece << std::endl;
	}

	return out;
}