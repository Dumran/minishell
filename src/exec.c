#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>

void	run_executor(t_state *state)
{
	if (!(state->token_arr && execute_prompt(state) == SUCCESS))
		print_unknown_err(state);
	dispose_prompt(state);
}

static void	fork_init_exec_child_part_close(int **fd, int i)
{
	if (i != 0)
	{
		close(fd[i - 1][0]);
		close(fd[i - 1][1]);
	}
}

int	fork_init_exec_child_part(t_state *state, t_cmd *cmd, pid_t *pids,
	int **fd)
{
	int		arr_len;
	pid_t	pid;
	int		i;

	i = 0;
	arr_len = token_arr_len(state->token_arr);
	while (i < arr_len)
	{
		if (set_heredoc_fds(state->token_arr[i], cmd, i) != SUCCESS)
			return (FAILURE);
		g_sig = IN_CMD;
		pid = fork();
		if (pid == -1)
			return (free(pids), FAILURE);
		pids[i] = pid;
		if (pid == 0)
			handle_child_process(fd, state, cmd, i);
		else
			fork_init_exec_child_part_close(fd, i);
		i++;
	}
	g_sig = AFTER_CMD;
	return (SUCCESS);
}

int	fork_init(t_state *state, t_cmd *cmd, int **fd, int arr_len)
{
	pid_t						*pids;
	int							i;

	if (!fd || arr_len < 0)
		return (FAILURE);
	pids = (int *) malloc(sizeof(int) * (arr_len + 1));
	if (!pids)
		return (FAILURE);
	if (fork_init_exec_child_part(state, cmd, pids, fd) != SUCCESS)
		return (free(pids), FAILURE);
	i = 0;
	while (i < arr_len)
	{
		waitpid(pids[i], &state->status, 0);
		state->status = w_exit_status(state->status);
		i++;
	}
	return (free(pids), SUCCESS);
}

int	execute_prompt(t_state *state)
{
	int			arr_len;
	t_cmd		cmd;
	int			**pipe_fds;

	pipe_fds = NULL;
	if (!state || !state->token_arr)
		return (FAILURE);
	arr_len = token_arr_len(state->token_arr);
	if (arr_len < 1)
		return (FAILURE);
	if (cmd_init(&cmd, arr_len) != SUCCESS)
		return (FAILURE);
	if (arr_len == 1)
		return (exec_single_cmd(state->token_arr[0], state, &cmd));
	pipe_fds = pipe_fds_init(arr_len - 1);
	if (!pipe_fds)
		return (free(cmd.heredoc), FAILURE);
	if (fork_init(state, &cmd, pipe_fds, arr_len) != SUCCESS)
		return (pipe_fds_dispose_idx(pipe_fds, arr_len - 1), free(cmd.heredoc),
			FAILURE);
	return (pipe_fds_dispose_idx(pipe_fds, arr_len - 1), free(cmd.heredoc),
		SUCCESS);
}
