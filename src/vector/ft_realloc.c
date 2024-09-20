# include "vector.h"

void	*ft_realloc(void *ptr, size_t current_size, size_t new_size)
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