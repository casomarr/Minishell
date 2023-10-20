/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/10/20 17:28:42 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
#include "../libft/libft.h"

//faire perror("Error") plutot que des printf pour toutes les fonctions qui utilisent errno
//utiliser ft_putstr_fd au lieu de printf

void	ft_welcome(void)
{
	printf("%s%s\n\n", BGRE, PC);
	printf("%s", WHT);
}

/*Deleted the /mnt/nfs/homes/casomarr/ part*/
char	*home_path_simplified(char *absolute_path, t_env *env_list)
{
	char	*path_from_home;
	char	*temp;
	t_env	*user;
	int		i;
	int		j;
	int		start;

	i = 0;
	start = 1;
	path_from_home = NULL;
	user = find_value_with_key_env(env_list, "USER");
	//je compares chaque element entre slashes au nom du user
	while(absolute_path[i])
	{
		if(absolute_path[i + 1] == '/')
		{
			temp = NULL;
			temp = strlcpy_middle(temp, absolute_path, start, i); // je copies absolute path jusqu a i dans temp
			start = i + 2; //begining of next word for next round
			if (ft_strncmp(temp, user->value, ft_strlen(user->value)) == 0 && ft_strlen(user->value) == ft_strlen(temp)) //je compares temp a user
			{
				//si sont les memes alors je mets dans path from home toutle reste de absolute path
				j = 0;
				i+=2;
				path_from_home = malloc(sizeof(char) * (ft_strlen(absolute_path) - i + 2));
				while (absolute_path[i])
					path_from_home[j++] = absolute_path[i++];
				path_from_home[j] = '\0';
				//printf("path = [%s]\n", path_from_home);
				free(temp);
				return (path_from_home);
			}
			free(temp);
		}
		i++;
	}
	return (NULL);
}

//PROTEGER TOUS MES MALLOCS!! --> avec perror

int main (int argc, char **argv, char **env)
{
	char                *line;
	char                *new_line;
	struct sigaction    signal;
	t_env				*env_list;
	t_env				*new_pwd;
	t_element			*cmd_list;
	t_element			*temp;
	t_pipe				*exec;
	
	
	
	exec = ft_calloc(1, sizeof(t_pipe));
	if (!exec)
	{
		perror("exec");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&signal.sa_mask);
	// signal.sa_flags = SA_SIGINFO;
	signal.sa_flags = SA_RESTART;
	signal.sa_handler = &signal_handler;
	if (sigaction(SIGINT, &signal, NULL) == -1 || \
	sigaction(SIGQUIT, &signal, NULL) == -1)
		return (EXIT_FAILURE);

	(void)argv;
	if (argc != 1)
	{
		printf("Error\nNo arguments accepted: run again with ./minishell\n"); //putsr
		return (EXIT_FAILURE);
	}
	ft_welcome();
	env_list = put_env_in_list(env);
	env_list->env = env;
	using_history();
	line = NULL;

//--------------------------------
	prompt(env_list);
	line = readline("$ ");
//--------------------------------
	while (1)
	//while (is_this_command(line, "exit") == false) // while (1) et mettre line = readline("$ ") tout en haut de la while
	//et mettre en commentaire le line = readline("$ ") du bas de la while
	{
//-----------------------------------------------------
//MODIFIER A CHAQUE TOUR DE WHILE LA VALEUR DE LA KEY PWD DE ENV!!!! CA REGLERA LE PROMPT
// + C ETS NECESSAIRE POUR LA FONCTION ENV SINON PWD SERA FAUX!
// 		new_pwd = find_value_with_key_env(env_list, "PWD");
// /* 		if (new_pwd->value != NULL)
// 			free(new_pwd->value); */
// 		new_pwd->value = malloc(ft_strlen(pwd(NO_PRINT)));
// 		new_pwd->value = ft_strlcpy(new_pwd->value, pwd(NO_PRINT), ft_strlen(pwd(NO_PRINT)));
//-----------------------------------------------------

		if (feof(stdin)) // pour ctrl + D?? // ne le comprend pas
		{
			printf("CTRL + D detected\n");
			final_free(line, env_list);
			return (EXIT_SUCCESS);
		}
		add_history(line);


	/*J'envoie new_line au lieu de line aux fonctions qui suivent
	car sur bash qd on fait flèche du haut on retrouve la commande
	telle qu'elle avait été écrite alors qu'ici on la modifiait*/
		new_line = erase_spaces(line);
		new_line = line_errors_and_fix(new_line, env_list);
	/*SI line_errors_and_fix TROUVE DES ERREURS IL NE FAUDRAIT PAS ENTRER DANS PARSING*/
		cmd_list = parsing(new_line, env_list);
		//ft_redirect(cmd_list); // a finir
    printlist_test(cmd_list);
		ft_execute(cmd_list, env_list, exec);
		//echo(cmd_list);
		//cd(cmd_list, env_list);
    //pwd(PRINT);
		free(new_line); //en commentaire pour tests avec dollar
		free_cmd_list(cmd_list);
//--------------------------------
		prompt(env_list);
		line = readline("$ ");
//--------------------------------
	}
	
	final_free(line, env_list);
	return (EXIT_SUCCESS);
}
