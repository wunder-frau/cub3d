# include "vector.h"

void	vector_free(t_vector *vector)
{
	size_t i = 0;

	while (i < vector->capacity)
	{
		free(vector->symbols[i]);
		i++;
	}
	free(vector->symbols);
	free(vector);
}
