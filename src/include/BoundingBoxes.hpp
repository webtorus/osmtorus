#ifndef BOUNDING_BOXES_HPP
#define BOUNDING_BOXES_HPP

/**
 * T = type du contenu des BoundingBoxes
 * L = Nombre de lignes
 * C = Nombre de colonnes
 */

#include <cmath>
#include <cstddef>
#include <set>

using namespace std;

template<typename T, unsigned int L, unsigned int C>
class BoundingBoxes
{
	public:
		BoundingBoxes(): min_x(0), min_y(0), max_x(0), max_y(0)
		{

		}

		BoundingBoxes(double min_x, double min_y, double max_x, double max_y)
		: min_x(min_x), min_y(min_y), max_x(max_x), max_y(max_y)
		{

		}

		const set<T>* get(unsigned int l, unsigned int c) const
		{
			if (l >= L || c >= C) {
				return NULL;
			}

			return &content[l][c];
		}

		set<T>* get(unsigned int l, unsigned int c)
		{
			if (l >= L || c >= C) {
				return NULL;
			}

			return &content[l][c];
		}

		set<T>* getBoxOf(const double x, const double y)
		{
			int l = floor((double) L * (x - min_x) / (max_x - min_x));
			int c = floor((double) C * (y - min_y) / (max_y - min_y));
	
			return get(l, c);
		}

		void getBoxesOf(const double x, const double y, unsigned int spread, set<T>& result) const
		{
			int l = floor((double) L * (x - min_x) / (max_x - min_x));
			int c = floor((double) C * (y - min_y) / (max_y - min_y));

			for (unsigned int i = l - spread; i <= l + spread; i++) {
				for (unsigned int j = c - spread; j <= c + spread; j++) {
					const set<T>* current = get(i, j);
					if (NULL != current) {
						result.insert(current->begin(), current->end());
					}
				}
			}
		}

		double min_x;
		double min_y;
		double max_x;
		double max_y;

	private:
		set<T> content[L][C];
};

#endif
