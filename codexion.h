/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 21:05:03 by fanilran          #+#    #+#             */
/*   Updated: 2026/08/27 12:17:24 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_data
{
	int				number_of_coder;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	pthread_mutex_t	print_mutex;
}	t_data;

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				available;
	long			released_t;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	t_dongle	*left;
	t_dongle	*right;
	t_data		*config;
}	t_coder;

int		pars(t_data *config, int agrc, char *argv[]);
void	*create_threads(t_data config, t_coder *coders);
void	*routine(void *arg);
int		init_coder_dongle(t_data *config, t_coder **coders, t_dongle **dongles);
void	release_dongle(t_dongle *dongle);
void	take_dongle(t_coder *coder, t_dongle *dongle);
void	compiles(t_coder *coder);
void	debuges(t_coder *coder);
void	refactores(t_coder *coder);

#endif
