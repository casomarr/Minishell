/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:11:19 by carolina          #+#    #+#             */
/*   Updated: 2023/12/08 01:15:57 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*Libraries*/
# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <strings.h>
# include <stdbool.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/uio.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>
# include <sys/ioctl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

/*Signals macros*/
# define IN_PROMPT 0
# define IN_COMMAND 1
# define IN_HEREDOC 2
# define QUIT_HEREDOC 3
# define QUIT 4
# define _XOPEN_SOURCE 700

/*General macros*/
# define COMMAND 0
# define OPTION 1
# define ARGUMENT 2
# define INFILE 3
# define OUTFILE 4
# define HEREDOC 5
# define OUTFILE_APPEND 6
# define PIPE 7

# define KEY 0
# define VALUE 1
# define CMD 2
# define STR 3

# define NONE 0
# define ENV 1
# define ECHO 2
# define HISTORY 3
# define CD 4

# define FT_HISTORY 0
# define FREE_HISTORY 1

# define PRINT 0
# define NO_PRINT 1

# define SPACES 0
# define QUOTES 1

# define HD_ERR	"bash: warning: \
here-document delimited by end-of-file (wanted `"

/* Colors macros */
# define YELLOW	"\033[33m"
# define GREEN	"\033[32m"
# define RESET	"\033[0m"
# define BYEL "\e[1;33m"
# define BRED "\e[1;31m"
# define BWHT "\e[1;37m"
# define BCYN "\e[1;36m"
# define WHT "\e[0;37m"
# define BGRE "\033[1m\033[32m"

extern int	g_location;

/* Command list
--> content represents the command
--> type is the command type
--> cmd_tab is an array of all commands (type 0)
*/
typedef struct s_element
{
	char				*content;
	char				*hd_filename;
	int					type;
	bool				builtin;
	bool				change;
	struct s_element	*prev;
	struct s_element	*next;
}	t_element;

/* Environment
--> key is a substring to find in value
--> value is the env as a whole
--> env is the env (for execve)
*/
typedef struct s_env
{
	char			*key;
	char			*value;
	int				fd_heredoc;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;

/* To handles pipes */
typedef struct s_pipe
{
	int		last_pid;
	int		pipe_nb;
	pid_t	*pid;
	char	**cmd_tab;
	char	**cmd_path;
	int		std_in;
	int		std_out;
	int		fd_temp;
	int		fd[2];
	char	**env;
	char	*line;
	char	*prompt;
	char	**env_execve;
}	t_pipe;

/*------------------MAIN FOLDER------------------*/
void		set_struct_null(t_env **env, t_element **cmd, t_pipe **exec);
t_pipe		*init_struct(t_pipe *exec, t_env **env_list, char **env);
int			ft_mini_prompt(t_env **env_list, char **path, t_pipe *exec, \
char **line);
void		line_null(char *line, t_env **env_list, t_pipe *exec);
void		line_not_null(char **line, t_element *cmd_list, t_env **env_list, \
t_pipe *exec);

/*------------------PARSING FOLDER------------------*/

/*Checks*/
bool		quotes_can_close(char *line, int i);
bool		is_builtin(char *cmd_content);
bool		is_user_in_path(char *path, t_env *env_list);
bool		is_in_line(char *big, char *little);

/*Cmd_types*/
int			cmd_type(char *command, int len);
void		cmd_type_loop(char *command, int *len, int *size);
int			key_and_value_type(char *command, int len, int type);

/*Commands*/
bool		is_cmd(char *buffer, char *command);
int			size_of_command(char *command, int len, int type);
bool		is_cmd_in_line(char *line, char *cmd);
void		ft_builtins_history(t_element *cmd);
void		ft_builtins(t_element *cmd, t_env **env_list, t_pipe *exec);

/*Env_list*/
int			put_env_in_list_loop(char **env, t_env **current, int line);
t_env		*put_env_in_list(char **env);
t_env		*find_value_with_key_env(t_env *env_list, char *key);
bool		is_key_in_env(t_env *env_list, char *key);

/*Erase_spaces*/
void		erase_spaces_loop(char *line, char **new_line, int *i, int *j);
char		*erase_spaces(char *line);
char		*erase_empty_strings(char *line);
void		erase_empty_strings_loop(char *line, char **new_line, int *i, \
int *j);
bool		if_consec_quotes(char *line, int *i);

/*Errors*/
bool		line_errors_and_fix(char *line);
bool		first_character_error(char *line);
bool		redirecters_error(char *line);
int			redirecters_error_norm(char *line, int i);
void		pipe_error(char *line);

/*Errors2*/
void		and_error(char *line);
bool		pipe_double_or_eof(char *line);

/*Free*/
void		exit_free(t_element *cmd_list, t_env **env_list, t_pipe *exec);
void		ctrld_free(char *line, char *prompt, t_env *env, t_pipe *exec);
void		free_cmd_arr(t_pipe *exec);
int			free_cmd_list(t_element *cmd_list);
int			free_env_list(t_env *env_list);

/*Free_2*/
int			ft_delete_node_cmd(t_element **head, t_element *to_delete);
int			ft_delete_node_cmd_parent(t_element **head, t_element *to_delete);
int			free_cmd_list_parent(t_element *cmd_list);
void		ft_free_null_cmd(t_element *to_delete);
void		free_and_update(char *line, t_element *cmd_list, t_pipe *exec, \
t_env **env_list);
void		free_cmd_tab(t_pipe *exec);

/*Lstnew*/
t_element	*lstnew(char *line, int i, int type);
t_env		*lstnew_env(char *line, int i);

/*Malloc*/
int			count_spaces(char *line);
char		*erase_spaces_malloc(char *line);
char		*joinstr_minishell_malloc(char *line, int len, char *str, \
char type);

/*Determine_cmd*/
bool		option(char *line, size_t end, size_t start);
bool		heredoc(char *line, size_t start);
bool		infile(char *line, size_t start);
bool		outfile_append(char *line, size_t start);
bool		outfile(char *line, size_t start);

/*Determine_cmd_2*/
int			determine_command_type(char *line, size_t end, size_t start);

/*Parsing*/
t_element	*parsing(char *line, t_env *env_list);
int			parsing_loop(char *line, int *i, int *start, \
t_element **current_cmd);
void		builtin_fix(t_element **cmd_list);
void		parsing_types_fix(t_element **current, t_env *env_list);
int			parsing_if(t_element **current, t_element **cmd_list);

/*Parsing2*/
t_element	*parsing_initialisation(char *line, int *i, int *start);
void		parsing_advance_to_next_word(char *line, int *start, int *i);
int			skip_first_quote(char *line, int *i);
int			fill_content_loop(t_element **cur, char *line, int *i);
int			fill_content_loop_if(t_element **cur, char *line, int *i, int *j);

/*Parsing 3*/
void		parsing_initialize_next(t_element **current_cmd, char *line, int \
*i);
int			parsing_fix_dollar(t_element *current, t_env *env_list);
void		type_arg_after_cmd(t_element **current);
bool		no_cmd_before(t_element *current);
int			parsing_fix(t_element **cmd_list, t_env *env_list);

/*Prompt*/
void		home_path_simplified_loop(char *absolute_path, t_env *user, int *i, \
char **path_from_home);
char		*home_path_simplified(char *absolute_path, t_env *env_list);
int			get_beggining_of_last_word(void);
char		*ft_prompt(t_env *env_list, int option);
void		ft_prompt2(char **prompt, char *word, t_env *env_list, char *path);

/*Signal*/
int			set_signals(void);
void		sigint_handler(int signal);
void		sigquit_handler(int signal);

/*Utils*/
char		*ft_joinstr_minishell(char *line, int len, char *str, char type);
char		*ft_join_for_cd(char *line_begining, char *path);
char		*strlcpy_middle(char *dst, const char *src, size_t start, \
size_t end);
char		*ft_strjoin_free(char const *s1, char *s2);
void		str_join_fill(const char *s1, char *new_str, int *i);

/*Utils2*/
bool		ft_is_num(char *s);
bool		ft_atoi_check(char *str);
bool		compare(char *s1, char *s2);

/*------------------BUILT-INS FOLDER------------------*/

/*Builtins_errors*/
bool		no_option(t_element *cmd, t_element *head);
bool		cd_option(t_element *cmd, t_element *head);
bool		env_option(t_element *cmd);
bool		echo_option(t_element *cmd);
bool		check_next(t_element *cmd, int option);

/*Cd*/
void		cd_directory(char *path, t_env *env_list);
void		cd_home(t_env *env_list);
void		cd(t_element *current, t_env *env_list);

/*Dollar*/
int			initialize_values_loop(char *content, size_t *i, size_t *j, \
int *dollar_nb);
int			initialize_values(char *content);
void		one_dollar(char *content, char **ret, t_env *env);
char		*dollar(char *content, t_env *env_list);

/*Dollar2*/
void		new_key(size_t *i, char **key_to_find, char *content);
char		*replace_dollar(char *key_to_find, t_env *env_list);
void		text_before(char *content, char **ret);
char		*text_after(char *content, size_t *j);
void		free_and_last_dollar(char **key_to_find, char *content, char **ret);

/*Dollar3*/
int			first_values(size_t *end, char *content, size_t *tmp);
char		*first_key_to_find(char *content, size_t start, size_t end);
void		replace_dollar_in_ret(char **ret, char *key_to_find, t_env *env);
char		*final_values(size_t *start, char *content, size_t end);
void		multiple_dollars(char *content, char **ret, t_env *env);

/*Echo*/
bool		no_further_args(t_element *cmd);
void		echo(t_element *current);

/*Env*/
void		ft_env(t_env *env, t_element *cmd, int option);

/*Exit*/
void		ft_exit(t_element *cmd, t_env **env, t_pipe *exec);
void		ft_num_arg(t_element *cmd, t_env **env, t_pipe *exec, \
t_element *head);
void		ft_too_many_arg(t_element *cmd, t_env **env);
void		exit_cont(t_element *cmd, t_env **env, t_pipe *exec, \
t_element *head);
int			add_exit_status_in_env(t_env **env, int n);

/*Exit_2*/
bool		no_args_or_options(t_element *cmd);
bool		no_pipes_before(t_element *cmd);
void		exit_check_all(t_element *cmd, t_env **env, t_pipe *exec);

/*Export*/
int			ft_export(t_element *cmd_list, t_env **env);
bool		ft_is_valid_key_var(char *s);
char		**split_var(char *s);
void		join_new_var(t_env **env, char *key, char *value);
void		replace_var(t_env **env, char *key, char *value);

/*Export_2*/
void		ft_export_continued(t_element *cmd, t_env **env);
void		put_var_in_env(t_env **env, char *key, char *value);
int			ft_strchr_int(char *s, char c);
void		ft_assign_key_and_value(int i, char **ret, char *s);

/*History*/
void		history(int option, int len);
void		print_all_hist(HISTORY_STATE *info, HIST_ENTRY **list);
void		print_hist_until_len(HISTORY_STATE *info, HIST_ENTRY **list, \
int len);
void		free_history(HISTORY_STATE *info, HIST_ENTRY **list);
bool		history_option(t_element *cmd);

/*Pwd*/
char		*pwd(int option);
void		pwd_update_in_env(t_env **env_list);

/*Unset*/
int			ft_unset(t_element *cmd, t_env **env);
int			ft_delete_node_env(t_env **head, t_env *to_delete);
void		ft_free_null(t_env *to_delete);

/*-----------------EXECUTABLE FOLDER ------------------*/

/*Exec*/
void		ft_execute(t_element *cmd, t_env **env, t_pipe *exec);
void		single_command(t_element *cmd, t_env **env, t_pipe *exec);
int			ft_exit_status_single(t_env **env, int pid);
void		multiple_commands(t_element *cmd, t_env **env, t_pipe *exec);
int			ft_exit_status_multiple(t_env **env, t_pipe *exec);

/*Exec_2*/
int			ft_is_builtin(t_element *cmd, t_env **env, t_pipe *exec, \
int option);
void		ft_saint_olivier(t_element *cmd, t_env **env, t_pipe *exec);
void		handle_command(t_element *cmd, t_env **env, t_pipe *exec);

/*Exec_3*/
int			exec_command(t_element *cmd, t_env *env, t_pipe *exec);
int			ft_exec_slash(t_element *cmd, t_pipe *exec);
void		free_child(t_element *cmd, t_env **env, t_pipe *exec);

/*Exec_errors*/
void		msg_error(int err, char *s);
void		msg_error_bash(int err, char *s);
void		msg_error_bash_2(int err, char *s);

/*Exec_utils*/
int			get_size_cmd(t_element *cmd);
int			ft_count_pipes(t_element *cmd);
void		fill_array(t_element *cmd, t_pipe *exec);
char		**split_path(t_env *env_list);
char		*ft_strcpy(char *dst, char *src);

/*Exec_utils_2*/
char		**ft_transform_env(t_env *env);
char		*ft_strjoin_env(char const *s1, char const *s2);
char		*ft_get_command(char **path, char *argument);
bool		is_only_dots(char *s);

/*Pipe_and_dup*/
void		middle_pipes(t_element *cmd, t_env **env, t_pipe *exec);
void		last_pipe(t_element *cmd, t_env **env, t_pipe *exec);
void		middle_dup(t_element *cmd, t_env **env, t_pipe *exec);
void		last_dup(t_element *cmd, t_env **env, t_pipe *exec);

/*Redirect*/
int			ft_infile(char *filename);
int			ft_outfile(t_element *cmd);
int			ft_redirect(t_element *cmd);
int			ft_redir_while(t_element *cmd, t_element *tmp);
void		ft_top_of_list(t_element **cmd);

/*Redirect_utils*/
bool		ft_all_redir(t_element *cmd);
void		ft_only_create(t_element *cmd);
bool		only_create_outfile_or_append(t_element *cmd, int *fd);
void		ft_unlink(t_element *cmd);

/*Heredoc*/
bool		ft_heredoc(t_element *cmd, t_env *env);
int			heredoc_loop(t_element *cmd, t_env *env, char *words, int fd);

/*Heredoc_utils*/
void		free_dup_heredoc(char *words, int fd, int fd_heredoc);
char		*ft_palbon(t_element *cmd);
char		*ft_strjoin_free_s2(char *s1, char *s2);
char		*create_heredoc(char *safeword, int i, int *fd);

#endif