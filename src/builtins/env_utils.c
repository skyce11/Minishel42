/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:08:42 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/17 15:04:35 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_name(char *fullenv)
{
	int	i;

	i = 0;
	while (fullenv[i] && fullenv[i] != '=' && fullenv[i] != ' ')
		i++;
	return (ft_substr(fullenv, 0, i));
}

char	*get_env_value(char *fullenv)
{
	int	i;

	i = 0;
	while (fullenv[i] && fullenv[i] != '=' && fullenv[i] != ' ')
		i++;
	if (fullenv[i] == '\0')
		return (ft_strdup(""));
	if (fullenv[i + 1] == '\"')
		return (ft_substr(fullenv, i + 2, ft_strlen(fullenv) - i));
	else
		return (ft_substr(fullenv, i + 1, ft_strlen(fullenv) - i));
}

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (!lst || !(*lst))
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*ft_lstnew_env(char *name, char *value, int alloc)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	new_node->var_name = name;
	new_node->value_var = value;
	if (alloc)
	{
		new_node->var_name = ft_strdup(name);
		new_node->value_var = ft_strdup(value);
		if (!new_node->var_name || !new_node->value_var)
		{
			ft_memfree(new_node->var_name);
			ft_memfree(new_node->value_var);
			ft_memfree(new_node);
			return (NULL);
		}
	}
	new_node->next = NULL;
	return (new_node);
}

// void	ft_lst_env_init(t_env **env, char **envp)
// {
// 	t_env	*new;
// 	int		i;
// 	char	*var_name;
// 	char	*var_value;

// 	new = NULL;
// 	i = -1;
// 	while (++i < ft_arraylen(envp))
// 	{
// 		var_name = get_env_name(envp[i]);
// 		var_value = get_env_value(envp[i]);
// 		new = ft_lstnew_env(var_name, var_value, 1);
// 		ft_lstadd_back_env(env, new);
// 		ft_memfree(var_name);
// 		ft_memfree(var_value);
// 	}
// }
