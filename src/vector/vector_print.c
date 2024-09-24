# include "vector.h"

void	vector_print(t_vector *vector)
{
	size_t i = 0;
	while (i < vector->capacity)
	{
		printf("Line %zu: %s\n", i + 1, vector->symbols[i]);
		i++;
	}
}
