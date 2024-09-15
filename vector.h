#ifndef VECTOR_H
# define VECTOR_H

# include <stdlib.h>

typedef struct s_vector
{
	char	**symbols;
	size_t	capacity;
	size_t	length;
}	t_vector;

/* vector_expand.c */
t_vector	*vector_new(size_t size);
int			vector_add_back(t_vector *current, char *new_symbols);
void		free_vector(t_vector *vector);

/* vector_decrease.c */
char		*extract_array(t_vector *vector, size_t array_index);
//void *vector_get(t_vector *vector, size_t index);

/* vector_copy.c */
t_vector	*copy_vector(t_vector *vector);

#endif