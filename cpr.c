/*------------------------------------------------------------
Fichier: cpr.c
Nom:
Numero d'etudiant:
Description: Ce programme contient le code pour la creation
             d'un processus enfant et y attacher un tuyau.
	     L'enfant envoyera des messages par le tuyau
	     qui seront ensuite envoyes a la sortie standard.
Explication du processus zombie
(point 5 de "A completer" dans le devoir):
	(s.v.p. completez cette partie);
-------------------------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>

/* Prototype */
void creerEnfantEtLire(int);

/*-------------------------------------------------------------
Function: main
Arguments: 
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description:
	Extrait le nombre de processus a creer de la ligne de
	commande. Si une erreur a lieu, le processus termine.
	Appel creerEnfantEtLire pour creer un enfant, et lire
	les donnees de l'enfant.
-------------------------------------------------------------*/

int main(int ac, char **av)
{
    int numeroProcessus;

    if (ac == 2)
    {
        if (sscanf(av[1], "%d", &numeroProcessus) == 1)
        {
            creerEnfantEtLire(numeroProcessus);
        }
        else
            fprintf(stderr, "Ne peut pas traduire argument\n");
    }
    else
        fprintf(stderr, "Arguments pas valide\n");
    return (0);
}

/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments: 
	int prcNum - le numero de processus
Description:
	Cree l'enfant, en y passant prcNum-1. Utilise prcNum
	comme identificateur de ce processus. Aussi, lit les
	messages du bout de lecture du tuyau et l'envoie a 
	la sortie standard (df 1). Lorsqu'aucune donnee peut
	etre lue du tuyau, termine.
-------------------------------------------------------------*/

void creerEnfantEtLire(int prcNum)
{
    /* S.V.P. completez cette fonction selon les
       instructions du devoirs. */
    int CHARLEN = 32;

    int fd[2], pid;
    char format[CHARLEN];

    sprintf(format, "Processus %d commence.\n", prcNum);
    write(1, format, CHARLEN);

    if (prcNum == 1)
    {
        sleep(5);
    }
    else
    {

        if (pipe(fd) < 0)
        {
            printf("Erreur de pipe().\n");
        }

        pid = fork();

        if (pid < 0)
        {
            printf("Erreur de fork().\n");
        }else if (pid == 0)
        {
            dup2(fd[1], 1);
            char str[10];
            sprintf(str, "%d", prcNum - 1);
            char *args[] = {"./cpr", str, NULL};
            execvp(args[0], args);
        }else
        {

            int readLen;
            char readOut[CHARLEN];
            
            close(fd[1]);
            
            while (readLen = read(fd[0], readOut, CHARLEN) > 0)
            {
                write(1, readOut, CHARLEN);
            }
        }
    }
    sprintf(format, "Processus %d termine.\n", prcNum);
    write(1, format, CHARLEN);

    close(1);
}