/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-behc <mel-behc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 17:16:34 by mel-behc          #+#    #+#             */
/*   Updated: 2021/12/09 11:57:00 by mel-behc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*m_buf(char *buf)
{
	char	*temp;
	int		i;	

	i = 0;
	while (buf[i] != '\n' && buf[i])
		i++;
	temp = ft_strdup(&buf[i + 1]);
	buf[i + 1] = '\0';
	return (temp);
}

static char	*m_reste(char *rs, char **str)
{
	char	*temp;
	int		i;

	i = 0;
	while (rs[i] != '\n' && rs[i])
		i++;
	temp = ft_strdup(&rs[i + 1]);
	rs[i + 1] = '\0';
	*str = ft_strjoin(str, &rs);
	return (temp);
}

static int	ft_read(int fd, char **buf)
{
	int	i;

	if (read(fd, *buf, 0) < 0)
		return (-1);
	*buf = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!*buf)
		return (-1);
	i = read(fd, *buf, BUFFER_SIZE);
	(*buf)[i] = '\0';
	if (i == -1 || i == 0)
	{
		free(*buf);
		*buf = NULL;
	}
	return (i);
}

static char	*ft_line(int fd, char **temp, char **reste)
{
	char	*buf;
	char	*line;
	int		rd;

	buf = ft_strdup("");
	line = ft_strdup("");
	rd = 1;
	while (rd > 0 && !ft_strchr(buf, '\n'))
	{	
		line = ft_strjoin(&line, &buf);
		rd = ft_read(fd, &buf);
	}
	if (rd > 0 && buf && ft_strchr(buf, '\n'))
	{
		*temp = m_buf(buf);
		line = ft_strjoin(&line, &buf);
	}
	if (!*line && (!*reste || !**reste) && rd <= 0)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	char		*str;
	char		*temp;
	char		*str_free;
	static char	*reste[65535];

	temp = NULL;
	if (fd < 0 || fd >= 65535)
		return (NULL);
	str = ft_strdup("");
	str_free = str;
	if (!reste[fd])
	{
		str = ft_line(fd, &temp, &reste[fd]);
		free(str_free);
	}
	else if (!ft_strchr(reste[fd], '\n'))
	{
		str = ft_line(fd, &temp, &reste[fd]);
		free(str_free);
		str = ft_strjoin(&reste[fd], &str);
	}
	else
		temp = m_reste(reste[fd], &str);
	reste[fd] = temp;
	return (str);
}
