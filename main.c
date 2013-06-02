#include <stdio.h>
#include <stdlib.h>
#include "getline.h"

/* #include "ST.h" */
#include "word.h"
#include "lemma.h"

#define MAX_OPTION_SIZE 3

#define TRUE 1
#define FALSE 0

static void visit_a(char *word)
{
    int s = 0;
    printf("===============================================\n");
    while(word[s] != ' ') putchar(word[s++]); 
    putchar('\n'); word_print_sentences(word_table_get(word));
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
        
        int total_words = 0;
        int total_tokens = 0;
        int total_sentences = 0;
        
        for(i = 0; i < MAX_OPTION_SIZE; i++) option[i] = ' ';
        
        printf("MAC0323-EP3: Localização de Palavras I\n\n");
    
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
            if(file == NULL) 
                printf("ERRO: arquivo %s não encontrado", file_name);
            else printf("Carregando o texto...\n");
            
            buffer = getline(file, EOF);
            for(i = 0; buffer[i] != '\0'; i++) 
            {
                if(strncmp(&buffer[i], "Sentence #", 10 * sizeof(char)) == 0)
                {
                    for(identifier = &buffer[i]; buffer[i] != ':'; i++);
                    i++; sentence = &buffer[i]; total_sentences++;
                    
                    while(buffer[i] != '['
                    && strncmp(&buffer[i], "[Text=", 6 * sizeof(char)) != 0) i++;
                    annotated = &buffer[i];
                    
                    while(1)
                    {
                        while(buffer[i] != '\n' && buffer[i] != '[' 
                        && strncmp(&buffer[i], "[Text=", 6 * sizeof(char))) i++;
                        
                        if(buffer[i] == '\n') break;
                        i += 6; word = &buffer[i]; 
                        total_tokens++; total_words++; 
                        
                        for(; buffer[i] != ' '; i++) 
                            if(buffer[i] < 'A' || (buffer[i] > 'Z' && buffer[i] < 'a')
                            || buffer[i] > 'z') { total_words--; break; }
                        
                        while(buffer[i] != 'L'
                        && strncmp(&buffer[i], "Lemma=", 6 * sizeof(char) != 0)) i++;
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
            verbosity = 0;
            printf("> ");
            scanf(" %3s", option); 
            
            if(option[0] != '-') 
            {
                printf("Formato de opção inválido!\n");
                printf("Use -<opcões> <palavra>\n");
                continue;
            }
            if(option[1] == 'F') break; /*3*/
            else 
            {
                Word w;
                switch(option[1]) 
                {
                    case 'e': /*1*/
                        getchar(); query = getline(stdin, '\n'); 
                        w = word_table_get(query);
                        if(w == NULL) printf("Palavra não encontrada.\n");
                        word_print_sentences(w);
                        break;
                    case 'a': /*2*/
                        getchar(); query = getline(stdin, '\n'); 
                        w = word_table_get(query);
                        if(w == NULL) printf("Palavra não encontrada.\n");
                        query = word_lemma(w); 
                        lemma_list_words(query, visit_a);
                        break;
                    case 't': /*4*/
                        word_print_tokens();
                        break;
                    case 'd': /*5*/
                        word_print_words();
                        break;
                    case 'l': /*6*/
                        lemma_print_lemmas();
                        break;
                    case 'L': /*7*/
                        lemma_print_lemma_word();
                        break;
                    case 's': /*8*/
                        printf("\n");
                        printf(" ESTATÍSTICAS DO TEXTO\n");
                        printf("\n");
                        printf(" * Total de tokens: %d;\n", total_tokens);
                        printf(" * Total de palavras: %d;\n", total_words);
                        printf(" * Total de sentenças: %d;\n", total_sentences);
                        printf(" * Total de tokens distintos: %d;\n", 
                              word_total_tokens());
                        printf(" * Total de palavras distintas: %d;\n", 
                              word_total_words());
                        printf(" * Total de lemas distintos: %d.\n", 
                              lemma_total_lemmas());
                        
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
                     *         em ordem alfabética)
                     * (7) -L: lista de todos os lemas seguidos das 
                     *         palavras no texto com aquele lema;
                     * (8) -s: estatísticas do texto: total de tokens com
                     *         repetição, total de palavras, total de 
                     *         tokens distintos, total de palavras dis-
                     *         tintas, total de lemas distintos.
                     */
                } /*switch*/
            } /*else */
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
