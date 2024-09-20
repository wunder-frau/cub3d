#ifndef VECTOR_H
# define VECTOR_H

# include <stdlib.h>
# include <stdio.h>

typedef struct s_vector
{
	char		**symbols;
	size_t	capacity;
	size_t	length;
}	t_vector;

void	*ft_realloc(void *ptr, size_t current_size, size_t new_size);
t_vector	*vector_create(size_t size);
int				vector_push_back(t_vector *current, char *new_symbols);
void			vector_free(t_vector *vector);
char			*vector_get_at(t_vector *vector, size_t array_index);

#endif