#include "cub3d.h"

void *ft_realloc(void *ptr, size_t current_size, size_t new_size)
{
	char		*old_ptr;
	char		*new_ptr;
	size_t	i;

	old_ptr = (char *)ptr;
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
		new_ptr[i] = old_ptr[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}

t_vector *vector_create(size_t initial_capacity)
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

int vector_push_back(t_vector *current, char *new_symbol)
{
	char		**temp_symbols;
	size_t	new_size;

	if (current->capacity == current->length)
	{
		if (current->length == 0)
			new_size = 1;
		else
			new_size = current->length * 2;
		temp_symbols = (char **)ft_realloc(current->symbols, current->length * sizeof(char *), new_size * sizeof(char *));
		if (temp_symbols == NULL)
			return (1);
		current->symbols = temp_symbols;
		current->length = new_size;
	}
	current->symbols[current->capacity] = new_symbol;
	current->capacity++;
	if (current->capacity < current->length)
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