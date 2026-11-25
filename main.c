#include "header.h"

int main() {
    initialisation_allegro();
    char pseudoJoueur1[256] = "";
    char pseudoJoueur2[256] = "";
    BITMAP *menu;
    menu = load_bitmap("menu_ece_cooked.bmp", NULL);
    stretch_blit(menu, screen, 0, 0, menu->w, menu->h, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    int selected_option = 0;
    int show_rules = 0;
    int game_active = 0;
    int game_start_ticks = 0;
    int ncontact;
    play_music();

    while (!key[KEY_ESC]) {
        if (!game_active) {
            if (key[KEY_DOWN]) {
                selected_option = (selected_option + 1) % 3;
                rest(100);
            }
            else if (key[KEY_UP]) {
                selected_option = (selected_option - 1 + 3) % 3;
                rest(100);
            }
            else if (key[KEY_SPACE]) {
                key[KEY_SPACE]=!key[KEY_SPACE];
                if (selected_option == 0) {
                    rest(300);
                    saisir_pseudo_j1(pseudoJoueur1, pseudoJoueur2);
                    rest(200);
                    saisir_pseudo_j2(pseudoJoueur1, pseudoJoueur2);
                    rest(300);
                    allegro_message("Noms des joueurs confirmés");
                    game_active = 1;
                    game_start_ticks = ticks;
                } else if (selected_option == 1) {
                    show_rules = 1;
                } else if (selected_option == 2) {
                    break;
                }
            }
        }
        if (game_active && selected_option != 2) {
            update_game(game_start_ticks);
        } else {
            for (int i = 0; i < 3; i++) {
                int rect_color = (i == selected_option) ? 255 : 0;
                int text_color = (i == selected_option) ? 0 : 255;
                rectfill(screen, 325, 140 + i * 70, 475, 200 + i * 70, makecol(rect_color, rect_color, rect_color));
                if (i == 0) {
                    textout_centre_ex(screen, font, "Jouer", 400, 170, makecol(text_color, text_color, text_color), -1);

                } else if (i == 1) {
                    textout_centre_ex(screen, font, "Guides de touches", 400, 240, makecol(text_color, text_color, text_color), -1);
                } else if (i == 2) {
                    textout_centre_ex(screen, font, "Quitter", 400, 310, makecol(text_color, text_color, text_color), -1);
                }
            }
        }
        if (show_rules) {
            allegro_message("Contrôles :\n"
                            "Joueur 1 :\n"
                            "Haut : Z\n"
                            "Gauche : Q\n"
                            "Bas : S\n"
                            "Droite : D\n"
                            "Joueur 2 :\n"
                            "Haut : Flèche du haut\n"
                            "Gauche : Flèche de gauche\n"
                            "Bas : Flèche du bas\n"
                            "Droite : Flèche de droite\n"
                            "Interagir : Espace (Les deux joueurs)");
            show_rules = 0;
        }
        rest(10);  // Rest to prevent excessive CPU usage
    }
    allegro_exit();
    return 0;
}END_OF_MAIN()