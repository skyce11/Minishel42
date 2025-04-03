/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migonzal <migonzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:09:11 by migonzal          #+#    #+#             */
/*   Updated: 2025/03/31 10:09:30 by migonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_variable(char *name, char *value, t_tools *tools);

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return (0);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	ft_arraylen(char **array)
{
	int	i;

	if (!array)
		return (0);
	while (array[i])
		i++;
	return (i);
}

void	ft_memfree(void *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}

void	add_arg_to_env(char *var, t_tools *tools)
{
	t_env	*env;
	char	*name;
	char	*value;

	if (!ft_strrchr(var, '='))
		return ;
	name = get_env_name(var);
	value = get_env_value(var);
	if (!check_variable(name, value, tools))
	{
		env = ft_lstnew_env(name, value, 0);
		ft_lstadd_back_env(&tools->env, env);
	}
}

static int	check_variable(char *name, char *value, t_tools *tools)
{
	t_env	*ptr;

	ptr = tools->env;
	if (!ptr)
		return (0);
	while (ptr)
	{
		if (!ft_strcmp(ptr->var_name, name))
		{
			ft_memfree(ptr->var_name);
			ft_memfree(ptr->value_var);
			ptr->var_name = name;
			ptr->value_var = value;
			return (1);
		}
		ptr = ptr->next;
	}
	return (0);
}
