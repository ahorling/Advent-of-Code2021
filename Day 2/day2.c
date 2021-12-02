/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   day2.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ahorling <ahorling@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/02 19:30:30 by ahorling      #+#    #+#                 */
/*   Updated: 2021/12/02 21:13:18 by ahorling      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

static char	**freesplits(char **table, int tableindex)
{
	int	i;

	i = tableindex;
	while (i >= 0)
	{
		free(table[i]);
		i--;
	}
	free(table);
	return (NULL);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;
	char	*string;

	i = 0;
	string = (char *)s;
	if (s == NULL)
		return (NULL);
	if (strlen(string) < start)
		len = 0;
	else if ((strlen(string) + start) < len)
		len = strlen(string) + start;
	substr = (char *)malloc((len + 1) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	while (i < len)
	{
		substr[i] = string[i + start];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}


static int	numofsplits(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i] == c && s[i] != '\0')
		i++;
	if (i == strlen(s))
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] == c && s[i + 1] != c && i != strlen(s) - 1)
			count++;
		i++;
	}
	return (count + 1);
}

static char	*pullstring(char const *s, char c)
{
	int		i;
	int		skip;
	int		length;
	char	*pulledstring;

	i = 0;
	while (s[i] == c)
		i++;
	skip = i;
	while (s[i] != c && s[i] != '\0')
		i++;
	length = i;
	pulledstring = (ft_substr(s, skip, length - skip));
	return (pulledstring);
}

char	**ft_split(char const *s, char c)
{
	int		entries;
	int		i;
	char	**table;

	i = 0;
	if (s == NULL)
		return (NULL);
	entries = numofsplits(s, c);
	table = (char **)malloc((entries + 1) * sizeof(char *));
	if (table == NULL)
		return (NULL);
	while (i < entries)
	{
		table[i] = pullstring(s, c);
		if (table[i] == NULL)
			return (freesplits(table, i));
		while (*s == c)
			s++;
		s = memchr(s, c, strlen(s));
		i++;
	}
	table[i] = NULL;
	return (table);
}

int		main()
{
	FILE	*fp;
	char	*string;
	long	length;
	char	**array;
	int		forward[1000];
	int		up[1000];
	int		down[1000];
	int		i;
	int		totalforward;
	int		totalup;
	int		totaldown;

	totalforward = 0;
	totalup = 0;
	totaldown = 0;
	length = 0;
	fp = fopen("day 2 input.txt", "r");
	if(fp == NULL)
	{
		printf("error1");
		exit(1);
	}
	fseek (fp, 0, SEEK_END);
	length = ftell (fp);
	fseek (fp, 0, SEEK_SET);
	string = (char *)malloc(length * sizeof(char));
	if (string)
		fread(string, 1, length, fp);

	array = ft_split(string, '\n');
	for (int i = 0; i <= 1000; i++)
	{
		forward[i] = 0;
		up[i] = 0;
		down[i] = 0;
	}
	i = 0;
	while (i <= 999)
	{
		if (array[i][0] == 'f')
			forward[i] = atoi(&array[i][8]);
		else if (array[i][0] == 'u')
			up[i] = atoi(&array[i][3]);
		else if (array[i][0] == 'd')
			down[i] = atoi(&array[i][5]);
		i++;
	}
/*	
*/	for (i = 0; i <= 1000; i++)
		totalforward = forward[i] + totalforward;
	for (i = 0; i <= 1000; i++)
		totaldown = down[i] + totaldown;
	for (i = 0; i <= 1000; i++)
		totalup = up[i] + totalup;
	printf("horizontal: %d\n", totalforward);
	printf("depth down: %d\n", totaldown);
	printf("depth up: %d\n", totalup);
	printf("distance traveled: %d\n", (totalforward+2)*(totaldown-totalup));
	return (0);
}