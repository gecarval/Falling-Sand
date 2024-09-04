/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:25:48 by gecarval          #+#    #+#             */
/*   Updated: 2024/08/09 18:03:49 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_and_set_null(char **ptr)
{
	if (ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

static ssize_t	read_from_fd(int fd, char **extra_chars)
{
	ssize_t		n;
	char		*buf;
	char		*tmp;

	n = 1;
	while (n > 0 && !ft_strchr(*extra_chars, '\n'))
	{
		buf = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
		if (buf == NULL)
			return (-1);
		n = read(fd, buf, BUFFER_SIZE);
		if (n == -1 || (n == 0 && (*extra_chars == NULL)))
		{
			free_and_set_null(extra_chars);
			free_and_set_null(&buf);
			return (-1);
		}
		buf[n] = '\0';
		tmp = ft_strdup(*extra_chars);
		free_and_set_null(extra_chars);
		*extra_chars = ft_strjoin(tmp, buf);
		free_and_set_null(&tmp);
		free_and_set_null(&buf);
	}
	return (n);
}

static char	*get_current_line(char **extra_chars, ssize_t *i)
{
	ssize_t		len;
	char		*line;

	*i = 0;
	while ((*extra_chars)[*i] != '\n' && (*extra_chars)[*i] != '\0')
		(*i)++;
	if ((*extra_chars)[*i] == '\n')
		(*i)++;
	line = (char *)malloc((*i + 1) * sizeof(char));
	if (!line)
		return (NULL);
	len = 0;
	while (len < *i)
	{
		line[len] = (*extra_chars)[len];
		len++;
	}	
	line[len] = '\0';
	return (line);
}

static void	save_extra_char(char **extra_chars, ssize_t *i)
{
	char		*tmp;

	if ((*extra_chars)[*i - 1] == '\0')
		tmp = ft_strdup(*extra_chars + *i - 1);
	else
		tmp = ft_strdup(*extra_chars + *i);
	free_and_set_null(extra_chars);
	*extra_chars = ft_strdup(tmp);
	free_and_set_null(&tmp);
}

char	*ft_get_next_line(int fd)
{
	ssize_t		i;
	static char	*extra_chars = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	if (read_from_fd(fd, &extra_chars) == -1)
		return (NULL);
	if (extra_chars[0] == '\0')
	{
		free_and_set_null(&extra_chars);
		return (NULL);
	}
	line = get_current_line(&extra_chars, &i);
	save_extra_char(&extra_chars, &i);
	return (line);
}
