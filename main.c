//LENNY MORICONI

//BUT : algorithme qui permet à un utilisateur de jouer au jeu du Snake. L'utilisateur contrôle un serpent d'une taille initiale de 3, qui lorsqu'il avale de la nourriture, grandit d'une case et marque 10 points. Lorsque le serpent heurte un mur ou son propre corps, la partie prend fin
//ENTREE : un tableau t contenant le terrain, les coordonnées x et y de base du serpent et les coordonnées aléatoire de la nourriture
//SORTIE : jeu du snake, score, highscore

//BIBLIOTHEQUES
#include <stdio.h>
#include <stdlib.h>
#include <time.h>       //pour utiliser la fonction _sleep()
#include <windows.h>    //pour utiliser le type structuré COORD
#include <conio.h>      //pour utiliser les fonctions _getch() et kbhit()

//CONSTANTES
#define COLS 30             //colonnes
#define ROWS 20             //lignes
#define SNAKE_X 15          //position x de la tête au début
#define SNAKE_Y 10          //position y de la tête au début
#define SNAKE_LEN 3         //taille du corps du serpent au début
#define MAXLENGTH 200       //taille maximale que peut atteindre le serpent
#define UP 122              //code du caractere 'z'
#define LEFT 113            //code du caractere 'q'
#define DOWN 115            //code du caractere 's'
#define RIGHT 100           //code du caractere 'd'
#define MAXSPEED 200        //nombre de millisecondes de la fonction _sleep pour les déplacements
#define C_HEAD_RIGHT '>'    //tête serpent droite
#define C_HEAD_LEFT '<'     //tête serpent gauche
#define C_HEAD_UP '^'       //tête serpent haut
#define C_HEAD_DOWN 'v'     //tête serpent bas
#define C_BODY '*'          //corps du serpent
#define C_FOOD 'o'          //nourriture
#define C_WALL '!'          //murs
#define C_EMPTY ' '         //vide

//TYPE ENUMERE
enum Bool{
    False, True     //False = 0 & True = 1
};

//PROTOTYPES
void tryAgain(int *ptScore, int *ptHighscore);
void collision(COORD snake[MAXLENGTH], COORD *ptFood, enum Bool *ptGameOver, int *ptLen, enum Bool *ptIsFood);
void motion(COORD snake[MAXLENGTH], COORD *ptFood, enum Bool *ptGameOver, int *ptLen, int keyPressed, int *ptScore, int *ptHighscore);
void input(COORD snake[MAXLENGTH], COORD *ptFood, enum Bool *ptGameOver, int *ptLen, int *ptLastKey, int *ptScore, int *ptHighscore);
void placeFood(COORD food);
void placeSnake(COORD snake[MAXLENGTH], int len, int keyPressed);
void gotoxy(int x, int y);
void display(int t[COLS][ROWS], int score, int *ptHighscore);
void initArray(int t[COLS][ROWS]);
void setFood(COORD *ptFood, COORD snake[MAXLENGTH], int len);
void setSnake(COORD snake[MAXLENGTH], int x, int y, int len);
void newgame(int *ptHighscore);
void rules();

//PROGRAMME PRINCIPAL
int main()
{
    int choice = 0;
    int highscore = 0;

    do{
        system("cls");
        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ SNAKE //////////////////\n\n");
        printf("1 = New game\n");
        printf("2 = Rules of the game\n");
        printf("0 = Quit\n\n");
        printf("Your choice = ");
        scanf("%1d",&choice);
        fflush(stdin);
        switch(choice){
            case 0 :
                system("cls");
                printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ SNAKE //////////////////\n\n");
                printf("Thank you for playing. See you next time !\n\n");
                printf("CREDITS : Lenny Moriconi\n\n");
                break;
            case 1 :
                newgame(&highscore);
                break;
            case 2 :
                rules();
                break;
            default :
                system("cls");
                printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ SNAKE //////////////////\n\n");
                printf("Wrong input, please try again.");
                getc(stdin);
                fflush(stdin);
                break;
        }
    }while(choice!=0);

    system("pause");

    return 0;
}

//DECLARATIONS PROCEDURES

void tryAgain(int *ptScore, int *ptHighscore){
//BUT : procédure servant à afficher un écran de game over ainsi que le score réalisé par le joueur au cours de la partie, puis lui propose de rejouer
//ENTREE : score et highscore
//SORTIE : écran de game over et choix de recommencer une partie ou non

    char choice;

    do{
        system("cls");
        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ SNAKE //////////////////\n\n");
        printf("GAME OVER\n\n");
        printf("Score = %d\n\n", *ptScore);
        printf("Try again ? O/N\n\n");
        printf("Your choice : ");
        choice = getc (stdin);
        fflush(stdin);

        if(choice != 'O' && choice != 'N'){
            system("cls");
            printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ SNAKE //////////////////\n\n");
            printf("Wrong input, please try again.");
            getc(stdin);
            fflush(stdin);
        }
    }while(choice != 'O' && choice != 'N');

    if(choice == 'O'){
        newgame(ptHighscore);
    }
}

void collision(COORD snake[MAXLENGTH], COORD *ptFood, enum Bool *ptGameOver, int *ptLen, enum Bool *ptIsFood){
//BUT : détecter une collision entre le serpent et un objet ou les murs
//ENTREE : coordonnées du serpent et de la nourriture, taille du serpent, booléens isFood et gameOver
//SORTIE : gameOver devient vrai si un mur ou le corps du serpent sont détectés, isFood devient vrai si la nourriture est détectée, sinon rien

    int i = 0;

    if(snake[0].X == ptFood->X && snake[0].Y == ptFood->Y){     //si la tête du serpent rencontre la nourriture alors sa taille augmente de 1 et un nouveau fruit est placé sur le terrain
        *ptLen = *ptLen+1;
        setFood(ptFood, snake, *ptLen);
        placeFood(*ptFood);
        *ptIsFood = True;
    }
    else if(snake[0].X == 0 || snake[0].X == COLS-1 || snake[0].Y == 0 || snake[0].Y == ROWS-1){  //si le serpent heurte un mur, il meurt
        *ptGameOver = True;
    }
    else{
        for(i=1;i<*ptLen;i++){
            if(snake[0].X == snake[i].X && snake[0].Y == snake[i].Y){       //si la tête du serpent entre en contract avec une partie du corps, il meurt
                *ptGameOver = True;
            }
        }
    }
}

void motion(COORD snake[MAXLENGTH], COORD *ptFood, enum Bool *ptGameOver, int *ptLen, int keyPressed, int *ptScore, int *ptHighscore){
//BUT: procédure qui fait avancer le serpent selon la touche pressée et augmente le score
//ENTREE : snake, food, gameover, len, keyPressed, score, highscore
//SORTIE : coordonnées x et y du serpent modifié, incrémentation du score si de la nourriture a été détectée
    enum Bool isFood = False;   //sert à détecter qu'un fruit a été rencontré
    int i = 0;
    int lastX = 0;
    int lastY = 0;

    //on efface le dernier caractère du serpent
    gotoxy(snake[*ptLen-1].X,snake[*ptLen-1].Y);
    printf("%c",C_EMPTY);

    //on sauvegarde les coordonnées de la queue, au cas où le serpent grandit
    lastX = snake[*ptLen-1].X;
    lastY = snake[*ptLen-1].Y;

    //on part de la fin du tableau, chaque case prend la valeur de la suivante, jusqu'à la tête
    for(i=1;i<*ptLen;i++){
        snake[*ptLen-i].X = snake[*ptLen-i-1].X;
        snake[*ptLen-i].Y = snake[*ptLen-i-1].Y;
    }

    //selon la direction du serpent, on modifie les coordonnées de la tête (première position du tableau)
    switch(keyPressed){
        case UP :
            snake[0].Y--;
            break;
        case LEFT :
            snake[0].X--;
            break;
        case DOWN :
            snake[0].Y++;
            break;
        case RIGHT :
            snake[0].X++;
            break;
    }

    //on verifie la collision
    collision(snake, ptFood, ptGameOver, ptLen, &isFood);

    //si un fruit a été rencontré, alors on set les coordonnées de la queue, afin d'éviter les bugs d'affichage puis on incrémente le score de 10 points
    if(isFood == True){
        //on set les coordonnées de la nouvelle case occupée du tableau
        snake[*ptLen-1].X = lastX;
        snake[*ptLen-1].Y = lastY;
        //on augmente le score de 10
        *ptScore += 10;
        //si le score actuel est supérieur au meilleur score, alors le meilleur score devient le score actuel
        if(*ptScore > *ptHighscore){
            *ptHighscore = *ptScore;
        }
        //affichage du score
        gotoxy(0, 21);
        printf("Score = %d\n", *ptScore);
        gotoxy(0, 23);
        printf("Highscore = %d\n", *ptHighscore);
    }

    //on affiche le serpent
    placeSnake(snake, *ptLen, keyPressed);

    _sleep(MAXSPEED);
}

void input(COORD snake[MAXLENGTH], COORD *ptFood, enum Bool *ptGameOver, int *ptLen, int *ptLastKey, int *ptScore, int *ptHighscore){
//BUT : procédure qui demande à l'utilisateur de saisir une des touches de direction et qui appelle la procédure correspondante

    int keyPressed = 0;     //touche pressée

    keyPressed = _getch();
    do{
        switch(keyPressed){
            case UP :        //'z'
                if(*ptLastKey != DOWN){                     //on ne peut pas se déplacer dans le sens inverse de l'axe actuel
                    motion(snake, ptFood, ptGameOver, ptLen, keyPressed, ptScore, ptHighscore);
                    *ptLastKey = keyPressed;
                }
                else
                    motion(snake, ptFood, ptGameOver, ptLen, *ptLastKey, ptScore, ptHighscore);
                break;
            case LEFT :      //'q'
                if(*ptLastKey != RIGHT){                    //on ne peut pas se déplacer dans le sens inverse de l'axe actuel
                    motion(snake, ptFood, ptGameOver, ptLen, keyPressed, ptScore, ptHighscore);
                    *ptLastKey = keyPressed;
                }
                else
                    motion(snake, ptFood, ptGameOver, ptLen, *ptLastKey, ptScore, ptHighscore);
                break;
           case DOWN :      //'s'
                if(*ptLastKey != UP){                       //on ne peut pas se déplacer dans le sens inverse de l'axe actuel
                    motion(snake, ptFood, ptGameOver, ptLen, keyPressed, ptScore, ptHighscore);
                    *ptLastKey = keyPressed;
                }
                else
                    motion(snake, ptFood, ptGameOver, ptLen, *ptLastKey, ptScore, ptHighscore);
                break;
            case RIGHT :      //'d'
                if(*ptLastKey != LEFT){                     //on ne peut pas se déplacer dans le sens inverse de l'axe actuel
                    motion(snake, ptFood, ptGameOver, ptLen, keyPressed, ptScore, ptHighscore);
                    *ptLastKey = keyPressed;
                }
                else
                    motion(snake, ptFood, ptGameOver, ptLen, *ptLastKey, ptScore, ptHighscore);
                break;
        }
    }while(!kbhit() && *ptGameOver == False);
}

void placeFood(COORD food){
//BUT : procédure qui place une nourriture sur l'écran

    gotoxy(food.X,food.Y);
    printf("%c",C_FOOD);
}

void placeSnake(COORD snake[MAXLENGTH], int len, int keyPressed){
//BUT : procédure qui place le serpent sur l'écran

    int i = 0;

    for(i=0;i<len;i++){
        gotoxy(snake[i].X,snake[i].Y);
        if(i==0){                               //tête
            switch(keyPressed){
                case UP :
                    printf("%c\n",C_HEAD_UP);
                    break;
                case LEFT :
                    printf("%c\n",C_HEAD_LEFT);
                    break;
                case DOWN :
                    printf("%c\n",C_HEAD_DOWN);
                    break;
                case RIGHT :
                    printf("%c\n",C_HEAD_RIGHT);
                    break;
            }
        }
        else{                                   //corps
            printf("%c",C_BODY);
        }
    }
}

void gotoxy(int x, int y){
//BUT : placer le curseur de la console sur une position donnée

    int i = 0;

    COORD coord = {0, 0};
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void display(int t[COLS][ROWS], int score, int *ptHighscore){
//BUT : procédure qui affiche un tableau t
//ENTREE : tableau t rempli
//SORTIE : affichage du tableau

    int i,j = 0;

    for(j=0;j<ROWS;j++){
        for(i=0;i<COLS;i++){
            if(t[i][j] == 1)
                printf("%c",C_WALL);           //murs
            else
                printf("%c",C_EMPTY);          //vide
        }
        printf("\n");
    }

    //affichage du score
    gotoxy(0, 21);
    printf("Score = %d\n", score);
    gotoxy(0, 23);
    printf("Highscore = %d\n", *ptHighscore);
}

void initArray(int t[COLS][ROWS]){
//BUT : procédure qui initialise un tableau d'entiers
//ENTREE : tableau t vide
//SORTIE : tableau t rempli de 0 et de 1

    int i=0,j=0;

    for(i=0;i<COLS;i++){
        for(j=0;j<ROWS;j++){
            if(j==0 || j==ROWS-1)
                t[i][j] = 1;
            else if(i==0 || i==COLS-1)
                t[i][j] = 1;
            else
                t[i][j] = 0;
        }
    }
}

void setFood(COORD *ptFood, COORD snake[MAXLENGTH], int len){
//BUT : procédure qui set aléatoirement les attributs x et y de la nourriture
//ENTREE : food et snake
//SORTIE : x et y de food modifiés

    int i = 0;
    int newX = 0, newY = 0;
    enum Bool equal = True;  //booleen pour vérifier si les coordonnées de food sont identiques aux coordonnées d'une partie du serpent

    srand(time(NULL));  //initialisation nombres aléatoires

    //boucle tant que les nouvelles coordonnées ne sont pas égales à la position d'un mur ou du serpent
    do{
        newX = rand()%COLS;
        newY = rand()%ROWS;
        do{
            if(newX == snake[i].X && newY == snake[i].Y || newX <= 0 || newX >= COLS-1 || newY <= 0 || newY >= ROWS-1)
                equal = True;
            else
                equal = False;
            i++;
        }while(i < len);
    }while(equal == True);

    ptFood -> X = newX;
    ptFood -> Y = newY;
}


void setSnake(COORD snake[MAXLENGTH], int x, int y, int len){
//BUT : procédure qui set les x et y des cellules du tableau snake, selon la taille du serpent
//ENTREE : tableau snake
//SORTIE : tableau rempli

    int i = 0;

    for(i=0;i<len;i++){
        snake[i].X = x-i;
        snake[i].Y = y;
    }
}

void newgame(int *ptHighscore){
//BUT : procédure qui démarre une nouvelle partie et appelle les procédures nécessaires au déroulement de la partie

    enum Bool gameOver = False;     //booleen pour mettre fin à la partie
    COORD snake[MAXLENGTH];         //serpent => type structuré contenant un x et un y
    COORD food;                     //nourriture => type structuré contenant un x et un y
    int t[COLS][ROWS];              //tableau
    int len = SNAKE_LEN;            //taille du serpent
    int lastKey = RIGHT;            //dernière touche pressée, de base 'd', placé ici pour éviter qu'il soit réinitialisé à chaque déplacement
    int score = 0;

    system("cls");

    //on set la position du serpent puis de la nourriture

    setSnake(snake, SNAKE_X, SNAKE_Y, len);
    setFood(&food, snake, len);

    //initialisation & affichage
    initArray(t);                           //initialiser tableau
    display(t, score, ptHighscore);                             //afficher tableau
    placeSnake(snake, len, RIGHT);          //placer serpent sur l'écran
    placeFood(food);                        //placer nourriture sur l'écran

    //lancement du jeu
    do{
        input(snake, &food, &gameOver, &len, &lastKey, &score, ptHighscore);            //tant que la partie n'est pas finie, l'utilisateur peut saisir une touche
    }while(gameOver == False);

    tryAgain(&score, ptHighscore);
}

void rules(){
    system("cls");
    printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ SNAKE //////////////////\n\n");
    printf("CONTROLS\n\n");
    printf("z = Up\n");
    printf("q = Left\n");
    printf("s = Down\n");
    printf("d = Right\n\n\n");
    printf("RULES\n\n");
    printf("You are a snake and your aim is to eat as much food as you can.\n");
    printf("Every time you eat food, you grow up and score 10 points.\n\n");
    printf("You never stop moving, but if you hit the wall or yourself, you die.\n\n\n");
    printf("GOOD LUCK !\n\n\n");
    printf("Enter = Return to the main menu");
    getc(stdin);
    fflush(stdin);
}
