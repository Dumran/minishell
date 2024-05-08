#include "minishell.h"
#include <unistd.h>

char	*get_env_path(char **env)
{
	int64_t	i;
	char		*path;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
			return (ft_substr(env[i], 5, ft_strlen(env[i]) - 5));
		i++;
	}
	return (NULL);
}

void	dispose_paths(char **paths)
{
	int64_t	i;

	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char *join_path(char **paths, char *command)
{
	char	*cmd_path;
	char	*tmp;
	int64_t		i;

	if (!paths || !command)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (dispose_paths(paths), NULL);
		cmd_path = ft_strjoin(tmp, command);
		if (!cmd_path)
			return (dispose_paths(paths), free(tmp), NULL);
		if (access(cmd_path, X_OK) == 0)
			return (dispose_paths(paths), free(tmp), cmd_path);
		free(tmp);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*find_path(char *command, char **env)
{
	char	**paths;
	char	*path;

	if (!command)
		return (NULL);
	path = get_env_path(env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (free(path), NULL);
	return (join_path(paths, command));
}