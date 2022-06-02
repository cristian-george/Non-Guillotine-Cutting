#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "Individual.h"

class GeneticAlgorithm
{
public:
	GeneticAlgorithm() = default;

	static void Start(int populationSize, int numberOfGenerations);

private:
	/* Generate an initial population and evaluate fitness and unfitness
	of individuals in the population */
	static void SeedPopulation(int populationSize);

	/* Select two individuals from the population  to be parents,
	parents were randomly selected */
	static const std::pair<Individual, Individual> Selection();

	/* Recombine (mate) parents to produce a single child,
	parents were combined using uniform crossover */
	static const Individual Crossover(const Individual& parent1,
									  const Individual& parent2);

	/* Mutate the child (1/10 children were mutated) */
	static void Mutation(Individual& child);

	/* Improve the child using the heuristic improvement scheme */
	static void Improve(Individual& child);

	/* Replace one member of the population by the child using the population replacement scheme,
	unless the child is a duplicate of a member of the population,
	in which case reject it */
	static void PopulationReplacement(Individual& child);

public:
	static Individual BestIndividual;
	static std::vector<Individual> Individuals;
	static int generationCounter;
};

