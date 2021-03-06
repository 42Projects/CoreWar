/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <nfinkel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 19:17:51 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/31 19:11:05 by mcanal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>

#include "corewar.h"

#define HEADER_SZ (16 + PROG_NAME_LENGTH + COMMENT_LENGTH)

void		vm_playerload(t_player *player, const char *file, int id)
{
	int		fd;
	ssize_t	sz;
	uint8_t	buf[4096];

	if ((fd = open(file, O_RDONLY)) < 0)
		vm_exit(EXIT_FAILURE, "%s: %m\n", file);
	if (read(fd, &buf, HEADER_SZ) < HEADER_SZ)
		vm_exit(EXIT_FAILURE, "%s: Invalid header.\n", file);
	if (*(uint32_t *)buf != swap_uint32(COREWAR_EXEC_MAGIC))
		vm_exit(EXIT_FAILURE, "%s: Wrong file: magic number.\n", file);
	ft_bzero(player, sizeof(t_player));
	ft_memcpy((void*)(player->name), buf + sizeof(uint32_t), PROG_NAME_LENGTH);
	ft_memcpy((void*)(player->comment),
			buf + sizeof(uint32_t) + PROG_NAME_LENGTH + 8, COMMENT_LENGTH + 1);
	if ((sz = read(fd, &buf, CHAMP_MAX_SIZE + 1)) <= 0)
		vm_exit(EXIT_FAILURE, "%s: %m: Void champion.\n", file);
	if (sz > CHAMP_MAX_SIZE)
		vm_exit(EXIT_FAILURE, "%s: Champion exceeding size: %d\n", file, sz);
	if (close(fd) < 0)
		vm_exit(EXIT_FAILURE, "%s: %m\n", file);
	player->id = id;
	player->size = (size_t)sz;
	ft_memcpy(player->bin, buf, player->size);
}

void		vm_playerspush(t_players *players, t_player *player)
{
	t_player *node;

	player = ft_memdup(player, sizeof(t_player));
	player->idx = (uint8_t)players->len;
	if (!players->head || players->head->id >= player->id)
	{
		player->next = players->head;
		players->head = player;
	}
	else
	{
		node = players->head;
		while (node->next && node->next->id < player->id)
			node = node->next;
		player->next = node->next;
		node->next = player;
	}
	++players->len;
}

t_player	*vm_playersfind(t_players *players, int id)
{
	t_player *player;

	player = players->head;
	while (player)
	{
		if (player->id == id)
			break ;
		player = player->next;
	}
	return (player);
}

void		vm_playersclr(t_players *players)
{
	t_player	*player;
	void		*tmp;

	player = players->head;
	while (player)
	{
		tmp = player->next;
		free(player);
		player = (t_player *)tmp;
	}
	ft_bzero(players, sizeof(t_players));
}
