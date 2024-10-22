
#include "../cub3d.h"

/**
 * Trims the identifier (NO, SO, EA, WE, F, C)
 * from the config line and removes trailing spaces and newlines.
 */
char	*trim_and_extract(char *str, unsigned int prefix_len)
{
	unsigned int	end;
	char			*trimmed;

	while (str[prefix_len] == ' ' || str[prefix_len] == '\n')
		prefix_len++;
	end = ft_strlen(str);
	while (end > prefix_len && (str[end - 1] == ' ' || str[end - 1] == '\n'))
		end--;
	trimmed = ft_substr(str, prefix_len, end - prefix_len);
	free(str);
	str = NULL;
	return (trimmed);
}