/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:25:43 by karl              #+#    #+#             */
/*   Updated: 2023/11/01 18:56:53 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

// A FIX unset bash --posix regarder les arguments acceptes

/* Reproduces the unset command */
int	ft_unset(t_element *cmd_list, t_env *env)
{
	t_env *tmp;
	
	tmp = env;
	while (cmd_list && cmd_list->next)
	{
		if (!ft_is_valid_key_var(cmd_list->next->content) || ft_strchr(cmd_list->next->content, '='))
		{
			ft_putstr_fd("unset: ", STDOUT_FILENO);
			ft_putstr_fd(cmd_list->next->content, STDOUT_FILENO);
			ft_putendl_fd(" not a valid identifier", STDOUT_FILENO);
			return (0);
		}
		else
		{
			if (is_key_in_env(tmp, cmd_list->next->content) == true)
			{
				tmp = find_value_with_key_env(tmp, cmd_list->next->content);
				if (tmp)
					ft_delete_node(&env, tmp);
			}
		}
		cmd_list = cmd_list->next;
	}
	return (1);
}

void printenv(t_env *head) // A EFFACER A LA FIN
{
	int i = 0;

	while (i <= 3)
	{
		// if (i != 0)
		// 	printf("prev cmd = %s\n", head->prev->content);
		printf("--------------------------------\n");
		printf("key = %s\n", head->key);
		printf("value = %s\n", head->value);
		printf("--------------------------------\n");
		// if (head->next != NULL)
		// 	printf("next cmd = %s\n", head->next->content);
		head = head->next;
		i++;
	}
}

/* Delete the node passed in parameters */
void	ft_delete_node(t_env **head, t_env *to_delete)
{	
	t_env	*tmp;
	tmp = to_delete;
	if (!to_delete)
		return ;
	if (to_delete->next && to_delete->prev == NULL)
	{
		(*head) = to_delete->next;
		(*head)->prev = NULL;
	}
	else if (to_delete->next && to_delete->prev)
	{
		tmp = to_delete->next;
		tmp->prev = to_delete->prev;
		to_delete->prev->next = to_delete->next;
	}
	else if (to_delete->next == NULL && to_delete->prev)
	{
		tmp = to_delete->prev;
		tmp->next = NULL;
	}
	free(to_delete->key);
	to_delete->key = NULL;
	free(to_delete->value);
	to_delete->value = NULL;
	free(to_delete);
	to_delete = NULL;
	return ;
}

