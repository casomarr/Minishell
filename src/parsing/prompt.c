/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 18:36:55 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/05 17:53:13 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	home_path_simplified_loop(char *absolute_path, t_env *user, int *i, \
char **path_from_home)
{
	int				j;
	char			*temp;
	static size_t	start;

	if ((*i) == 0)
		start = 1;
	if (absolute_path[(*i) + 1] == '/')
	{
		temp = NULL;
		temp = strlcpy_middle(temp, absolute_path, start, (*i));
		start = (*i) + 2;
		if (compare(temp, user->value) == true)
		{
			j = 0;
			(*i) += 2;
			(*path_from_home) = ft_calloc(ft_strlen(absolute_path) \
			- (*i) + 2, sizeof(char));
			while (absolute_path[(*i)])
				(*path_from_home)[j++] = absolute_path[(*i)++];
			(*path_from_home)[j] = '\0';
		}
		free(temp);
	}
}

/*Deletes the /mnt/nfs/homes/casomarr/ part by comparing each word
in between slashes to the username (user->value). If the word between
slashes (stored in temp) == username, we store in path_from_home the
rest of the path. This will be used in the prompt function, since the
prompt only prints the path from home (unless we are located previous 
the home folder)*/
char	*home_path_simplified(char *absolute_path, t_env *env_list)
{
	char	*path_from_home;
	t_env	*user;
	int		i;

	i = 0;
	path_from_home = NULL;
	if (!is_key_in_env(env_list, "USER"))
		return (NULL);
	user = find_value_with_key_env(env_list, "USER");
	while (absolute_path[i])
	{
		home_path_simplified_loop(absolute_path, user, &i, &path_from_home);
		if (path_from_home != NULL)
			return (path_from_home);
		i++;
	}
	return (NULL);
}

/*Starts at the length of pwd and goes backwards until a
slash is found : it represents the beggining of the
last word of the path. We start at ft_strlen(PWD)-2 to skip
the slash at the end of pwd.*/
int	get_beggining_of_last_word(void)
{
	int		i;
	char	*temp;

	temp = pwd(NO_PRINT);
	if (!temp)
		return (-1);
	i = ft_strlen(temp) - 2;
	while (i > 0)
	{
		if (temp[i - 1] == '/')
			break ;
		i--;
	}
	free(temp);
	return (i);
}

/*Adaptive prompt :
IF : if we are in the user folder, print nothing (readline will add
"$ " to it).
First ELSE IF : if we are in home folder, print only the home folder.
Second ELSE IF : when path is further than the user folder (inside it),
print from user folder (excluded) to current folder.
ELSE : when we are before the home folder, print a slash.*/
char	*ft_prompt(t_env *env_list, int option)
{
	char	*word;
	char	*prompt;
	char	*path;
	int		ret;

	ret = 0;
	word = NULL;
	if (!is_key_in_env(env_list, "USER") || \
	!is_key_in_env(env_list, "PWD"))
		return ("\0");
	path = pwd(NO_PRINT);
	if (!path)
		return ("\0");
	ret = get_beggining_of_last_word();
	if (ret < 0)
		return ("\0");
	word = strlcpy_middle(word, path, get_beggining_of_last_word(), \
	ft_strlen(path));
	ft_prompt2(&prompt, word, env_list, path);
	if (option == PRINT)
		printf("%s", prompt);
	free(path);
	free(word);
	return (prompt);
}

void	ft_prompt2(char **prompt, char *word, t_env *env_list, char *path)
{
	t_env	*user;
	t_env	*gpath;

	user = NULL;
	gpath = NULL;
	if (is_key_in_env(env_list, "USER") == true)
		user = find_value_with_key_env(env_list, "USER");
	if (is_key_in_env(env_list, "PWD") == true)
		gpath = find_value_with_key_env(env_list, "PWD");
	if (user != NULL && compare(word, user->value) == true)
		(*prompt) = "";
	else if (gpath != NULL && ft_strncmp(word, "homes", ft_strlen(word) - \
	get_beggining_of_last_word() + 1) == 0)
		(*prompt) = strlcpy_middle((*prompt), gpath->value, 1, \
		ft_strlen(gpath->value) - 1);
	else if (user != NULL && compare(word, user->value) == false && \
	is_user_in_path(path, env_list) == true)
		(*prompt) = home_path_simplified(path, env_list);
	else
		(*prompt) = "/";
}
