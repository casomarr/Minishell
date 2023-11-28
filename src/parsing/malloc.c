/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:08:27 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/28 20:27:13 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*The first two conditions are here to avoid counting the spaces between
quotes, since those won't be deleted.*/
int	count_spaces(char *line)
{
	int	i;
	int	spaces;

	i = 0;
	spaces = 0;
	while (line[i])
	{
		if (line[i] == '\'' && quotes_can_close(line, i) == true)
		{
			i++;
			while (line[i] != '\'')
				i++;
		}
		if (line[i] == '\"' && quotes_can_close(line, i) == true)
		{
			i++;
			while (line[i] != '\"')
				i++;
		}
		if (line[i] == ' ' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			spaces++;
		i++;
	}
	return (spaces);
}

/*Malloc for the new command line that replaces the original
command line after all the extra spaces are deleted.*/
char	*erase_spaces_malloc(char *line)
{
	char	*new_line;
	int		spaces;

	spaces = count_spaces(line);
	new_line = ft_calloc(ft_strlen(line) - spaces + 2, sizeof(char));
	if (!new_line)
	{
		perror("Error");
		return (NULL); //il faut qd meme un return qd on utilise perror??
	}
	return (new_line);
}

/*Malloc for the joinstr found in the ft_joinstr_minishell function.*/
char	*joinstr_minishell_malloc(char *line, int len, char *str, char type)
{
	char	*new_str;

	if (type == '\'' || type == '\"')
	{
		if (str == NULL)
			new_str = ft_calloc(size_of_command(line, len, STR) \
			+ 1, sizeof(char));
		else
			new_str = ft_calloc((ft_strlen(str) + \
			size_of_command(line, len, STR) + 1), sizeof(char));
	}
	else
	{
		if (str == NULL)
			new_str = ft_calloc(size_of_command(line, len, CMD) \
			+ 1, sizeof(char));
		else
			new_str = ft_calloc((ft_strlen(str) \
			+ size_of_command(line, len, CMD) + 1), sizeof(char));
	}
	if (!new_str)
		return (NULL);
	return (new_str);
}

char	*empty_strings_malloc(char *line)
{
	int		i;
	int		size;
	char	*new_line;

	i = 0;
	size = 0;
	while (line[i])
	{
		if (line[i] == '\'' && line[i + 1] == '\'')
			i += 2;
		else if (line[i] == '\"' && line[i + 1] == '\"')
			i += 2;
		else
		{
			size++;
			i++;
		}
	}
	new_line = ft_calloc(size + 2, sizeof(char));
	return (new_line);
}
