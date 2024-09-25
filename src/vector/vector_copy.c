#include "vector.h"

t_vector	*vector_copy(t_vector *vector)
{
	t_vector	*vec_cpy;
	char			*cpy_line;
	size_t		i;

	vec_cpy = vector_create(vector->capacity);
	if (vec_cpy == NULL)
		return (NULL);
	i = 0;
	while (i < vector->capacity)
	{
		cpy_line = ft_strdup(vector->symbols[i]);
		if (cpy_line == NULL)
			return (NULL);
		if (vector_push_back(vec_cpy, cpy_line) == 1)
		{
			vector_free(vec_cpy);
			return (NULL);
		}
		i++;
	}
	return (vec_cpy);
}