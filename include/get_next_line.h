/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:18:54 by ampocchi          #+#    #+#             */
/*   Updated: 2025/04/26 18:45:32 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE	5
# endif

char	*get_next_line(int fd);
void	ft_free(char **inox);
int		ft_search_pos(const char *s, int c);
char	*read_and_store(int fd, char *inox);

char	*ft_strjoin_gnl(int bytes_read, char *s1, char *s2);
char	*ft_substr_gnl(char const *s, unsigned int start, size_t len);

#endif
