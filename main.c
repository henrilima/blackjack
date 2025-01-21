#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>
#include <windows.h>
#include <time.h>
#include "resource.h"

const char *warn = "\033[0;35m";
const char *white = "\033[0;37m";
const char *bot = "\033[0;32m";
const char *user = "\033[0;36m";
const char *danger = "\033[0;31m";
const char *hint = "\033[0;33m";
const char *reset = "\033[0m";

void verifyAction(char *action, int *user_cards, int *user_card_count, int *bot_cards, int *bot_card_count);

const char *getCardNumber(int i)
{
    char *id = "I";
    switch (i)
    {
    case 0:
        id = "I";
        break;
    case 1:
        id = "II";
        break;
    case 2:
        id = "III";
        break;
    case 3:
        id = "IV";
        break;
    case 4:
        id = "V";
        break;
    case 5:
        id = "VI";
        break;
    case 6:
        id = "VII";
        break;
    case 7:
        id = "VIII";
        break;
    case 8:
        id = "IX";
        break;
    case 9:
        id = "X";
        break;
    case 10:
        id = "X";
        break;
    case 11:
        id = "X";
        break;
    case 12:
        id = "X";
        break;
    default:
        id = "-/-";
    }

    return id;
}

void addCard(char *action, int *user_cards, int *user_card_count, int *bot_cards, int *bot_card_count)
{
    int new_card = (rand() % 10) + 1;        // Gera uma nova carta aleatória (1 a 10)
    user_cards[*user_card_count] = new_card; // Adiciona ao array
    (*user_card_count)++;                    // Incrementa o contador de cartas do usuário

    char *suit[4] = {"♠", "♣", "♥", "♦"}; // Naipes
    int suit_size = sizeof(suit) / sizeof(suit[0]);

    // Inicializando gerador de números aleatórios
    srand(time(NULL));

    int user_total = 0;

    printf("%sSuas cartas:\n", user);
    for (int i = 0; i < *user_card_count; i++)
    {
        user_total += user_cards[i];
        printf("%s. %d %s\n", getCardNumber(i), user_cards[i], suit[rand() % suit_size]);
    }
    printf("Total: %d%s\n\n", user_total, white);

    if (user_total > 21)
    {
        printf("%sSua pontuação excede 21 pontos. Você perdeu.%s\n\n", danger, white);
    }

    if (user_total == 21)
    {
        printf("--> %sParabéns! %sVocê atingiu exatamente 21 pontos e ganhou.%s\n\n", bot, hint, white);
    }

    if (user_total < 21)
    {
        // Pedir ao jogador se deseja continuar jogando
        printf("%sDeseja continuar jogando? (h/s)%s ", user, white);
        scanf(" %c", action);
        verifyAction(action, user_cards, user_card_count, bot_cards, bot_card_count);
    }

    // if (user_total >= 17 && user_total < 21) {
    //     // Encerra o turno do jogador pois o total dele ultrapassou o limite do jogo de 17 pontos
    //     printf("\033[1;35mJogada encerrada. Iniciando jogada do bot.\033[0;37m\n\n");
    //     // Simulação do bot jogando
    // }
}

void verifyAction(char *action, int *user_cards, int *user_card_count, int *bot_cards, int *bot_card_count)
{
    switch (*action)
    {
    case 'h':
    case 'H':
        // Adicionar uma carta a mais ao jogador
        printf("%sAdicionando uma carta a mais ao jogador.%s\n\n", warn, white);
        addCard(action, user_cards, user_card_count, bot_cards, bot_card_count);
        break;
    case 's':
    case 'S':
        // Encerrar a jogada do jogador e aguardar a pontuação do bot
        printf("%sJogada encerrada. Iniciando jogada e revelando as cartas do bot.%s\n\n", warn, white);

        // Simulação do bot jogando
        char *suit[4] = {"♠", "♣", "♥", "♦"}; // Naipes
        int suit_size = sizeof(suit) / sizeof(suit[0]);

        int bot_total = 0;

        printf("%sCartas do bot:\n", bot);
        for (int i = 0; i < *bot_card_count; i++)
        {

            bot_total += bot_cards[i];
            printf("%s. %d %s\n", getCardNumber(i), bot_cards[i], suit[rand() % suit_size]);
        }
        printf("Total: %d%s\n\n", bot_total, white);

        int user_total = 0;
        for (int i = 0; i < *user_card_count; i++)
        {
            user_total += user_cards[i];
        }

        while (bot_total < 17)
        {
            printf("--> %sBot está comprando uma carta a mais.%s\n\n", warn, white);
            Sleep(1000);

            int new_card = (rand() % 10) + 1;
            bot_cards[*bot_card_count] = new_card;
            (*bot_card_count)++;

            Sleep(1000);
            printf("%sCartas do bot:\n", bot);

            bot_total = 0; // Resetando para não ter problemas ao recalcular
            for (int i = 0; i < *bot_card_count; i++)
            {
                bot_total += bot_cards[i];
                printf("%s. %d %s\n", getCardNumber(i), bot_cards[i], suit[rand() % suit_size]);
            }
            printf("Total: %d%s\n\n", bot_total, white);
        }

        if (bot_total > 21)
        {
            printf("%sBot perdeu (%d). %sVocê ganhou (%d).%s\n\n", danger, bot_total, bot, user_total, white);
        }
        else if (bot_total == 21)
        {
            printf("--> %sWow! O bot atingiu exatamente 21 pontos e %sganhou%s.%s\n\n", hint, bot, hint, white);
        }
        else if (bot_total < 21 && bot_total < user_total)
        {
            printf("--> %sParabéns! Você ganhou (%d). %sO bot perdeu (%d).%s\n\n", bot, user_total, danger, bot_total, white);
        }
        else if (bot_total < 21 && bot_total > user_total)
        {
            printf("--> %sWow! O bot ganhou (%d). %sVocê perdeu (%d).%s\n\n", bot, bot_total, danger, user_total, white);
        }
        else if (bot_total < 21 && bot_total == user_total)
        {
            printf("--> %sEmpate! %sVocê e o bot tiveram exatamente %d pontos.%s\n\n", hint, danger, bot_total, white);
        }
        break;
    default:
        printf("%s\n\nAção inválida. Por favor, tente novamente.\n", danger);
        printf("%sVocê pode executar as seguintes ações escrevendo as letras:\nh%s - Hit, pegue mais uma carta\n%ss%s - Stand, conclua sua jogada e aguarde o bot\n%sAção:%s ", hint, white, hint, white, user, white);
        scanf(" %c", action); // Lê um único caractere
        verifyAction(action, user_cards, user_card_count, bot_cards, bot_card_count);
    }
}

void main()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    printf(" ▄▄▄▄    ██▓     ▄▄▄       ▄████▄   ██ ▄█▀ ▄▄▄██▀▀▀ ▄▄▄       ▄████▄   ██ ▄█▀\n");
    printf("▓█████▄ ▓██▒    ▒████▄    ▒██▀ ▀█   ██▄█▒    ▒██   ▒████▄    ▒██▀ ▀█   ██▄█▒ \n");
    printf("▒██▒ ▄██▒██░    ▒██  ▀█▄  ▒▓█    ▄ ▓███▄░    ░██   ▒██  ▀█▄  ▒▓█    ▄ ▓███▄░ \n");
    printf("▒██░█▀  ▒██░    ░██▄▄▄▄██ ▒▓▓▄ ▄██▒▓██ █▄ ▓██▄██▓  ░██▄▄▄▄██ ▒▓▓▄ ▄██▒▓██ █▄ \n");
    printf("░▓█  ▀█▓░██████▒ ▓█   ▓██▒▒ ▓███▀ ░▒██▒ █▄ ▓███▒    ▓█   ▓██▒▒ ▓███▀ ░▒██▒ █▄\n");
    printf("░▒▓███▀▒░ ▒░▓  ░ ▒▒   ▓▒█░░ ░▒ ▒  ░▒ ▒▒ ▓▒ ▒▓▒▒░    ▒▒   ▓▒█░░ ░▒ ▒  ░▒ ▒▒ ▓▒\n");
    printf("▒░▒   ░ ░ ░ ▒  ░  ▒   ▒▒ ░  ░  ▒   ░ ░▒ ▒░ ▒ ░▒░     ▒   ▒▒ ░  ░  ▒   ░ ░▒ ▒░\n");
    printf(" ░    ░   ░ ░     ░   ▒   ░        ░ ░░ ░  ░ ░ ░     ░   ▒   ░        ░ ░░ ░ \n");
    printf(" ░          ░  ░      ░  ░░ ░      ░  ░    ░   ░         ░  ░░ ░      ░  ░   \n");
    printf("      ░                   ░                                  ░                \n\n");

    int user_cards[9] = {0};              // Cartas do jogador
    int bot_cards[9] = {0};               // Cartas do bot
    int user_card_count = 2;              // Número inicial de cartas do jogador
    int bot_card_count = 2;               // Número inicial de cartas do bot
    char *suit[4] = {"♠", "♣", "♥", "♦"}; // Naipes
    int suit_size = sizeof(suit) / sizeof(suit[0]);

    // Inicializando gerador de números aleatórios
    srand(time(NULL));

    // Cartas iniciais do jogador e do bot
    user_cards[0] = (rand() % 10) + 1;
    user_cards[1] = (rand() % 10) + 1;
    bot_cards[0] = (rand() % 10) + 1;
    bot_cards[1] = (rand() % 10) + 1;

    // Exibindo cartas iniciais do jogador e do bot
    printf("%sSuas cartas:\nI. %d %s\nII. %d %s\nTotal: %d\n\n",
           user, user_cards[0], suit[rand() % suit_size],
           user_cards[1], suit[rand() % suit_size],
           user_cards[0] + user_cards[1]);

    printf("%sCartas do bot:\nI. %d %s\nII. X x\nTotal: %d\n\n",
           bot, bot_cards[0], suit[rand() % suit_size], bot_cards[0]);

    // Ação do jogador
    char action;
    printf("%sVocê pode executar as seguintes ações escrevendo as letras:\nh%s - Hit, pegue mais uma carta\n%ss%s - Stand, conclua sua jogada e aguarde o bot\n%sAção:%s ", hint, white, hint, white, user, white);
    scanf(" %c", &action);

    verifyAction(&action, user_cards, &user_card_count, bot_cards, &bot_card_count);

    system("pause");
}