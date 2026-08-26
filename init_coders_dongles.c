#include "codexion.h"

int init_coder_dongle(t_data *config, t_coder **coders, t_dongle **dongles)
{
	int	i;

	*dongles = malloc(sizeof(t_dongle) * config->number_of_coder);
	if (!*dongles)
		return (0);
	i = 0;
	while (i < config->number_of_coder)
	{
		pthread_mutex_init(&(*dongles)[i].lock, NULL);
		pthread_cond_init(&(*dongles)[i].cond, NULL);
		(*dongles)[i].available = 1;
		(*dongles)[i].released_at = 0;
		i++;
	}
	*coders = malloc(sizeof(t_coder) * config->number_of_coder);
	if (!*coders)
	{
		free(*dongles);
		return (0);
	}
	i = 0;
	while (i < config->number_of_coder)
	{
		(*coders)[i].id = i + 1;
		(*coders)[i].config = config;
		(*coders)[i].left = &(*dongles)[i];
		(*coders)[i].right = &(*dongles)[(i + 1) % config->number_of_coder];
		i++;
	}
	return (1);
}
