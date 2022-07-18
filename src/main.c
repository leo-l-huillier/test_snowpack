/*
** EPITECH PROJECT, 2022
** test_snowpack
** File description:
** main.c
*/
#include "include.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    if (strcmp(argv[1], "Proxy") == 0)
        server();
    if (strcmp(argv[1], "User") == 0)
        client(argc, argv);
    return 0;
}