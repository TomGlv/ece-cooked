#ifndef PROJET_ECE_COOKED_HEADER_H
#define PROJET_ECE_COOKED_HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define TILE_SIZE 4
#define MATRIX_WIDTH 800
#define MATRIX_HEIGHT 800
#define NIMAGE 6
#define GAME_DURATION_SECS 120  // Durée du jeu en secondes
#define TICKS_PER_SECOND 100
#define GAME_DURATION (GAME_DURATION_SECS * TICKS_PER_SECOND)  // Durée du jeu en ticks

typedef struct ingredient {
    int riz_cru;
    int riz_cuit;
    int algues;
    int saumon_cru;
    int saumon_cuit;
    int avocat;
    int sauce_soja;
    int assiete_vide
}ingredient;

extern BITMAP *buffer;
extern BITMAP *playerBitmap[NIMAGE];
extern BITMAP *map;
extern BITMAP *murs;
extern int player1X;
extern int P_player1X;
extern int player1Y;
extern int P_player1Y;
extern int player2X;
extern int player2Y;
extern int currentFrame1;
extern int currentFrame2;
extern int frameDelay;
extern int tx;
extern int ty;
extern volatile int ticks;
extern SAMPLE *music;
void inventaire();
void ticker();
int contactBordActeur();
void initialisation_allegro();
void play_music();
void draw_timer(int game_start_ticks);
void draw_players();
void update_screen();
void update_game(int game_start_ticks);
void saisir_pseudo_j1(char *pseudoJoueur1, char *pseudoJoueur2);
void saisir_pseudo_j2(char *pseudoJoueur1, char *pseudoJoueur2);

#endif // PROJET_ECE_COOKED_HEADER_H