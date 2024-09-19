#ifndef VECTOR_H
# define VECTOR_H

# include <stdlib.h>

typedef struct s_vector
{
	char	**symbols;
	size_t	capacity;
	size_t	length;
}	t_vector;

/* vector.c */
t_vector	*vector_create(size_t size);
int				vector_push_back(t_vector *current, char *new_symbols);
void			free_vector(t_vector *vector);
char			*vector_get_at(t_vector *vector, size_t array_index);

#endif