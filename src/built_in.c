#include "minishell.h"

int64_t	handle_built_in(t_token *token)
{
	int64_t	result;

	result = 0;
	if ((ft_strncmp(token->data, "echo", 4) == 0)
		&& ft_strlen(token->data) == 4)
		result = handle_echo(token);
	else if ((ft_strncmp(token->data, "cd", 2) == 0)
		&& ft_strlen(token->data) == 2)
		result = handle_cd(token);
	else if ((ft_strncmp(token->data, "pwd", 3) == 0)
		&& ft_strlen(token->data) == 3)
		result = handle_pwd();
	else if ((ft_strncmp(token->data, "export", 6) == 0)
		&& ft_strlen(token->data) == 7)
		result = handle_export(token);
	else if ((ft_strncmp(token->data, "unset", 5) == 0)
		&& ft_strlen(token->data) == 5)
		result = handle_unset(token);
	else if ((ft_strncmp(token->data, "env", 3) == 0)
		&& ft_strlen(token->data) == 3)
		result = handle_env(token);
	else if ((ft_strncmp(token->data, "exit", 4) == 0)
		&& ft_strlen(token->data) == 4)
		result = handle_exit(token);
	return (result);
}

bool	is_built_in(t_token *token)
{
	if (!token || token->type != CMD)
		return (false);
	if ((ft_strncmp(token->data, "echo", 4) == 0)
		&& ft_strlen(token->data) == 4)
		return (true);
	else if ((ft_strncmp(token->data, "cd", 2) == 0)
		&& ft_strlen(token->data) == 2)
		return (true);
	else if ((ft_strncmp(token->data, "pwd", 3) == 0)
		&& ft_strlen(token->data) == 3)
		return (true);
	else if (ft_strncmp(token->data, "export", 6) == 0)
		return (true);
	else if ((ft_strncmp(token->data, "unset", 5) == 0)
		&& ft_strlen(token->data) == 5)
		return (true);
	else if ((ft_strncmp(token->data, "env", 3) == 0)
		&& ft_strlen(token->data) == 3)
		return (true);
	else if ((ft_strncmp(token->data, "exit", 4) == 0)
		&& ft_strlen(token->data) == 4)
		return (true);
	return (false);
}