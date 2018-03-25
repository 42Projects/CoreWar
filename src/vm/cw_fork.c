/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cw_fork.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfinkel <nfinkel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 19:09:54 by nfinkel           #+#    #+#             */
/*   Updated: 2018/03/25 03:11:37 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int			cw_fork(t_proc *proc, uint8_t *pc)
{
	t_proc		*new;
	uint8_t		*ptr;
	int16_t		n;

	ptr = cw_move_ptr(pc, 1);
	n = cw_read_nbytes(ptr, 2);
	new = ft_malloc(sizeof(t_proc));
	ft_memcpy(new, proc, sizeof(t_proc));
	new->lastlive = 0;
	new->wait = 1;
	new->pid = ++g_cw->max_pid;
	new->pc = cw_move_ptr(pc, n % IDX_MOD);
	proc->pc = cw_move_ptr(pc, 3);
	new->next = g_cw->procs;
	g_cw->procs = new;
	++g_cw->proc_count;
	return (EXIT_SUCCESS);
}
