/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:15 by yilin             #+#    #+#             */
/*   Updated: 2024/11/02 19:43:20 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_free(t_env )
{
	
}

void	free_all_shell(t_shell *content)
{
	// Check if ctx (shell context) exists before freeing its contents
	if (content)
	{
		if (content->exec)//exec
			builder_free(content->exec);
		if (content->env)//env
			env_free(content->env);
		if (content->pids)//pids
			free(content->pids);
		free (content);//content
	}
}

static int check_line_empty(char *line)
{
	if (line[0] == '\0')
		return (1);
	return (0);
}

//inside dup_edit_env:
//TODO : if env exists => edit the value, otherwise create it

t_env *dup_env(char *env[])
{
	int	i; // Index for iterating through the environment variables
	char *env_id; // Variable to hold the identifier of the environment variable(the part before the =)
	char *env_value; // Variable to hold the value of the environment variable(environment variable’s contents, after =)
	t_env *res_env; 
	t_env *tmp;

	i = 0;
	res_env = set_default_env();
	//Check if the environment pointer is NULL or points to an empty string
	// if (!env || (env && *env == NULL)) //envp points to a valid location; But the environment variable list is empty.
	// 	return (set_default_env());
	while (env[i])// Loop through each environment variable
	{
		env_id = get_env_id(env[i]);//get identifier
		env_value = get_env_value(env[i]);//get value
		tmp = create_env(env_id, env_value, ft_strdup(env[i])); // Create a new environment variable structure (tmp)
		//if fail -> return NULL
		if (!tmp)
			return (NULL);
		env_add_back(&res_env, tmp);
		i++;
	}
	return (res_env);
}
//Functions like export, unset, or env can rely on env_add_back to modify the environment. 
//

t_shell *init_shell(char *env[])//initialize & dup env to supershell
{
	t_shell *content; // Declare a pointer to a context structure
	
	content = malloc(sizeof(t_shell));// Allocate memory for the context structure
	if (!content)
		return (NULL);
	content->env = dup_env(env); // Duplicate the environment variables into the context
	if (!(content->env))
	{
		free(content);
		return (NULL);
	}
	////initialize everything
	content->default_in = STDIN_FILENO; // Set default input to standard input
	content->default_out = STDOUT_FILENO; // Set default output to standard output
	content->pids = NULL;
	content->exec = NULL;
	content->pid_count = 0;
	content->exec_count = 0;
	return (content);
}

int read_n_loop(t_shell *content)
{
	char *line;

	(void)content;
	line = NULL;
	while (1)
	{
		// sig_init_signals(); // Set up signal handling for each loop iteration
		line = readline(PROMPT);
		if (!line)
			break ;// Exit the loop to end the shell
		if (strncmp(line, "exit", 4) == 0)
			break ;//TESTER
		if (strncmp(line, "env", 3) == 0)
			display_env(content->env);//TESTER
		else if (check_line_empty(line) == 0)// check if line valid
		{
			add_history(line);//add history //REMEMBER TO CLEAR 
			// if (process_pipeline(content, line) != 0)//process pipeline
			// {
			// 	ft_putstr_fd("Parcing Error\n", 2);
			// 	content->exit_code = 2;
			// }
			line = NULL;//reset line
		}
		if (line)//if line not null -> free
			free(line);
	}
	return (0); //return if loop end ((i.e., on EOF or NULL input))
}

int main(int ac, char *av[], char *env[])
{
	t_shell *content;

	content = NULL;
	(void)ac;
	(void)av;
	content = init_shell(env);//initialize everthing to 0/NULL
	if (!content)
		return (EXIT_FAILURE);
	read_n_loop(content);//strat loop -> readline(PROMPT)
	free_all_shell(env);
	ft_putstr_fd("exit\n", 2);//or 1
	return (EXIT_SUCCESS);
}
