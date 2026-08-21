/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 13:17:12 by fanilran          #+#    #+#             */
/*   Updated: 2026/08/21 15:01:44 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int verify_schedule(char *argv)
{
    if (strcmp(argv, "fifo") == 0 || strcmp(argv, "edf") == 0)
        return (1);
    return (0);
}

int is_positive(char *argv)
{
    int i;

    i = 0;
    if (!argv)
        return 0;
    while (argv[i])
    {
        if (argv[i] < '0' || argv[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int check(int argc, char **argv)
{
    int i;

    i = 1;
    while (argc - 1 > i)
    {
        if (!is_positive(argv[i]))
        {
            printf("Error: Don't use negative numbers");
            return (0);
        }
        i++;
    }
    if (!verify_schedule(argv[argc - 1]))
    {
        printf("Error: Make sure the argument is “fifo/edf”\n");
        return (0);
    }
    return (1);
}

int    parsing(int argc, char **argv)
{
    int i;
    t_data config;

    i = 0;
    if (argc != 9)
    {
        printf("Error: Wrong number of arguments!\n");
        return (1);
    }
    if (!check(argc, argv))
        return (1);
    config.number_of_coders = atoi(argv[1]);
    config.time_to_burnout = atoi(argv[2]);
    config.time_to_compile = atoi(argv[3]);
    config.time_to_debug = atoi(argv[4]);
    config.time_to_refactor = atoi(argv[5]);
    config.number_of_compilse = atoi(argv[6]);
    config.dongle_cooldown = atoi(argv[7]);
    config.scheduler = argv[8];
    return (0);
}