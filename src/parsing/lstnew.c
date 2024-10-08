/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstnew.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:59:32 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/06 12:06:53 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*To add a new node when creating the cmd_list.*/
t_element	*lstnew(char *line, int i, int type)
{
	t_element	*new;

	new = ft_calloc(1, sizeof(t_element));
	if (!new)
		return (NULL);
	new->content = ft_calloc(size_of_command(line, i, type) + 1, sizeof(char));
	if (!new->content)
		return (NULL);
	new->type = 0;
	new->builtin = false;
	new->change = true;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

/*To add a new node when creating the env_list.*/
t_env	*lstnew_env(char *line, int i)
{
	t_env	*new;

	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_calloc(size_of_command(line, i, KEY) + 1, sizeof(char));
	if (!new->key)
		return (NULL);
	new->value = ft_calloc(size_of_command(line, i, VALUE) + 1, sizeof(char));
	if (!new->value)
		return (NULL);
	new->prev = NULL;
	new->next = NULL;
	return (new);
}
