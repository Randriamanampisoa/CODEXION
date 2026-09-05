/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 21:05:03 by fanilran          #+#    #+#             */
/*   Updated: 2026/09/05 14:29:53 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
#include "struct.h"

int		pars(t_data *config, int agrc, char *argv[]);
void	*create_threads(t_data config, t_coder *coders);
void	*routine(void *arg);
int		init_coder_dongle(t_data *config, t_coder **coders, t_dongle **dongles);
void	release_dongle(t_dongle *dongle);
void	take_one(t_coder *coder, t_dongle *dongle);
void	compiles(t_coder *coder);
void	debuges(t_coder *coder);
void	refactores(t_coder *coder);
long get_timestamp_ms(long start_time);

#endif
