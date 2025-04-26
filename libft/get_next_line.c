/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:19:33 by ampocchi          #+#    #+#             */
/*   Updated: 2025/02/22 21:00:26 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*inox[4096];
	char		*line;
	int			newline_pos;
	char		*temp;

	if (!inox[fd])
		inox[fd] = ft_strdup("");
	inox[fd] = read_and_store(fd, inox[fd]);
	if (!inox[fd] || ft_strlen(inox[fd]) == 0)
	{
		ft_free(&inox[fd]);
		return (NULL);
	}
	newline_pos = ft_search_pos(inox[fd], '\n');
	if (newline_pos == -1)
	{
		line = ft_strdup(inox[fd]);
		ft_free(&inox[fd]);
		return (line);
	}
	line = ft_substr_gnl(inox[fd], 0, newline_pos + 1);
	temp = ft_strdup(inox[fd] + newline_pos + 1);
	free(inox[fd]);
	inox[fd] = temp;
	return (line);
}

char	*read_and_store(int fd, char *inox)
{
	char	*buffer;
	int		bytes_read;
	int		newline_pos;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	newline_pos = ft_search_pos(inox, '\n');
	while (bytes_read > 0 && newline_pos == -1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read > 0)
			inox = ft_strjoin_gnl(bytes_read, inox, buffer);
		else if (bytes_read < 0)
		{
			free(inox);
			free(buffer);
			return (NULL);
		}
		newline_pos = ft_search_pos(inox, '\n');
	}
	free(buffer);
	return (inox);
}

void	ft_free(char **inox)
{
	free(*inox);
	*inox = NULL;
}

int	ft_search_pos(const char *s, int c)
{
	int		i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (unsigned char)c)
			return (i);
		i++;
	}
	return (-1);
}
