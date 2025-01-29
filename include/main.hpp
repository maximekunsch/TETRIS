#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "input.hpp"
#include "grid.hpp"
#include "piece.hpp"
#include "game.hpp"

// NAMESPACE
using namespace sf;

// CONSTANTES
Font menuFont;

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
int score = 0;
int Level = 1;
int nbTot = 0;

bool gameOver = false; // Variable pour gérer l'état du jeu
bool linecleared = false;
int moveCooldown = 150; // Délai minimal entre les déplacements en millisecondes

// PROTOTYPES
void LoadFont();
void SetScore(Text& txt, int score);
void SetLevel(Text& txt, int Level);
void CheckBtn(Piece& piece,Piece& nextPiece, Grid& grid, bool& test);

// Fonction pour afficher le menu principal avec détection des clics
int showMenu(RenderWindow& window) {
    Font menuFont;
    if (!menuFont.loadFromFile("res/AirntQuantum-JRMra.otf")) {
        std::cerr << "Erreur : Impossible de charger la police" << std::endl;
        return -1;
    }

    // Titre du menu
    Text title("My TETRIS", menuFont, 50);
    title.setFillColor(Color::White);
    title.setPosition(WIN_WIDTH / 2 - 150, 100);

    // Options du menu avec rectangles cliquables
    Text playOption("Play", menuFont, 30);
    playOption.setFillColor(Color::Cyan);
    playOption.setPosition(WIN_WIDTH / 2 - 100, 250);
    
    RectangleShape playButton(Vector2f(200, 40));
    playButton.setPosition(WIN_WIDTH / 2 - 100, 250);
    playButton.setFillColor(Color::Transparent);

    Text instructionsOption("Instructions", menuFont, 30);
    instructionsOption.setFillColor(Color::Cyan);
    instructionsOption.setPosition(WIN_WIDTH / 2 - 100, 300);
    
    RectangleShape instructionsButton(Vector2f(200, 40));
    instructionsButton.setPosition(WIN_WIDTH / 2 - 100, 300);
    instructionsButton.setFillColor(Color::Transparent);

    Text quitOption("Quit", menuFont, 30);
    quitOption.setFillColor(Color::Cyan);
    quitOption.setPosition(WIN_WIDTH / 2 - 100, 350);
    
    RectangleShape quitButton(Vector2f(200, 40));
    quitButton.setPosition(WIN_WIDTH / 2 - 100, 350);
    quitButton.setFillColor(Color::Transparent);

    // Éléments pour l'écran d'instructions
    Text instructionsText(
        "Instructions\n\n"
        " Utiliser les fleches Left/Right\n pour bouger les pieces\n\n"
        " Utiliser la fleche Up pour tourner\n\n"
        " Utiliser la fleche Down pour\n descendre vers le bas\n\n"
        " Utiliser espace pour\n descendre vers le bas\n plus rapidement\n\n"
        " Remplissez les lignes\n pour marquer des points",
        menuFont, 25);
    instructionsText.setFillColor(Color::White);
    instructionsText.setPosition(50, 100);

    Text backOption("Back to Menu", menuFont, 30);
    backOption.setFillColor(Color::Cyan);
    backOption.setPosition(WIN_WIDTH / 2 - 100, 500);

    RectangleShape backButton(Vector2f(200, 40));
    backButton.setPosition(WIN_WIDTH / 2 - 100, 500);
    backButton.setFillColor(Color::Transparent);

    bool showingInstructions = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return -1;
            }

            if (event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f worldPos = window.mapPixelToCoords(mousePos);

                if (!showingInstructions) {
                    if (playButton.getGlobalBounds().contains(worldPos)) {
                        return 1;
                    } else if (instructionsButton.getGlobalBounds().contains(worldPos)) {
                        showingInstructions = true;
                    } else if (quitButton.getGlobalBounds().contains(worldPos)) {
                        return 3;
                    }
                } else {
                    if (backButton.getGlobalBounds().contains(worldPos)) {
                        showingInstructions = false;
                    }
                }
            }

            if (event.type == Event::KeyPressed) {
                if (!showingInstructions) {
                    if (event.key.code == Keyboard::Num1) {
                        return 1;
                    } else if (event.key.code == Keyboard::Num2) {
                        showingInstructions = true;
                    } else if (event.key.code == Keyboard::Num3) {
                        return 3;
                    }
                } else if (event.key.code == Keyboard::Escape) {
                    showingInstructions = false;
                }
            }
        }

        // Effet de survol
        Vector2i mousePos = Mouse::getPosition(window);
        Vector2f worldPos = window.mapPixelToCoords(mousePos);

        window.clear(Color::Black);

        if (!showingInstructions) {
            // Mise à jour des couleurs du menu principal
            playOption.setFillColor(playButton.getGlobalBounds().contains(worldPos) ? Color::Yellow : Color::Cyan);
            instructionsOption.setFillColor(instructionsButton.getGlobalBounds().contains(worldPos) ? Color::Yellow : Color::Cyan);
            quitOption.setFillColor(quitButton.getGlobalBounds().contains(worldPos) ? Color::Yellow : Color::Cyan);

            // Affichage du menu principal
            window.draw(title);
            window.draw(playButton);
            window.draw(instructionsButton);
            window.draw(quitButton);
            window.draw(playOption);
            window.draw(instructionsOption);
            window.draw(quitOption);
        } else {
            // Mise à jour de la couleur du bouton retour
            backOption.setFillColor(backButton.getGlobalBounds().contains(worldPos) ? Color::Yellow : Color::Cyan);

            // Affichage de l'écran d'instructions
            window.draw(instructionsText);
            window.draw(backButton);
            window.draw(backOption);
        }

        window.display();
    }

    return -1;
}