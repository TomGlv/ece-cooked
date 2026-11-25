//
// Created by tomga on 07/04/2024.
//

#include "header.h"
#include "header.h"

BITMAP *buffer;
BITMAP *playerBitmap[NIMAGE];
BITMAP *map;
BITMAP *murs;
int player1X = 80;
int P_player1X = 0;
int player1Y = 80;
int P_player1Y = 0;
int player2X = 120;
int player2Y = 80;
int currentFrame1 = 0;
int currentFrame2 = 0;
int frameDelay = 0;
int tx = 40;
int ty = 80;
volatile int ticks = 0;
SAMPLE *music;
void ticker() {
    ticks++;
    //printf("Ticks: %d\n", ticks);  // Debug message
}

int contactBordActeur() {
    int ncontact = 0;
    // Parcourir la largeur et la hauteur du sprite du joueur
    for (int i = 0; i < ty; i++) {
        // Bord gauche du sprite
        if (getpixel(murs, player1X, player1Y + i) == makecol(64, 64, 64))
            ncontact++;
        // Bord droit du sprite
        if (getpixel(murs, player1X + tx - 1, player1Y + i) == makecol(64, 64, 64))
            ncontact++;
    }

    for (int k = 0; k < tx; k++) {
        // Bord supérieur du sprite
        if (getpixel(murs, player1X + k, player1Y) == makecol(64, 64, 64))
            ncontact++;
        // Bord inférieur du sprite
        if (getpixel(murs, player1X + k, player1Y + ty - 1) == makecol(64, 64, 64))
            ncontact++;
    }

    return ncontact > 0; // Si ncontact est > 0, il y a collision
}


void initialisation_allegro() {
    if (allegro_init() != 0) {
        allegro_message("Erreur d'initialisation d'Allegro.");
        exit(EXIT_FAILURE);
    }

    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) != 0) {
        allegro_message("Erreur d'initialisation du mode graphique.");
        exit(EXIT_FAILURE);
    }

    buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    map = load_bitmap("carte1.bmp", NULL);
    murs = load_bitmap("carte2.bmp",NULL);

    for (int i = 0; i < NIMAGE; i++) {
        char nomfichier[20];
        sprintf(nomfichier, "player%d.bmp", i);
        playerBitmap[i] = load_bitmap(nomfichier, NULL);
        if (!playerBitmap[i]) {
            allegro_message("Impossible de charger %s", nomfichier);
            exit(EXIT_FAILURE);
        }
    }

    // Setup timer
    LOCK_FUNCTION(ticker);
    LOCK_VARIABLE(ticks);
    install_timer();
    install_int_ex( ticker, BPS_TO_TIMER(TICKS_PER_SECOND));

    // Initialisation des modules audio
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
        allegro_message("Erreur d'initialisation du module audio.");
        exit(EXIT_FAILURE);
    }

    // Chargement du fichier audio WAV
    music = load_sample("Samurai-Nights.wav");
    if (!music) {
        allegro_message("Erreur lors du chargement du fichier WAV.");
        exit(EXIT_FAILURE);
    }
}

void play_music() {
    play_sample(music, 100, 128, 900, 1);
}
void draw_timer(int game_start_ticks) {
    int current_ticks = ticks;
    int elapsed_ticks = current_ticks - game_start_ticks;
    //printf("Elapsed Ticks: %d\n", elapsed_ticks);  // Debug message
    int remaining_seconds = GAME_DURATION_SECS - (elapsed_ticks / TICKS_PER_SECOND);
    //printf("Remaining Seconds: %d\n", remaining_seconds);  // Debug message
    char time_str[100];
    sprintf(time_str, "Time Remaining: %d seconds", remaining_seconds);
    //printf("Elapsed Ticks: %d\n", elapsed_ticks);  // Debug message
    textout_centre_ex(buffer, font, time_str, SCREEN_WIDTH / 2, 30, makecol(127, 0, 255), -1);
}
void draw_players() {
    stretch_sprite(buffer, playerBitmap[currentFrame1], player1X * TILE_SIZE, player1Y * TILE_SIZE, playerBitmap[currentFrame1]->w / 4, playerBitmap[currentFrame1]->h / 4);
    stretch_sprite(buffer, playerBitmap[currentFrame2], player2X * TILE_SIZE,player2Y * TILE_SIZE, playerBitmap[currentFrame2]->w / 4, playerBitmap[currentFrame2]->h / 4);
}
void update_screen() {

    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

}
void update_game(int game_start_ticks) {
    // Efface le buffer avec l'image de fond au lieu d'une couleur unie
    draw_sprite(buffer, map, 0, 0);

    int elapsed_ticks = ticks - game_start_ticks;
    int P_player1X = player1X;
    int P_player1Y = player1Y;
    if (elapsed_ticks < GAME_DURATION) {
        if (key[KEY_W] && player1Y > 0) player1Y--;
        if (key[KEY_S] && player1Y < MATRIX_HEIGHT - 1) player1Y++;
        if (key[KEY_A] && player1X > 0) player1X--;
        if (key[KEY_D] && player1X < MATRIX_WIDTH - 1) player1X++;
        if (key[KEY_UP] && player2Y > 0) player2Y--;
        if (key[KEY_DOWN] && player2Y < MATRIX_HEIGHT - 1) player2Y++;
        if (key[KEY_LEFT] && player2X > 0) player2X--;
        if (key[KEY_RIGHT] && player2X < MATRIX_WIDTH - 1) player2X++;
        frameDelay++;
        if (frameDelay >= 10) {
            currentFrame1 = (currentFrame1 + 1) % NIMAGE;
            currentFrame2 = (currentFrame2 + 1) % NIMAGE;
            frameDelay = 0;
            rest(10);
        }
        //printf("%d %d %d %d\n",player1X,player1Y,P_player1X,P_player1Y);
        if (contactBordActeur())
        {
            player1X = P_player1X;
            player1Y = P_player1Y;
        }
        draw_players();
        draw_timer(game_start_ticks);

        update_screen();
    } else {
        allegro_message("Time is up!");
        rest(20);
        allegro_exit();
        int game_active = 0;
    }
}
void saisir_pseudo_j1(char *pseudoJoueur1,char *pseudoJoueur2) {
    install_keyboard();
    int pos1 = 0;
    while (!key[KEY_SPACE]) {
        textout_centre_ex(screen, font, "Quel est le pseudo du joueur 1 :", (SCREEN_W / 4), SCREEN_H / 2 - 30, makecol(250, 250, 250), -1);
        textout_centre_ex(screen, font, pseudoJoueur1, (SCREEN_W / 4), SCREEN_H / 2, makecol(250, 250, 250), -1);
        textout_centre_ex(screen, font, "Quel est le pseudo du joueur 2 :", (SCREEN_W / 4) + 400, SCREEN_H / 2 - 30, makecol(250, 250, 250), -1);
        textout_centre_ex(screen, font, pseudoJoueur2, (SCREEN_W / 4) + 400, SCREEN_H / 2, makecol(250, 250, 250), -1);
        if (keypressed()) {
            int key = readkey();
            char caractere = key & 0xFF;
            if (pos1 < 255) {
                pseudoJoueur1[pos1] = caractere;
                pos1++;
                pseudoJoueur1[pos1] = '\0';
            }update_screen();
        }
    }
}
void saisir_pseudo_j2(char *pseudoJoueur1,char * pseudoJoueur2) {
    int pos2 = 0;
    while (!key[KEY_SPACE]) {
        textout_centre_ex(screen, font, "Quel est le pseudo du joueur 1 :", (SCREEN_W / 4), SCREEN_H / 2 - 30, makecol(250, 250, 250), -1);
        textout_centre_ex(screen, font, pseudoJoueur1, (SCREEN_W / 4), SCREEN_H / 2, makecol(250, 250, 250), -1);
        textout_centre_ex(screen, font, "Quel est le pseudo du joueur 2 :", (SCREEN_W / 4) + 400, SCREEN_H / 2 - 30, makecol(250, 250, 250), -1);
        textout_centre_ex(screen, font, pseudoJoueur2, (SCREEN_W / 4) + 400, SCREEN_H / 2, makecol(250, 250, 250), -1);
        if (keypressed()) {
            int key = readkey();
            char caractere = key & 0xFF;
            if (pos2 < 255) {
                pseudoJoueur2[pos2] = caractere;
                pos2++;
                pseudoJoueur2[pos2] = '\0';
            }
            update_screen();
        }
    }
}