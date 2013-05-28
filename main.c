#include <stdio.h>
#include <stdlib.h>
/* #include "ST.h" */

#define MAX_OPTION_SIZE 3

char sequence[80];

int main(int argc, char **argv)
{
    /** VARIÁVEIS *****************************************************/
        char option[MAX_OPTION_SIZE];
        char *file_name;
        FILE *file;
        int verbosity = 0;
        
        printf("MAC0323-EP3: Localização de Palavras I\n");
    
    /** PRÉ-PROCESSAMENTO DO TEXTO ************************************/
        if(argc != 2 || argc != 3) 
        {
            printf("Uso: ./ep3 -f<arquivo.txt.out>\n");
            return EXIT_FAILURE;
        }
        else if(argv[1][0] != '-' || argv[1][1] != 'f')
        {
            printf("Erro: arquivo requerido para análise!\n");
            printf("Uso: ./ep3 -f<arquivo.txt.out>\n");
            return EXIT_FAILURE;
        }    
        else
        {
            /* Nome está separado nos argumentos */
            if(argc == 3) file_name = argv[3];
            /* Nome está colado em '-f' nos argumentos */
            else file_name = &argv[2][2];
            
            file = fopen(file_name, "r");
            fclose(file);
        }
        
    /** INTERFACE ITERATIVA *******************************************/
        while(1)
        {
            verbosity = 0;
            printf("> ");
            scanf(" %3s", option);
            
            if(option[0] != '-') 
            {
                printf("Formato de opção inválido!\n");
                printf("Use -<opcões> <palavra>\n");
                continue;
            }
            switch(option[1]) 
            {
                case 'e': /*1*/
                    break;
                case 'a': /*2*/
                    break;
                case 'F': /*3*/
                    return EXIT_SUCCESS;
                case 't': /*4*/
                    break;
                case 'd': /*5*/
                    break;
                case 'l': /*6*/
                    break;
                case 'L': /*7*/
                    break;
                case 's': /*8*/
                    break;
                default:
                    printf("Opção não reconhecida!\n");
                
                /* Sumário de opções:
                 * (1) -e: todas as sentenças que contêm exatamente
                 *         a palavra
                 * (2) -a: todas as sentenças que contêm a palavra
                 *         e suas variantes;
                 * (3) -F: fim das instruções;
                 * (4) -t: lista de todos os 'tokens' presentes no 
                 *         texto em ordem alfabética;
                 * (5) -d: lista de todas as palavras no texto, sem
                 *         repetição e em ordem alfabética;
                 * (6) -l: todas as palavras presentes no texto em 
                 *         sua forma lematizada (sem repetição e 
                 *         em ordem alfabética);
                 * (7) -L: lista de todos os lemas seguidos das 
                 *         palavras no texto com aquele lema;
                 * (8) -s: estatísticas do texto: total de tokes com
                 *         repetição, total de palavras, total de 
                 *         tokens distintos, total de palavras dis-
                 *         tintas, total de lemas distintos.
                 */
            }
            if(option[1] == 'a' || option[1] == 'e')
            {
                switch(option[2])
                {
                    case 'v':
                        verbosity = 1;
                        break;
                    case 'V':
                        verbosity = 2;
                        break;
                    case ' ':
                    case '\0':
                        break;
                    default:
                        printf("Opção não reconhecida!\n");
                }
            }
            else if(option[2] != ' ') 
                printf("Opção -%c não aceita argumento %c!\n", 
                        option[1], option[2]);
            
            printf("%s\n", option);
        }
    
    return EXIT_SUCCESS;
}
