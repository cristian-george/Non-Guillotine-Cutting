#pragma once
#include "Piece.h"

class ComparePieces
{
public:
	bool operator()(const Piece& piece1, const Piece& piece2) const
	{
		if (piece1.Type == piece2.Type)
		{
			if (piece1.IsCut == piece2.IsCut)
			{
				if (piece1.X == piece2.X)
				{
					if (piece1.Y < piece2.Y)
						return true;

					return false;
				}

				if (piece1.X < piece2.X)
					return true;

				return false;
			}

			if (!piece1.IsCut && piece2.IsCut)
				return true;

			return false;
		}

		if (piece1.Type < piece2.Type)
			return true;

		return false;
	}
};