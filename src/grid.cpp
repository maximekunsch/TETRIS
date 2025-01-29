#include "grid.hpp"
#include "piece.hpp"

void Grid::empty() {
        state.clear();
        state.resize(Hauteur, std::vector<char>(Largeur, '.'));
    }

void Grid::placePiece(const Piece& piece) {
        char T = piece.getType();
        for (Case C : piece.getCases()) {
            int x = C.getX();
            int y = C.getY();
            state[y][x] = T; // Utilise le type de la pièce comme symbole
        }
    }

void Grid::clearPiece(const Piece& piece) {
        for (Case C : piece.getCases()) {
            int x = C.getX();
            int y = C.getY();
            state[y][x] = '.'; // Réinitialise en '.'
        }
    }

bool Grid::isCollision(int newX, int newY, char pieceType, const Piece& piece) {
    // Vérifie les limites de la grille
    if (newX < 0 || newX >= Largeur || newY < 0 || newY >= Hauteur) {
        return true;
    }
    // Vérifie les collisions avec d'autres pièces
    if (state[newY][newX] != '.' && state[newY][newX] != pieceType) {
        return true;
    }
    // Vérifie si la case appartient déjà à la même pièce
    if (state[newY][newX] == pieceType && !piece.belong(Case(newX, newY))) {
        return true;
    }
    return false;
}

bool Grid::movePiece(Piece& piece, int dx, int dy) {
    for (const Case& C : piece.getCases()) {
        int newX = C.getX() + dx;
        int newY = C.getY() + dy;

        if (isCollision(newX, newY, piece.getType(), piece)) {
            return (dy == 1); // Si mouvement vers le bas et collision, retournez vrai
        }
    }

    // Déplacer la pièce si aucune collision
    clearPiece(piece);
    piece.move(dx, dy);
    placePiece(piece);

    return (dy == 1 && piece.getMaxY() >= Hauteur - 1); // Retourne vrai si la pièce touche le bas
}


bool Grid::rotatePiece(Piece& piece) {
    // Créer une copie de la pièce pour tester la rotation
    Piece newPiece = piece;
    newPiece.rotate();

    // Vérifier chaque case après la rotation
    for (const Case& c : newPiece.getCases()) {
        int x = c.getX();
        int y = c.getY();

        // Vérifier les limites de la grille
        if (x < 0 || x >= Largeur || y < 0 || y >= Hauteur) {
            return attemptWallKick(piece); // Tenter un Wall Kick si la rotation échoue
        }

        // Vérifier les collisions
        if (state[y][x] != '.') {
            // Si la case est occupée par une pièce du même type mais ne fait pas partie de la pièce actuelle
            if (state[y][x] == piece.getType() && !piece.belong(Case(x, y))) {
                return attemptWallKick(piece); // Tenter un Wall Kick si la rotation échoue
            }

            // Si la case est occupée par une pièce d'un autre type
            if (state[y][x] != piece.getType()) {
                return attemptWallKick(piece); // Tenter un Wall Kick si la rotation échoue
            }
        }
    }

    // Si aucune collision, appliquer la rotation
    clearPiece(piece);    // Retirer la pièce actuelle de la grid
    piece.rotate();       // Appliquer la rotation
    placePiece(piece);    // Mettre à jour la grid
    return true;
}

// Correction d'une rotation qui a échouée à l'aide des Wall Kicks
bool Grid::attemptWallKick(Piece& piece) {
    // Décalages à tester
    std::vector<std::pair<int, int>> wallKicks = {
        {1, 0},   // Déplacement à droite
        {-1, 0},  // Déplacement à gauche
        {0, 1},   // Déplacement vers le bas
        {0, -1}   // Déplacement vers le haut
    };

    // Essayer chaque décalage
    for (auto [dx, dy] : wallKicks) {
        Piece testPiece = piece;
        testPiece.move(dx, dy); // Décalage
        testPiece.rotate();     // Rotation

        // Vérification si la rotation est valide après le décalage
        bool valid = true;
        for (const Case& c : testPiece.getCases()) {
            int x = c.getX();
            int y = c.getY();

            // Vérifier les limites de grid
            if (x < 0 || x >= Largeur || y < 0 || y >= Hauteur) {
                valid = false;
                break;
            }

            // Vérifier les collisions avec d'autres pièces
            if (state[y][x] != '.' && !piece.belong(Case(x, y))) {
                valid = false;
                break;
            }
        }

        // Si valide, on applique le décalage et la rotation
        if (valid) {
            clearPiece(piece);   // Retirer la pièce actuelle de la grid
            piece.move(dx, dy);  // Appliquer le décalage à la pièce
            piece.rotate();      // Appliquer la rotation
            placePiece(piece);   // Mettre à jour la grid
            return true;         
        }
    }

    // Si aucun Wall Kick ne fonctionne, on retourne false
    return false;
}




void Grid::clearLine(int line){
        for (int col=0; col < Hauteur; ++col){
            state[line][col] = '.'; // Réinitialise en '.'
        }
    }

void Grid::downLine(int i){ // i correspond à l'indice de la ligne que l'on souhaite descendre d'un rang
        state[i+1] = state[i];
    }


void drawGrid(RenderWindow& window, Grid grid){
    // Dessine la grille
    std::vector<std::vector<char>> gridState = grid.getState();
    Color color;
 
    for(int i = 0; i < lignes; ++i){
        for(int j = 0; j< colonnes; ++j){

            switch(gridState[i][j]){
                case '.': // Pièce vide
                    color = Color::Black;
                    break;
                case 'I':
                    color = Color::Red;
                    break;
                case 'O':
                    color = Color::Blue;
                    break;
                case 'L':
                    color = Color::Green;
                    break;
                case 'J':
                    color = Color::Red;
                    break;
                case 'T':
                    color = Color::Cyan;
                    break;
                case 'Z':
                    color = Color::Yellow;
                    break;
                case 'S':
                    color = Color::Magenta;
                    break;
    }

            RectangleShape Cell(Vector2f(caseSize-1, caseSize-1));
            Cell.setPosition(j*caseSize, i*caseSize);
            Cell.setFillColor(color);
            Cell.setOutlineColor(Color::White);
            Cell.setOutlineThickness(1);
            window.draw(Cell);
        }
    }
}

void drawPiece(RenderWindow& window, Piece piece){
    // Dessine une pièce
    std::vector<Case> cases = piece.getCases();
    char type = piece.getType();

    Color color;
    switch(type){
        case 'I':
            color = Color::Red;
            break;
        case 'O':
            color = Color::Blue;
            break;
        case 'L':
            color = Color::Green;
            break;
        case 'J':
            color = Color::Red;
            break;
        case 'T':
            color = Color::Cyan;
            break;
        case 'Z':
            color = Color::Yellow;
            break;
        case 'S':
            color = Color::Magenta;
            break;
    }

    for(int i = 0; i<4; ++i){
        uint x = cases[i].getX();
        uint y = cases[i].getY();

        

        RectangleShape Cell(Vector2f(caseSize-1, caseSize-1));
        Cell.setPosition(x*caseSize, y*caseSize);
        Cell.setFillColor(color);
        Cell.setOutlineColor(Color::White);
        Cell.setOutlineThickness(1);

        window.draw(Cell);
    }
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

Piece generateRandomPiece(uint x, uint y){
    // Tirage d'un entier entre 1 et 7
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 7);
    int i = distrib(gen);
    
    if(i == 1){
        Piece piece('I',x,y);
        return piece;
    }
    else if(i == 2){
        Piece piece('O',x,y);
        return piece;
    }
    else if(i == 3){
        Piece piece('T',x,y);
        return piece;
    }
    else if(i == 4){
        Piece piece('L',x,y);
        return piece;
    }
    else if(i == 5){
        Piece piece('J',x,y);
        return piece;
    }
    else if(i == 6){
        Piece piece('S',x,y);
        return piece;
    }
    else if(i == 7){
        Piece piece('Z',x,y);
        return piece;
    }
}

bool Grid::over(Piece& piece){
    return piece.getMinY() <= 1; // Cas de GameOver
}