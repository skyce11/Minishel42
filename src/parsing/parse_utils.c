/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:39:33 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/12 17:24:59 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_dup_after(char *s, char c)
{
	char	**pp;
	size_t	len;
	size_t	j;
	size_t	aux;

	j = 0;
	if (!s)
		return (NULL);
	len = count_tokens(s, c);
	if (len == 0)
		return (NULL);
	pp = ft_calloc(len +1, sizeof(s));
	if (!pp)
		return (NULL);
	aux = 0;
	while (j < len)
	{
		pp[j] = word_dup_after(s, c, aux);
		if (!pp[j])
			return (ft_free_matrix(pp));
		aux = dup_after_pos(s, c, aux);
		j++;
	}
	return (pp);
}

char *process_quotes(const char *input) {
    size_t buf_size = strlen(input) + 1;
    char *result = malloc(buf_size);
    if (!result) {
        perror("malloc");
        return NULL;
    }

    size_t j = 0; // indice d'écriture dans le buffer résultat

    enum { NORMAL, SIMPLE, DOUBLE } state = NORMAL;

    for (size_t i = 0; input[i] != '\0'; i++) {
        char c = input[i];

        /* Dans les états NORMAL et DOUBLE, on traite l'expansion de variable */
        if ((state == NORMAL || state == DOUBLE) && c == '$') {
            size_t var_start = i + 1;
            if (input[var_start] == '\0') {
                /* '$' en fin de chaîne */
                if (j + 1 >= buf_size) {
                    buf_size *= 2;
                    result = realloc(result, buf_size);
                    if (!result) return NULL;
                }
                result[j++] = c;
                continue;
            }
            /* Cas spécial : $? */
            if (input[var_start] == '?') {
                char numbuf[16];
                snprintf(numbuf, sizeof(numbuf), "%d", 0);  // vous pouvez remplacer 0 par l'exit_status
                size_t num_len = strlen(numbuf);
                while (j + num_len >= buf_size) {
                    buf_size *= 2;
                    result = realloc(result, buf_size);
                    if (!result) return NULL;
                }
                strcpy(result + j, numbuf);
                j += num_len;
                i++;  // on saute le '?' déjà traité
                continue;
            }
            /* Extraction du nom : lettres, chiffres et '_' uniquement */
            size_t var_end = var_start;
            while (isalnum((unsigned char)input[var_end]) || input[var_end] == '_') {
                var_end++;
            }
            size_t var_len = var_end - var_start;
            char *varName = malloc(var_len + 1);
            if (!varName) {
                free(result);
                return NULL;
            }
            strncpy(varName, input + var_start, var_len);
            varName[var_len] = '\0';
            char *varValue = getenv(varName);
            free(varName);
            if (!varValue)
                varValue = "";  // si la variable n'existe pas
            size_t value_len = strlen(varValue);
            while (j + value_len >= buf_size) {
                buf_size *= 2;
                result = realloc(result, buf_size);
                if (!result) return NULL;
            }
            strcpy(result + j, varValue);
            j += value_len;
            i = var_end - 1;
            continue;
        }

        /* Gestion de la machine à états pour les quotes */
        if (state == NORMAL) {
            if (c == '\'') {
                state = SIMPLE;  // démarre un segment en quotes simples (la quote n'est pas copiée)
            } else if (c == '\"') {
                state = DOUBLE;  // démarre un segment en double quotes
            } else {
                if (j + 1 >= buf_size) {
                    buf_size *= 2;
                    result = realloc(result, buf_size);
                    if (!result) return NULL;
                }
                result[j++] = c;
            }
        }
        else if (state == SIMPLE) {
            if (c == '\'') {
                state = NORMAL;  // fin du segment en quotes simples
            } else {
                if (j + 1 >= buf_size) {
                    buf_size *= 2;
                    result = realloc(result, buf_size);
                    if (!result) return NULL;
                }
                result[j++] = c;
            }
        }
        else if (state == DOUBLE) {
            if (c == '\"') {
                state = NORMAL;  // fin du segment en double quotes
            } else {
                if (j + 1 >= buf_size) {
                    buf_size *= 2;
                    result = realloc(result, buf_size);
                    if (!result) return NULL;
                }
                result[j++] = c;
            }
        }
    }
    result[j] = '\0';
    return result;
}

char **process_quotes_array(char **inputs) {
    size_t count = 0;
    while (inputs[count] != NULL) {
        count++;
    }

    char **result_array = malloc((count + 1) * sizeof(char *));
    if (!result_array) {
        perror("malloc");
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        result_array[i] = process_quotes(inputs[i]);
        if (!result_array[i]) {
            for (size_t j = 0; j < i; j++) {
                free(result_array[j]);
            }
            free(result_array);
            return NULL;
        }
    }
    result_array[count] = NULL;
    return result_array;
}

static int	count_valid_args(char **args)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (args[0] && args[0][0] != '<' && args[0][0] != '>')
		count++;
	i = 1;
	while (args[i])
	{
		if (args[i][0] != '<' && args[i][0] != '>' &&
			args[i - 1][0] != '<' && args[i - 1][0] != '>')
		{
			count++;
		}
		i++;
	}

	i = 0;
	// while (args[i])
	// {
		// process_quotes(args[i]);
		// printf("test: %s\n", test);
	// 	delete_quotes(args[i], '\"');
	// 	delete_quotes(args[i], '\'');
		// i++;
	// }
	return (count);
}

// Copiar argumentos válidos a la nueva matriz
static char	**copy_valid_args(char **args, int count)
{
	char	**res;
	int		i;
	int		j;

	res = ft_calloc(count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	j = 0;
	i = 0;
	if (args[0] && args[0][0] != '<' && args[0][0] != '>')
		res[j++] = ft_strdup(args[0]);
	i = 1;
	while (args[i])
	{
		if (args[i][0] != '<' && args[i][0] != '>' &&
			args[i - 1][0] != '<' && args[i - 1][0] != '>')
		{
			res[j++] = ft_strdup(args[i]);
		}
		i++;
	}
	return (res);
}

// Función principal refactorizada
char	**parse_args(char *s)
{
	char	**aux;
	char	**res;
	int		count;

	aux = split_minishell(s, ' ');
	if (!aux)
		return (NULL);
	count = count_valid_args(aux);
	res = process_quotes_array(aux);
	res = copy_valid_args(aux, count);
	ft_free_matrix(aux);
	return (res);
}

/// @brief free an tab
/// @param arr
void	ft_free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
