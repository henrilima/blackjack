#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>
#include <windows.h>
#include <time.h>
#include "resource.h"

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

#define COLOR_WHITE 7
#define COLOR_RED 4
#define COLOR_GREEN 2
#define COLOR_CYAN 11
#define COLOR_YELLOW 14
#define COLOR_MAGENTA 13

const char *warn = "\033[0;35m";
const char *white = "\033[0;37m";
const char *bot = "\033[0;32m";
const char *user = "\033[0;36m";
const char *danger = "\033[0;31m";
const char *hint = "\033[0;33m";
const char *reset = "\033[0m";

void verifyAction(char *action, int *user_cards, int *user_card_count, int *bot_cards, int *bot_card_count);

const char *getCardNumber(int i) {
    switch (i) {
    case 0: return "I";
    case 1: return "II";
    case 2: return "III";
    case 3: return "IV";
    case 4: return "V";
    case 5: return "VI";
    case 6: return "VII";
    case 7: return "VIII";
    case 8: return "IX";
    case 9: return "X";
    default: return "-/-";
    }
}

void addCard(char *action, int *user_cards, int *user_card_count, int *bot_cards, int *bot_card_count) {
    int new_card = (rand() % 10) + 1;        // Gera uma nova carta aleatória (1 a 10)
    user_cards[*user_card_count] = new_card; // Adiciona ao array
    (*user_card_count)++;                    // Incrementa o contador de cartas do usuário

    char *suit[4] = {"S", "C", "H", "D"}; // Naipes
    int suit_size = sizeof(suit) / sizeof(suit[0]);

    // Inicializando gerador de números aleatórios
    srand(time(NULL));

    int user_total = 0;

    setColor(COLOR_CYAN);
    printf("Suas cartas:\n");
    setColor(COLOR_WHITE);
    for (int i = 0; i < *user_card_count; i++) {
        user_total += user_cards[i];
        printf("%d. %d %s\n", i + 1, user_cards[i], suit[rand() % suit_size]);
    }
    printf("Total: %d\n\n", user_total);

    if (user_total > 21) {
        setColor(COLOR_RED);
        printf("Sua pontuação excede 21 pontos. Você perdeu.\n\n");
        setColor(COLOR_WHITE);
    } else if (user_total == 21) {
        setColor(COLOR_GREEN);
        printf("--> Parabéns! Você atingiu exatamente 21 pontos e ganhou.\n\n");
        setColor(COLOR_WHITE);
    } else {
        // Pedir ao jogador se deseja continuar jogando
        setColor(COLOR_YELLOW);
        printf("Deseja continuar sua jogada? (h - para hit/s - para stand) ");
        setColor(COLOR_WHITE);
        scanf(" %c", action);
        verifyAction(action, user_cards, user_card_count, bot_cards, bot_card_count);
    }
}

void verifyAction(char *action, int *user_cards, int *user_card_count, int *bot_cards, int *bot_card_count) {
    switch (*action) {
    case 'h':
    case 'H':
        setColor(COLOR_MAGENTA);
        printf("Adicionando uma carta a mais ao jogador.\n\n");
        setColor(COLOR_WHITE);
        addCard(action, user_cards, user_card_count, bot_cards, bot_card_count);
        break;
    case 's':
    case 'S':
        setColor(COLOR_MAGENTA);
        printf("Jogada encerrada. Iniciando jogada e revelando as cartas do bot.\n\n");
        setColor(COLOR_WHITE);

        // Simulação do bot jogando
        char *suit[4] = {"♠", "♣", "♥", "♦"}; // Naipes
        int suit_size = sizeof(suit) / sizeof(suit[0]);

        int bot_total = 0;

        setColor(COLOR_GREEN);
        printf("Cartas do bot:\n");
        setColor(COLOR_WHITE);
        for (int i = 0; i < *bot_card_count; i++) {
            bot_total += bot_cards[i];
            printf("%d. %d %s\n", i + 1, bot_cards[i], suit[rand() % suit_size]);
        }
        printf("Total: %d\n\n", bot_total);

        int user_total = 0;
        for (int i = 0; i < *user_card_count; i++) {
            user_total += user_cards[i];
        }

        while (bot_total < 17) {
            setColor(COLOR_MAGENTA);
            printf("--> Bot está comprando uma carta a mais.\n\n");
            setColor(COLOR_WHITE);

            Sleep(1000);

            int new_card = (rand() % 10) + 1;
            bot_cards[*bot_card_count] = new_card;
            (*bot_card_count)++;

            Sleep(1000);
            setColor(COLOR_GREEN);
            printf("Cartas do bot:\n");
            setColor(COLOR_WHITE);

            bot_total = 0; // Resetando para não ter problemas ao recalcular
            for (int i = 0; i < *bot_card_count; i++) {
                bot_total += bot_cards[i];
                printf("%d. %d %s\n", i + 1, bot_cards[i], suit[rand() % suit_size]);
            }
            printf("Total: %d\n\n", bot_total);
        }

        if (bot_total > 21) {
            setColor(COLOR_RED);
            printf("Bot perdeu (%d). ", bot_total);
            setColor(COLOR_GREEN);
            printf("Você ganhou (%d).\n\n", user_total);
            setColor(COLOR_WHITE);
        } else if (bot_total == 21) {
            setColor(COLOR_YELLOW);
            printf("--> Wow! O bot atingiu exatamente 21 pontos e ganhou.\n\n");
            setColor(COLOR_WHITE);
        } else if (bot_total < 21 && bot_total < user_total) {
            setColor(COLOR_GREEN);
            printf("--> Parabéns! Você ganhou (%d). ", user_total);
            setColor(COLOR_RED);
            printf("O bot perdeu (%d).\n\n", bot_total);
            setColor(COLOR_WHITE);
        } else if (bot_total < 21 && bot_total > user_total) {
            setColor(COLOR_RED);
            printf("--> Wow! O bot ganhou (%d). ", bot_total);
            printf("Você perdeu (%d).\n\n", user_total);
            setColor(COLOR_WHITE);
        } else if (bot_total < 21 && bot_total == user_total) {
            setColor(COLOR_YELLOW);
            printf("--> Empate! Você e o bot tiveram exatamente %d pontos.\n\n", bot_total);
            setColor(COLOR_WHITE);
        }
        break;
    default:
        setColor(COLOR_RED);
        printf("\n\nAção inválida. Por favor, tente novamente.\n");
        setColor(COLOR_YELLOW);
        printf("Você pode executar as seguintes ações escrevendo as letras:\nh - Hit, pegue mais uma carta\ns - Stand, conclua sua jogada e aguarde o bot\n");
        setColor(COLOR_WHITE);
        printf("Ação: ");
        scanf(" %c", action);
        verifyAction(action, user_cards, user_card_count, bot_cards, bot_card_count);
    }
}

void main()
{
    system("chcp 65001 > nul");
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
    char *suit[4] = {"S", "C", "H", "D"}; // Naipes
    int suit_size = sizeof(suit) / sizeof(suit[0]);

    // Inicializando gerador de números aleatórios
    srand(time(NULL));

    // Cartas iniciais do jogador e do bot
    user_cards[0] = (rand() % 10) + 1;
    user_cards[1] = (rand() % 10) + 1;
    bot_cards[0] = (rand() % 10) + 1;
    bot_cards[1] = (rand() % 10) + 1;

    // Exibindo cartas iniciais do jogador e do bot
    setColor(1); // Cor do jogador
    printf("Suas cartas:\nI. %d %s\nII. %d %s\nTotal: %d\n\n",
           user_cards[0], suit[rand() % suit_size],
           user_cards[1], suit[rand() % suit_size],
           user_cards[0] + user_cards[1]);

    setColor(2); // Cor do bot
    printf("Cartas do bot:\nI. %d %s\nII. X x\nTotal: %d\n\n",
           bot_cards[0], suit[rand() % suit_size], bot_cards[0]);

    // Ação do jogador
    char action;
    setColor(COLOR_YELLOW); // Cor de dica/hint
    printf("Você pode executar as seguintes ações escrevendo as letras:\nh - Hit, pegue mais uma carta\ns - Stand, conclua sua jogada e aguarde o bot\n");

    setColor(1); // Cor do jogador
    printf("Ação: ");
    scanf(" %c", &action);

    verifyAction(&action, user_cards, &user_card_count, bot_cards, &bot_card_count);

    system("pause");
}