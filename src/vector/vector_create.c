# include "vector.h"

t_vector	*vector_create(size_t initial_capacity)
{
	t_vector *new_vector;

	new_vector = (t_vector *)malloc(sizeof(t_vector));
	if (new_vector == NULL)
		return (NULL);
	new_vector->symbols = (char **)malloc((initial_capacity + 1) * sizeof(char *));
	if (new_vector->symbols == NULL)
	{
		free(new_vector);
		return (NULL);
	}
	new_vector->length = initial_capacity;
	new_vector->capacity = 0;
	new_vector->symbols[0] = NULL;
	return (new_vector);
}
