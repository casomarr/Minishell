/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:25:43 by karl              #+#    #+#             */
/*   Updated: 2023/12/07 13:58:46 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Reproduces the unset command */
int	ft_unset(t_element *cmd, t_env **env)
{
	t_env	*tmp;

	while (cmd && cmd->type != PIPE)
	{
		tmp = *env;
		while (cmd && cmd->type >= 3)
			cmd = cmd->next;
		if (cmd && is_key_in_env(tmp, cmd->content) == true)
		{
			tmp = find_value_with_key_env(tmp, cmd->content);
			if (tmp)
			{
				if (ft_delete_node_env(env, tmp) == 1)
				{
					*env = NULL;
					return (1);
				}
			}
		}
		if (cmd)
			cmd = cmd->next;
	}
	return (1);
}

/* Delete the node passed in parameters */
int	ft_delete_node_env(t_env **head, t_env *to_delete)
{
	if (!to_delete)
		return (2);
	if (!to_delete->prev && !to_delete->next)
	{
		if (compare(to_delete->key, "EXIT_STATUS") == false)
			free(to_delete->key);
		ft_free_null(to_delete);
		return (1);
	}
	if (to_delete->prev)
		to_delete->prev->next = to_delete->next;
	else
		*head = to_delete->next;
	if (to_delete->next)
		to_delete->next->prev = to_delete->prev;
	if (compare(to_delete->key, "EXIT_STATUS") == false)
		free(to_delete->key);
	ft_free_null(to_delete);
	return (0);
}

/* Frees and sets it to NULL */
void	ft_free_null(t_env *to_delete)
{
	if (to_delete->value != NULL)
	{
		free(to_delete->value);
		to_delete->value = NULL;
	}
	free(to_delete);
	to_delete = NULL;
}
