# include "vector.h"

void	vector_print(t_vector *vector)
{
	size_t i = 0;
			printf("helppppppp %zu: %s\n", i + 1, vector->symbols[i]);
		printf("len: %zu\n", vector->length);

	while (i < vector->capacity)
	{
		printf("Line %zu: %s\n", i + 1, vector->symbols[i]);
		printf("Total lines: %zu\n", vector->capacity + 1);
		printf("len: %zu\n", vector->length);
		i++;
	}
}
