#include <stdio.h>
#include <stdlib.h>
#include "getline.h"

/* #include "ST.h" */
#include "word.h"
#include "lemma.h"

#define MAX_OPTION_SIZE 3

#define TRUE 1
#define FALSE 0

static void visit(char *word)
{
    int s = 0;
    printf("===============================================\n");
    while(word[s] != ' ') putchar(word[s++]); 
    putchar('\n'); /* word_print_sentences(word_table_get(word)); */
}

int main(int argc, char **argv)
{
    /** VARIÁVEIS *****************************************************/
        char *file_name; int i;
        FILE *file;
        char *buffer;
        
        char *identifier;
        char *sentence;
        char *annotated;
        char *word;
        char *lemma;
        
        char option[MAX_OPTION_SIZE];
        char *query;
        int verbosity = 0;
        
        int exit;
        
        for(i = 0; i < MAX_OPTION_SIZE; i++) option[i] = ' ';
        
        printf("MAC0323-EP3: Localização de Palavras I\n");
    
    /** PRÉ-PROCESSAMENTO DO TEXTO ************************************/
        if(argc != 2 && argc != 3) 
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
            if(argc == 3) file_name = argv[2];  /* Nome separado */
            else file_name = &argv[1][2]; /* Nome colado no '-f' */
            
            /* Inicializa tabelas de símbolos */
            word_table_init(); lemma_table_init(); 
            
            /* Abre texto */
            file = fopen(file_name, "r");
            buffer = getline(file, EOF);
            
            for(i = 0; buffer[i] != '\0'; i++) 
            {
                if(strncmp(&buffer[i], "Sentence #", 10 * sizeof(char)) == 0)
                {
                    for(identifier = &buffer[i]; buffer[i] != ':'; i++) 
                        putchar(buffer[i]); /* Tirar */
                    putchar('\n'); 
                    i++; sentence = &buffer[i];
                    
                    while(buffer[i] != '['
                    && strncmp(&buffer[i], "[Text=", 6 * sizeof(char))) 
                        { putchar(buffer[i]); i++; }
                    annotated = &buffer[i];
                    
                    while(1)/* buffer[++i] != '\n')  */
                    {
                        while(buffer[i] != '\n' && buffer[i] != '[' 
                        && strncmp(&buffer[i], "[Text=", 6 * sizeof(char))) i++;
                        
                        if(buffer[i] == '\n') break;
                        i += 6; word = &buffer[i];
                        
                        while(buffer[i] != 'L'
                        && strncmp(&buffer[i], "Lemma=", 6 * sizeof(char))) i++;
                        i += 6; lemma = &buffer[i];
                        
                        word_table_insert(word, lemma, 
                                identifier, sentence, annotated);
                        lemma_table_insert(lemma, word);
                        
                        while(buffer[i] != ']') i++;
                    }
                }
            }
            
            /* Fecha texto */
            fclose(file);
        }
        
    /** INTERFACE ITERATIVA *******************************************/
        while(1)
        {
            exit = verbosity = 0;
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
                Word w;
                case 'e': /*1*/
                    getchar(); query = getline(stdin, '\n');
                    w = word_table_get(query);
                    if(w == NULL) { printf("Palavra não encontrada.\n"); break; }
                    word_print_sentences(w);
                    break;
                case 'a': /*2*/
                    getchar(); query = getline(stdin, '\n');
                    w = word_table_get(query);
                    if(w == NULL) { printf("Palavra não encontrada.\n"); break; }
                    query = word_lemma(w); lemma_list_words(query, visit);
                    break;
                case 'F': /*3*/
                    exit = TRUE;
                    break;
                case 't': /*4*/
                    getchar(); query = getline(stdin, '\n');
                    break;
                case 'd': /*5*/
                    getchar(); query = getline(stdin, '\n');
                    break;
                case 'l': /*6*/
                    getchar(); query = getline(stdin, '\n');
                    break;
                case 'L': /*7*/
                    getchar(); query = getline(stdin, '\n');
                    break;
                case 's': /*8*/
                    getchar(); query = getline(stdin, '\n');
                    break;
                default:
                    printf("Opção não reconhecida!\n");
                    continue;
                
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
                 * (8) -s: estatísticas do texto: total de tokens com
                 *         repetição, total de palavras, total de 
                 *         tokens distintos, total de palavras dis-
                 *         tintas, total de lemas distintos.
                 */
            }
            if(exit == TRUE) break;
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
        }
        
    /** LIBERAÇÃO DE MEMÓRIA ******************************************/
        printf("Encerrando programa...\n");
        free(buffer); word_table_free(); lemma_table_free(); 
    
    return EXIT_SUCCESS;
}
