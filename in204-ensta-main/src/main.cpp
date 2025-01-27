#include <cmath> // Pour pow()
#include "main.hpp"

// Variables globales
Font font;
Text txt,txtlevel, gameOverText, nextPieceText, LevelText;
char temp[256];
Clock moveClock; // Horloge pour les déplacements manuels



RenderWindow window;
Input input;

SoundBuffer gameOverBuffer;
Sound gameOverSound;
SoundBuffer lineClearBuffer;
SoundBuffer rotateBuffer;
Sound lineClearSound;
Sound rotateSound;
Music backgroundMusic;



int main(){

    window.create(VideoMode(WIN_WIDTH,WIN_HEIGHT),"My TETRIS");
    window.setVerticalSyncEnabled(true);

        // Charger la musique de fond
    if (!backgroundMusic.openFromFile("res/background_music.wav")) {
        std::cout << "Erreur chargement musique de fond" << std::endl;
    } else {
        backgroundMusic.setLoop(true); // Boucle infinie
        backgroundMusic.setVolume(50); // Volume entre 0 et 100
        backgroundMusic.play();
    }

   // Afficher le menu principal
    int choice = showMenu(window);

    if(choice == 2){
        Text instructionsText("Instructions:\n\n- Use Left/Right to move\n- Use Up to rotate\n- Use Down to accelerate\n- ""Fill rows to score points.\n\nPress any key to return to the menu.",menuFont, 20);
        instructionsText.setFillColor(Color::White);
        instructionsText.setPosition(50, 100);

        window.clear(Color::Black);
        window.draw(instructionsText);
        window.display();
    }
    

    if (choice == -1 || choice == 3) {
        return 0; // Quitter si le joueur choisit de quitter ou ferme la fenêtre
    }

    //Police
    LoadFont();

    //Son
    if (!gameOverBuffer.loadFromFile("res/game_over.wav")) {
        std::cout << "Erreur chargement son Game Over" << std::endl;
    } else {
        gameOverSound.setBuffer(gameOverBuffer);
        gameOverSound.setVolume(50); // Volume entre 0 et 100
    }

    // Charger le son de destruction de ligne
    if (!lineClearBuffer.loadFromFile("res/line_clear.ogg")) {
        std::cout << "Erreur chargement son destruction ligne" << std::endl;
    } else {
        lineClearSound.setBuffer(lineClearBuffer);
        lineClearSound.setVolume(50); // Volume entre 0 et 100
    }

    // Charger le son de rotation de pièce
    if (!rotateBuffer.loadFromFile("res/rotate_piece.wav")) {
        std::cout << "Erreur chargement son rotation pièce" << std::endl;
    } else {
        rotateSound.setBuffer(rotateBuffer);
        rotateSound.setVolume(30); // Volume entre 0 et 100
    }


    // Affichage Game Over
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(36);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setStyle(Text::Bold);
    gameOverText.setPosition(100, WIN_HEIGHT / 2 - 50);
    gameOverText.setString("Game Over! Press R to Restart");

    // Affichage Next Piece
    nextPieceText.setFont(font);
    nextPieceText.setCharacterSize(20);
    nextPieceText.setFillColor(Color::White);
    nextPieceText.setStyle(Text::Bold);
    nextPieceText.setPosition(400, 100);
    nextPieceText.setString("Next Piece:");

    Clock clock;


    Piece piece = generateRandomPiece(4,0);
    Piece nextPiece = generateRandomPiece(4,0);


    Grid grid(lignes, colonnes);
    grid.placePiece(piece);

    
    bool test = 0;

    while(window.isOpen()){
        
        Event event;

        while(window.pollEvent(event)){
            input.InputHandler(event, window);
            CheckBtn(piece,nextPiece, grid, test);
        }

        if (!gameOver) {
            SetScore(txt, score);
            SetLevel(txtlevel, Level);
            auto time = std::pow((0.8 - ((Level - 1) * 0.007)), (Level - 1));
            // Déplacer la pièce automatiquement vers le bas
            if (clock.getElapsedTime().asSeconds() >= time) {
                test = grid.movePiece(piece, 0, 1);
                clock.restart();
            }

            // Vérifier les entrées utilisateur
            CheckBtn(piece,nextPiece, grid, test);

            // Gérer les lignes pleines et mise à jour du score
            lineFullHandler(grid, test, score, nbTot);
            
            if (linecleared) {
                Level++;
                lineClearSound.play(); // Jouer le son
            }
            if (nbTot >= 10){
                nbTot -= 10;
                Level++;
            }


            // Si la pièce actuelle ne peut plus bouger, placer la prochaine pièce
            if (test) {
                if (grid.over(piece)) {
                    // Si la pièce est bloquée en haut, c'est un game over
                    gameOver = true;

                    //Joue le son
                    gameOverSound.play();
                    backgroundMusic.stop();
                } else {
                    piece = nextPiece; // Remplacer par la prochaine pièce
                    nextPiece = generateRandomPiece(4, 0); // Générer une nouvelle prochaine pièce
                    grid.placePiece(piece);
                    test = false;
                }
            }
        }
    
        window.clear(Color::Black);

        

        // Elements à afficher
        drawGrid(window,grid);
        window.draw(txt);
        window.draw(txtlevel);
        
        if (gameOver) {
            window.draw(gameOverText); // Afficher le texte de Game Over
        } else {
            // Afficher la pièce actuelle
            drawPiece(window, piece);

            // Afficher la prochaine pièce (dans une zone dédiée, hors grille)
            nextPieceText.setString("Next Piece:");
            window.draw(nextPieceText);
            drawPieceAtPosition(window, nextPiece, 400, 150); // Dessiner la prochaine pièce à une position fixe
        }

        // Dessiner à l'écran les éléments
        window.display();

        
    }

    return 0;
}

// Chargement de la police d'écriture
void LoadFont(){
    if(!font.loadFromFile("res/Poppins-Regular.ttf")){
        std::cout << "Erreur chargement font" << std::endl;
    }
}

// Définir le score
void SetScore(Text& txt, int score){
    txt.setPosition(400,50);
    txt.setFont(font);


    snprintf(temp, sizeof(temp), "Score : %d", score);
    txt.setString(temp);


    txt.setCharacterSize(26);
    txt.setFillColor(Color::White);
    txt.setStyle(Text::Bold);

}

// Définir le score
void SetLevel(Text& txtlevel, int Level){
    txtlevel.setPosition(500,350);
    txtlevel.setFont(font);


    snprintf(temp, sizeof(temp), "Level : %d", Level);
    txtlevel.setString(temp);


    txtlevel.setCharacterSize(40);
    txtlevel.setFillColor(Color::Red);
    txtlevel.setStyle(Text::Bold);

}

void CheckBtn(Piece& piece,Piece& nextPiece, Grid& grid, bool& test){

    static bool spacePressedPreviously = false; // Résolution du problème de rotation où la fonction rotatePiece() est appelée 2 fois à chaque appui sur entrée : essayer de comprendre pourquoi plus tard
    static Clock moveClock;                     // Horloge pour limiter la vitesse des mouvements
    static int moveCooldown = 150;              // Temps d'attente entre deux mouvements (en ms)

    // Vérifier si suffisamment de temps s'est écoulé depuis le dernier mouvement
    if (moveClock.getElapsedTime().asMilliseconds() >= moveCooldown) {
        if (input.getButton().left == true) {
            test = grid.movePiece(piece, -1, 0); // Déplacement à gauche
            moveClock.restart();                // Réinitialiser l'horloge après un mouvement
        }
        if (input.getButton().right == true) {
            test = grid.movePiece(piece, 1, 0); // Déplacement à droite
            moveClock.restart();
        }
        if (input.getButton().down == true) {
            test = grid.movePiece(piece, 0, 1); // Déplacement rapide vers le bas
            moveClock.restart();
        }
    }
    if (input.getButton().space == true) {
            test = grid.movePiece(piece, 0, 1); // Déplacement rapide vers le bas
            moveClock.restart();
        }
    if(input.getButton().r == true && gameOver){
        score = 0;
        Level = 1;
        nbTot = 0;
        grid.empty();
        piece = generateRandomPiece(4, 0);
        nextPiece = generateRandomPiece(4, 0);
        grid.placePiece(piece);
        gameOver = false;
        test = false;

        backgroundMusic.play(); // Reprendre la musique de fond
    }
    if(input.getButton().up == true && !spacePressedPreviously){
        grid.rotatePiece(piece);
        rotateSound.play(); // Jouez le son de rotation
    }
    spacePressedPreviously = input.getButton().up;
}

// Dessiner une pièce à une position spécifique
void drawPieceAtPosition(RenderWindow& window, Piece piece, int offsetX, int offsetY) {
    std::vector<Case> cases = piece.getCases();
    char type = piece.getType();

    Color color;
    switch (type) {
        case 'I': color = Color::Red; break;
        case 'O': color = Color::Blue; break;
        case 'L': color = Color::Green; break;
        case 'J': color = Color::Red; break;
        case 'T': color = Color::Cyan; break;
        case 'Z': color = Color::Yellow; break;
        case 'S': color = Color::Magenta; break;
    }

    for (const Case& c : cases) {
        uint x = c.getX();
        uint y = c.getY();

        RectangleShape Cell(Vector2f(caseSize - 1, caseSize - 1));
        Cell.setPosition(offsetX + x * caseSize, offsetY + y * caseSize);
        Cell.setFillColor(color);
        Cell.setOutlineColor(Color::White);
        Cell.setOutlineThickness(1);

        window.draw(Cell);
    }
}