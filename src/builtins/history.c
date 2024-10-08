/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:43:38 by carolina          #+#    #+#             */
/*   Updated: 2023/12/02 13:18:46 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Reproduces the history command
First if : if we ask to print more entries than there exist,
print all existing entries.*/
void	history(int option, int len)
{
	HISTORY_STATE	*info;
	HIST_ENTRY		**list;

	info = history_get_history_state();
	list = history_list();
	if (len > info->length)
		len = info->length;
	if (option == FT_HISTORY && len == -1)
		print_all_hist(info, list);
	else if (option == FT_HISTORY && len != -1)
		print_hist_until_len(info, list, len);
	else if (option == FREE_HISTORY)
		free_history(info, list);
}

void	print_all_hist(HISTORY_STATE *info, HIST_ENTRY **list)
{
	int		i;
	char	*nb;

	i = 0;
	while (i < info->length)
	{
		nb = ft_itoa(i + 1);
		ft_putstr_fd(nb, STDOUT_FILENO);
		ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putendl_fd(list[i]->line, STDOUT_FILENO);
		free(nb);
		i++;
	}
}

void	print_hist_until_len(HISTORY_STATE *info, HIST_ENTRY **list, int len)
{
	int		reverse;
	int		i;
	char	*nb;

	i = 0;
	reverse = info->length - len;
	while (i < len)
	{
		nb = ft_itoa(i + 1);
		ft_putstr_fd(nb, STDOUT_FILENO);
		ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putendl_fd(list[reverse]->line, STDOUT_FILENO);
		free(nb);
		i++;
		reverse++;
	}
}

void	free_history(HISTORY_STATE *info, HIST_ENTRY **list)
{
	int	i;

	i = 0;
	while (i < info->length)
	{
		free_history_entry(list[i]);
		i++;
	}
	free (info);
	free (list);
}

bool	history_option(t_element *cmd)
{
	if (cmd->next && (cmd->next->type < 2 || \
	(ft_is_num(cmd->next->content) == false || \
	ft_atoi_check(cmd->next->content) == false)))
	{
		msg_error_bash(3, cmd->next->content);
		return (false);
	}
	if (cmd->next && ft_is_num(cmd->next->content) && cmd->next->next && \
	cmd->next->next->type < 3)
	{
		msg_error_bash(6, "history: too many arguments");
		return (false);
	}
	return (true);
}
