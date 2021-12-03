/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   day3.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ahorling <ahorling@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/03 17:21:38 by ahorling      #+#    #+#                 */
/*   Updated: 2021/12/03 20:18:39 by ahorling      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>

int BUFFER_SIZE = 20;

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strdup(const char *str)
{
	int		i;
	int		leng;
	char	*dupe;

	i = 0;
	leng = 0;
	while (str[i] != '\0')
	{
		leng++;
		i++;
	}
	i = 0;
	dupe = (char *)malloc((leng + 1) * sizeof(char));
	if (dupe == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		dupe[i] = str[i];
		i++;
	}
	dupe[i] = '\0';
	return (dupe);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*fullstr;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	fullstr = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (fullstr == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		fullstr[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		fullstr[i + j] = s2[j];
		j++;
	}
	fullstr[i + j] = '\0';
	return (fullstr);
}

char	*add_to_buffer(char *buffer, char *filler)
{
	char	*new_buffer;

	if (!buffer && !filler)
		return (NULL);
	if (buffer == NULL)
		return (ft_strdup(filler));
	else
		new_buffer = ft_strjoin(buffer, filler);
	if (new_buffer == NULL)
		return (NULL);
	free(buffer);
	buffer = NULL;
	return (new_buffer);
}


static int	contains_newline(char *string)
{
	size_t	i;

	i = 0;
	if (string == NULL)
		return (0);
	while (string[i] != '\0')
	{
		if (string[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static int	fill_buffer(int fd, char **buffer)
{
	int		file;
	char	*filler;

	filler = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	file = 1;
	while (contains_newline(*buffer) == 0 && file > 0)
	{
		file = read(fd, filler, BUFFER_SIZE);
		if (file < 0)
		{
			free(filler);
			filler = NULL;
			return (-1);
		}
		filler[file] = '\0';
		*buffer = add_to_buffer(*buffer, filler);
		if (*buffer == NULL)
			return (-1);
	}
	free(filler);
	filler = NULL;
	if (file > 0)
		file = 1;
	return (file);
}

static char	*pull_line(char *buffer)
{
	size_t	i;
	char	*line;

	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
		i++;
	line = (char *)malloc((i + 1) * sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
	{
		line[i] = buffer[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*edit_buffer(char *buffer)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
		i++;
	if (buffer[i] != '\0')
		i++;
	while (buffer[i] != '\0' && i < ft_strlen(buffer))
	{
		buffer[j] = buffer[i];
		i++;
		j++;
	}
	buffer[j] = '\0';
	return (buffer);
}

int	get_next_line(int fd, char **line)
{
	static char	*buffer;
	int			return_value;

	if (BUFFER_SIZE < 1 || fd < 0 || !line || fd > 1024)
		return (-1);
	return_value = fill_buffer(fd, &buffer);
	if (return_value == -1)
	{
		*line = NULL;
		return (-1);
	}
	*line = pull_line(buffer);
	if (!*line)
		return (-1);
	buffer = edit_buffer(buffer);
	if (return_value == 0 && buffer != NULL)
	{
		free(buffer);
		buffer = NULL;
	}
	return (return_value);
}

static void	print_error(int fd, char **line)
{
	if (fd < 0)
		printf("Error in file opening\n");
	if (!line)
		printf("Error in line creation (line does not exist)\n");
	if (BUFFER_SIZE <= 0)
		printf("Error in BUFFER_SIZE, value smaller than 0\n");
	else
		printf("Error in reading file or a Malloc function\n");
}

int	main()
{
	char	*text;
	int		fd;
	int		count;
	int		return_value;
	char	**array;
	int		i;
	int		j;
	int		compare;
	char	gamma[13];
	char	epsilon[13];

	j = 0;
	i = 0;
	return_value = 1;
	count = 1;
	fd = open("day 3 input.txt", O_RDONLY);
	while (return_value != 0)
	{
		return_value = get_next_line(fd, &text);
		if (return_value == -1)
		{
			print_error(fd, &text);
			break;
		}
		count++;
		free(text);
	}
	lseek(fd, 0, SEEK_SET);
	return_value = 1;
	array = (char **)malloc(count * sizeof(char));
	for (i = 0; i < count; i++)
		array[i] = (char *)malloc(13 * sizeof(char));
	i = 0;
	while (return_value != 0)
	{
		return_value = get_next_line(fd, &text);
		array[i] = text;
		i++;
	}
//	printf("%s\n", array[1]);
	i = 0;
	compare = 0;
	j = 0;
  	while (j < 12)
	{
		while(i < count)
		{
			if (array[i][j] == '1')
			compare++;
			i++;
		}
		printf("comparison for digit %d is: %d\n", j, compare);
		if (compare > 500)
		{
			gamma[j] = '1';
			epsilon[j] = '0';
		}
		else if (compare < 500)	
		{
			gamma[j] = '0';
			epsilon[j] = '1';
		}
		j++;
		i = 0;
		compare = 0;
	}
	j = 0;
	gamma[12] = '\0';
	epsilon[12] = '\0';
	printf("G: %s\nE: %s\n", gamma, epsilon);
	close(fd);
	return (0);
}