/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:36:13 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/17 11:04:58 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* exit:
ALBAN 3 cas

si le premier argument de exit n'est pas valide:
- soit pas numerique
- soit superieur a INT_MAX 
meme si il y en a deux
"exit abc def" OR "exit 999999999999"
bash: exit: abc: numeric argument required
--> ON EXIT

si le premier arg marche( >= 0 && < INT_MAX) mais que le 2eme ne marche pas
"exit 57 df"
bash: exit: too many arguments
--> ON EXIT PAS

exit (int n)

return (n % 255)

*/
int	ft_exit(t_element *cmd, t_env **env, t_pipe *exec)
{
	t_element	*head;

	head = cmd;
	if ((cmd->next && cmd->next->type != ARGUMENT) || cmd->prev)
		return (0);
	if (cmd->next && (!ft_is_num(cmd->next->content) || !ft_atoi_check(cmd->next->content) || cmd->next->type != ARGUMENT))
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(cmd->next->content, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		printf("cmd->content 1 = %s\n", cmd->content);
		exit_free(head, env, exec);
		close(0);
		close(1);
		close(2);
		exit(0); // envoye exit_code
	}
	if (cmd->next && cmd->next->next)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		if (ft_is_num(cmd->next->content))
		{
			ft_putendl_fd("bash: exit: too many arguments", STDERR_FILENO);
			return (0);
		}
	}
	// if (!cmd->next && !cmd->prev)
	// {
	// 	ft_putendl_fd("exit", STDOUT_FILENO);
	// 	exit_free(cmd, env, exec);
	// 	close(0);
	// 	close(1);
	// 	close(2);
	// 	exit(0); // exit(exit_code)
	// }
	//exit_free(cmd, env, entries); seg fault double free
	// on ne devrait pas arriver la normalement
	close(0);
	close(1);
	close(2);
	printf("cmd->content 2 = %s\n", cmd->content);
	exit_free(head, env, exec);
	exit(0); // exit(exit_code) g.status_exitcode
	return (0);
}
