#include "cub3d.h"

void *ft_realloc(void *ptr, size_t current_size, size_t new_size)
{
	void		*new_ptr;
	size_t	i;

	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	i = 0;
	while (i < current_size)
	{
		((char *)new_ptr)[i] = ((char *)ptr)[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}

t_vector *vector_new(size_t size)
{
	t_vector *new_vector;

	new_vector = (t_vector *)malloc(sizeof(t_vector));
	if (new_vector == NULL)
		return (NULL);
	new_vector->symbols = (char **)malloc((size + 1) * sizeof(char *));
	if (new_vector->symbols == NULL)
	{
		free(new_vector);
		return (NULL);
	}
	new_vector->length = size;
	new_vector->capacity = 0;
	new_vector->symbols[0] = NULL;
	return (new_vector);
}

int vector_add_back(t_vector *current, char *new_symbols)
{
	if (current->capacity == current->length)
	{
		size_t new_size = current->length * 2;
		char **new_symbols = (char **)ft_realloc(current->symbols, current->length * sizeof(char *), new_size * sizeof(char *));
		if (new_symbols == NULL)
			return (1);
		current->symbols = new_symbols;
		current->length = new_size;
	}
	current->symbols[current->capacity] = new_symbols;
	current->capacity++;
	current->symbols[current->capacity] = NULL;
	return (0);
}

void free_vector(t_vector *vector)
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

static void	rearrange_vector(t_vector *vector, size_t index, char **new_symbols)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < index)
	{
		new_symbols[i] = vector->symbols[i];
		i++;
	}
	j = i + 1;
	while (j < vector->capacity)
	{
		new_symbols[i] = vector->symbols[j];
		i++;
		j++;
	}
	free(vector->symbols);
	vector->symbols = new_symbols;
	vector->capacity = vector->capacity - 1;
	vector->symbols[vector->capacity] = NULL;
	vector->length = vector->length - 1;
}

char	*extract_array(t_vector *vector, size_t array_index)
{
	char	**new_symbols;
	char	*extracted_array;

	if (array_index >= vector->capacity)
		return (NULL);
	new_symbols = (char **)malloc(sizeof(char *) * (vector->length - 1));
	if (new_symbols == NULL)
		return (NULL);
	extracted_array = vector->symbols[array_index];
	printf("extracted:%s\n", extracted_array);
	rearrange_vector(vector, array_index, new_symbols);
	return (extracted_array);
}