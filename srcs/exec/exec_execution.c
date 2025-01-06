/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:12:01 by varodrig          #+#    #+#             */
/*   Updated: 2025/01/06 17:39:01 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_exec_path(char **paths, char *cmd)
{
	char	*path;
	char	*exec;
	int		i;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		exec = ft_strjoin(path, cmd);
		free(path);
		if (!access(exec, X_OK | F_OK))
		{
			ft_free_all(paths);
			return (exec);
		}
		free(exec);
		i++;
	}
	ft_free_all(paths);
	return (NULL);
}

char	*find_path(char *cmd, t_env *env)
{
	char	**paths;

	if (cmd && cmd[0] == '\0')
		return (NULL);
	if (!env || check_is_alr_path(cmd) == 1)
		return (ft_strdup(cmd));
	while (env && ft_strncmp("PATH=", env->env_line, 5) != 0)
		env = env->next;
	if (!env)
		return (ft_strdup(cmd));
	paths = ft_split(env->value, ':');
	return (get_exec_path(paths, cmd));
}

char	**env_format(t_env *env)
{
	char	**env_arr;
	int		size;
	int		i;

	size = ft_env_lstsize(env);
	env_arr = (char **)malloc(sizeof(char *) * (size + 1));
	if (!env_arr)
		return (NULL);
	i = 0;
	while (i < size && env)
	{
		env_arr[i] = ft_strdup(env->env_line);
		if (!env_arr[i])
		{
			arrs_free(env_arr);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

// 126 : Command found but cannot be executed(due to permission issues)
// 127 : Command not found(the file does not exist or is not in the PATH)
// int execve(const char *fichier, char *const argv[], char *const envp[])
// char	*args[] = {"/bin/ls", "-l", "/home", NULL};
// +2 for path and NULL
int	ft_execution(t_shell *ctx, t_exec *temp)
{
	int		args_nb;
	char	*path;
	char	**args;
	char	**env;

	if (!temp->cmd)
		return (0);
	path = find_path(temp->cmd, ctx->env);
	if (!path)
		return (err_execve(temp->cmd, errno), 4);
	env = env_format(ctx->env);
	if (!env)
		return (free(path), 4);
	args_nb = ft_args_lstsize(temp->args) + 2;
	args = malloc(sizeof(char *) * args_nb);
	if (!args)
		return (free(path), ft_free_all(env), 4);
	exec_args_create(temp, args_nb, args);
	if (execve(path, args, env) == -1)
		return (err_execve(path, errno), free(path), ft_free_all(env),
			free(args), -2);
	free(path);
	ft_free_all(env);
	free(args);
	return (0);
}
