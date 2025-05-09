/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_error2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:26:27 by yilin             #+#    #+#             */
/*   Updated: 2025/01/07 18:12:52 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/** ERROR CD */
void	error_cd(int err_n, char *pathname)
{
	char	*err_msg;

	err_msg = strerror(err_n);
	if (pathname && ft_strcmp(pathname, "HOME") == 0)
	{
		ft_putstr_fd(P_NAME, STDERR_FILENO);
		ft_putstr_fd(": cd: ", STDERR_FILENO);
		ft_putstr_fd(pathname, STDERR_FILENO);
		ft_putstr_fd(" not set\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(P_NAME, STDERR_FILENO);
		ft_putstr_fd(": cd: ", STDERR_FILENO);
		ft_putstr_fd(pathname, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(err_msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
}

/** ERROR EXPORT */
void	error_export(char *input_line)
{
	ft_putstr_fd(P_NAME, STDERR_FILENO);
	ft_putstr_fd(": export: ", STDERR_FILENO);
	ft_putstr_fd(input_line, STDERR_FILENO);
	ft_putstr_fd(" : Not a valid identifier\n", STDERR_FILENO);
}

/** ERROR UNSET */
void	error_unset(char *input_line)
{
	ft_putstr_fd(P_NAME, STDERR_FILENO);
	ft_putstr_fd(": unset: ", STDERR_FILENO);
	ft_putstr_fd(input_line, STDERR_FILENO);
	ft_putstr_fd(" : Not a valid identifier\n", STDERR_FILENO);
}
