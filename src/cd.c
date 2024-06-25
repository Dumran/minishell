#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

static bool	is_relative_path(const char *path);
static bool	is_absolute_path(const char *path);
static int	handle_relative_path(t_token *token, t_state *state);
static int	handle_absolute_path(t_token *token);

int	handle_cd(t_token *token, t_state *state)
{
	const char	*home_path;

	if (!token)
		return (1);
	home_path = getenv("HOME");
	if (!home_path)
		return (write(2, "cd: HOME not set\n", 17), 1); // todo: use print_err instead
	if (!token->next)
	{
		if (chdir(home_path) == -1)
			return (write(2, "cd: cannot change directory\n", 17), 1);
		return (0);
	}
	if (is_relative_path(token->next->data))
		return (handle_relative_path(token, state));
	else if (is_absolute_path(token->next->data))
		return (handle_absolute_path(token));
	return (0);
}

static bool	is_relative_path(const char *path)
{
	if (!path || *path == '\0')
		return (false);
	if (*path == '.' && (path[1] == '/' || \
			(path[1] == '.' && path[2] == '/' && path[2] != '\0')))
		return (true);
	if (path[0] != '/')
		return (true);
	return (false);
}

static bool	is_absolute_path(const char *path)
{
	if (!path)
		return (false);
	return (path[0] == '/');
}

static int	handle_relative_path(t_token *token, t_state *state)
{
	char	*temp_path;
	char	cwd[PATH_MAX];
	int		len;

	len = 0;
	if (!getcwd(cwd, PATH_MAX))
		return (1);
	len = ft_strlen(cwd) + ft_strlen(token->next->data) + 1;
	if (len >= PATH_MAX)
		return (1);
	temp_path = malloc(sizeof(char) * (len + 1));
	if (!temp_path)
		return (1);
	ft_strlcpy(temp_path, cwd, ft_strlen(cwd) + 1);
	temp_path[ft_strlen(cwd)] = '/';
	ft_strlcpy(temp_path + ft_strlen(cwd) + 1, token->next->data, len);
	temp_path[len] = '\0';
	if (chdir(temp_path) == -1)
	{
		print_err("##handle_relative_path.if##", 1);
		state->status = 1;
		return (free(temp_path), 1);
	}
	return (free(temp_path), 0);
}

static int	handle_absolute_path(t_token *token)
{
	if (chdir(token->next->data) == -1)
		return (print_err("##handle_absolute_path.if##", 1), 1);
	return (0);
}
