# include "vector.h"

static void	vector_erase_at(t_vector *vector, size_t index, char **remaining_symbs)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < index)
	{
		remaining_symbs[i] = vector->symbols[i];
		i++;
	}
	j = i + 1;
	while (j < vector->capacity)
	{
		remaining_symbs[i] = vector->symbols[j];
		i++;
		j++;
	}
	free(vector->symbols);
	vector->symbols = remaining_symbs;
	vector->length--;
	vector->capacity--;
	vector->symbols[vector->capacity] = NULL;
}

char	*vector_get_at(t_vector *vector, size_t index)
{
	char	**remaining_symbs;
	char	*removed_symb;

	if (index >= vector->capacity)
		return (NULL);
	remaining_symbs = (char **)malloc(sizeof(char *) * (vector->length - 1));
	if (remaining_symbs == NULL)
		return (NULL);
	removed_symb = vector->symbols[index];
	printf("extracted:%s\n", removed_symb);
	vector_erase_at(vector, index, remaining_symbs);
	return (removed_symb);
}