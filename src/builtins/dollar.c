/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:47 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/17 12:04:48 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"


// A GERER
/*

	bash:
	--> si $? utilise deux fois (ou plus) de suite -> exit code de 127

*/

/*
1. Checks if the $ is followed by alphanumeric
2. Puts what follows the $ into the variable key_to_find.
3. Cheks that the key exists in env_list (and key_in_env "takes its rank" if true).
4. Replaces the content by the key value.
*/
char	*dollar(char *content, t_env *env_list)
{
	/*Il manque à gérer l'option $?*/
	char	*key_to_find;
	t_env	*key_in_env;

	key_to_find = ft_calloc(ft_strlen(content) - 1, sizeof(char)); //pq j avais mis -1??
	key_to_find = strlcpy_middle(key_to_find, content, 1, ft_strlen(content));
 	if (is_key_in_env(env_list, key_to_find) == false)
	{
		free(content);
		content = NULL;
		//content = "\0"; //et non \n car deja un \n a la fin de la fonction echo
	}
	else
	{
		key_in_env = find_value_with_key_env(env_list, key_to_find);
		free(content);
		content = ft_calloc(ft_strlen(key_in_env->value), sizeof(char));
		content = strlcpy_middle(content, key_in_env->value, 0, ft_strlen(key_in_env->value));
	}
	free(key_to_find);
	return (content);
}

void	ft_dollar_question_mark()
{
	
	printf("---->Exit : %d\n", g_signals.exit_status);
	
	ft_putstr_fd("bash: ", STDOUT_FILENO);
	ft_putstr_fd(ft_itoa(g_signals.exit_status), STDOUT_FILENO);
	ft_putendl_fd(": command not found", STDOUT_FILENO);
	// FREE ITOA

	// 	si c'est $?, cas special
	// les signaux prennent 125 + l'int que rend le signal
	// CTRL + C = 5 + 125 = 130
	// 127 command not found (+2)
}
