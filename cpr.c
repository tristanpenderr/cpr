

/*------------------------------------------------------------
Fichier: cpr.c
Nom: Tristan Pender and Sebastien Leduc
Numero d'etudiant: 300065847 and 300066083
Description: Ce programme contient le code pour la creation
             d'un processus enfant et y attacher un tuyau.
	     L'enfant envoyera des messages par le tuyau
	     qui seront ensuite envoyes a la sortie standard.
Explication du processus zombie
(point 5 de "A completer" dans le devoir):

    Un processus "zombie" est un processus qui a complete l'execution, mais ne ferme pas. Ce qui cause le programe à rester dans la table de processus.
    Quand on fait sleep(10) avant la terminaison du processus, ca creer un processus zombie.

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
    int LONGEURCHAR = 32;

    char format[LONGEURCHAR];

    int fd[2];
    int pid;

    sprintf(format, "Processus %d commence.\n", prcNum);
    write(1, format, LONGEURCHAR);

    if (prcNum == 1)
    {
        sleep(5);
    }
    else
    {
        if (pipe(fd) < 0)
        {
            fprintf(stderr, "Erreur de pipe \n");
        }

        pid = fork();

        if (pid < 0)
        {
            fprintf(stderr, "Erreur de fork \n");
        }
        //si le processus est un enfant
        else if (pid == 0)
        {
            //ferme le read pour le processus enfant
            close(fd[0]);
            processusEnfant(prcNum);
            close(fd[1]);
        }
        else
        {
            close(fd[1]);
            //pendant qu'il y a toujours un processus, continue à écrire "Processus (prcNum) commence"

            int longeur;
            char out[LONGEURCHAR];

            while (longeur = read(fd[0], out, LONGEURCHAR) > 0)
            {
                write(1, out, LONGEURCHAR);
            }
        }
    }
    //écrire le le processus se termine
    sprintf(format, "Processus %d termine.\n", prcNum);
    write(1, format, LONGEURCHAR);

    close(1);
}

void processusEnfant(int prcNum)
{

    char str[10];
    //donne la valeur de prcNum -1 à la variable str
    sprintf(str, "%d", prcNum - 1);
    //créer l'argument args pour le program cpr avec l'argument mis dans str
    char *args[] = {"./cpr", str, NULL};
    //exécute args
    execvp(args[0], args);
}
