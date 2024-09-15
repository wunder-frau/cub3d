#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"

int main(void)
{
	t_vector *my_vector = vector_new(2);
	if (!my_vector)
	{
		printf("Failed to create vector.\n");
		return 1;
	}

	vector_add_back(my_vector, strdup("Hello"));
	vector_add_back(my_vector, strdup("World"));
	vector_add_back(my_vector, strdup("This"));
	vector_add_back(my_vector, strdup("is"));
	vector_add_back(my_vector, strdup("a"));
	vector_add_back(my_vector, strdup("test!"));

	printf("Vector contents:\n");
	for (size_t i = 0; i < my_vector->capacity; i++)
	{
		if (my_vector->symbols[i] != NULL)
			printf("%s\n", my_vector->symbols[i]);
	}

	free_vector(my_vector);
	return 0;
}
