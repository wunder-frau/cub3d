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

int vector_add_back(t_vector *current, char *updated_symbols)
{
	if (current->capacity == current->length)
	{
		size_t new_size = current->length * 2;
		char **updated_symbols = (char **)ft_realloc(current->symbols, current->length * sizeof(char *), new_size * sizeof(char *));
		if (updated_symbols == NULL)
			return (1);
		current->symbols = updated_symbols;
		current->length = new_size;
	}
	current->symbols[current->capacity] = updated_symbols;
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

static void	adjust_vector_capacity(t_vector *vector, size_t index, char **updated_symbols)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < index)
	{
		updated_symbols[i] = vector->symbols[i];
		i++;
	}
	j = i + 1;
	while (j < vector->capacity)
	{
		updated_symbols[i] = vector->symbols[j];
		i++;
		j++;
	}
	free(vector->symbols);
	vector->symbols = updated_symbols;
	vector->length--;
	vector->capacity--;
	vector->symbols[vector->capacity] = NULL;
}

char	*get_element_from_vector(t_vector *vector, size_t index)
{
	char	**updated_symbols;
	char	*extracted_symbols;

	if (index >= vector->capacity)
		return (NULL);
	updated_symbols = (char **)malloc(sizeof(char *) * (vector->length - 1));
	if (updated_symbols == NULL)
		return (NULL);
	extracted_symbols = vector->symbols[index];
	printf("extracted:%s\n", extracted_symbols);
	adjust_vector_capacity(vector, index, updated_symbols);
	return (extracted_symbols);
}