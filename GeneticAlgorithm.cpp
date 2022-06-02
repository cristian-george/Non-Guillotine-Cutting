#include "GeneticAlgorithm.h"

Individual GeneticAlgorithm::BestIndividual;
std::vector<Individual> GeneticAlgorithm::Individuals;
int GeneticAlgorithm::generationCounter;

void GeneticAlgorithm::Start(int populationSize, int numberOfGenerations)
{
    Individuals.clear();
    generationCounter = 0;

	SeedPopulation(populationSize);

	while (generationCounter < numberOfGenerations)
	{
		// Selection
		const std::pair<Individual, Individual>& selectedParents = Selection();
		const Individual& parent1 = selectedParents.first;
		const Individual& parent2 = selectedParents.second;

		// Crossover
		Individual child = Crossover(parent1, parent2);

		// Mutation
		Mutation(child);

		// Compute fitness and unfitness
		child.ComputeFitness();
		child.ComputeUnfitness();

		// Improve child
		Improve(child);

		// Replace one individual by the child
		PopulationReplacement(child);

		// Setting the best individual
		for (const Individual& individual : Individuals)
		{
			if (individual.Unfitness == 0 && individual.Fitness > BestIndividual.Fitness)
			{
				BestIndividual = individual;
			}
		}
	}
}

void GeneticAlgorithm::SeedPopulation(int populationSize)
{
	std::ifstream fin("input.txt");

	int paperLength, paperWidth;
	fin >> paperLength >> paperWidth;

	int amount, type = 1;

	std::vector<std::pair<Piece, int>> pieces;

	while (fin >> amount)
	{
		Piece piece;
		fin >> piece;
		piece.Type = type;
		piece.IsCut = true;

		pieces.push_back({ piece, amount });
		++type;
	}

	for (int index = 0; index < populationSize; ++index)
	{
		Individual individual(paperLength, paperWidth, pieces);
		Individuals.push_back(individual);
	}
}

const std::pair<Individual, Individual> GeneticAlgorithm::Selection()
{
	std::random_device rd;
	std::mt19937_64 generator(rd());
	std::uniform_int_distribution<int> distrib(0, Individuals.size() - 1);

	int indexParent1 = distrib(generator);
	int indexParent2 = distrib(generator);

	while (indexParent2 == indexParent1)
	{
		indexParent2 = distrib(generator);
	}

	return std::make_pair(Individuals[indexParent1], Individuals[indexParent2]);
}

const Individual GeneticAlgorithm::Crossover(const Individual& parent1, const Individual& parent2)
{
	Individual child;

	child.PaperLength = parent1.PaperLength;
	child.PaperWidth = parent1.PaperWidth;

	std::random_device rd;
	std::mt19937_64 generator(rd());
	std::uniform_real_distribution<double> distrib(0, 1);

	for (size_t index = 0; index < parent1.Pieces.size(); ++index)
	{
		double probability = distrib(generator);

		if (probability <= 0.5)
		{
			child.Pieces.push_back(parent1.Pieces[index]);
		}
		else
		{
			child.Pieces.push_back(parent2.Pieces[index]);
		}
	}

	return child;
}

void GeneticAlgorithm::Mutation(Individual& child)
{
	generationCounter++;
	if (generationCounter % 10 == 0)
	{
		child.Mutation();
	}
}

void GeneticAlgorithm::Improve(Individual& child)
{
	child.Improve();
}

void GeneticAlgorithm::PopulationReplacement(Individual& child)
{
	child.SortPieces();

	bool canReplace = true;

	for (size_t index = 0; index < Individuals.size() && canReplace == true; ++index)
	{
		Individuals[index].SortPieces();
		if (child == Individuals[index])
		{
			canReplace = false;
		}
	}

	if (canReplace)
	{
		std::vector<int> indexesGroup1;
		std::vector<int> indexesGroup2;
		std::vector<int> indexesGroup3;
		std::vector<int> indexesGroup4;

		for (int index = 0; index < Individuals.size(); ++index)
		{
			//group 1
			if (child.Fitness > Individuals[index].Fitness &&
				child.Unfitness < Individuals[index].Unfitness)
			{
				indexesGroup1.push_back(index);
				continue;
			}

			//group 4
			if (child.Fitness < Individuals[index].Fitness &&
				child.Unfitness > Individuals[index].Unfitness)
			{
				indexesGroup4.push_back(index);
				continue;
			}

			//group 2
			if (child.Unfitness < Individuals[index].Unfitness)
			{
				indexesGroup2.push_back(index);
				continue;
			}

			//group 3
			indexesGroup3.push_back(index);

		}

		int indexIndividualToDelete = -1;

		std::random_device rd;
		std::mt19937_64 generator(rd());
		std::uniform_int_distribution<int> distrib;

		if (indexesGroup1.size() > 0)
		{
			distrib = std::uniform_int_distribution<int>(0, indexesGroup1.size() - 1);
			int indexGroup = distrib(generator);

			indexIndividualToDelete = indexesGroup1[indexGroup];
		}
		else if (indexesGroup2.size() > 0)
		{
			distrib = std::uniform_int_distribution<int>(0, indexesGroup2.size() - 1);
			int indexGroup = distrib(generator);
			indexIndividualToDelete = indexesGroup2[indexGroup];
		}
		else if (indexesGroup3.size() > 0)
		{
			distrib = std::uniform_int_distribution<int>(0, indexesGroup3.size() - 1);
			int indexGroup = distrib(generator);
			indexIndividualToDelete = indexesGroup3[indexGroup];
		}
		else if (indexesGroup4.size() > 0)
		{
			distrib = std::uniform_int_distribution<int>(0, indexesGroup4.size() - 1);
			int indexGroup = distrib(generator);
			indexIndividualToDelete = indexesGroup4[indexGroup];
		}

		Individuals.erase(Individuals.begin() + indexIndividualToDelete);

		Individuals.push_back(child);
	}
}
