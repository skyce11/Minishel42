/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:07:21 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 10:13:35 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_list(t_env **list)
{
	t_env	*tmp;

	while (*list)
	{
		tmp = (*list)->next;
		ft_memfree((*list)->var_name);
		ft_memfree((*list)->value_var);
		ft_memfree(*list);
		*list = tmp;
	}
	*list = NULL;
}

void	change_path(t_tools *tools)
{
	char	*aux;

	aux = ft_strdup(tools->pwd);
	free(tools->old_pwd);
	tools->old_pwd = aux;
	free(tools->pwd);
	tools->pwd = getcwd(NULL, sizeof(NULL));
}

int	check_valid_identifier(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
		|| c == '\'' || c == '\"' || c == ' ' || c == ',' || c == '.'
		|| c == ':' || c == '/' || c == '{' || c == '}' || c == '+'
		|| c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
		|| c == '~' || c == '=' || c == '-' || c == '?' || c == '&'
		|| c == '*');
}
