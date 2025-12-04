#include <FEHLCD.h>
#include <FEHImages.h>
#include <FEHUtility.h>
#include <FEHRandom.h>
#include <FEHKeyboard.h>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

/*
This is the Monster class that keeps track of the monster's evolutions, sprites, animation
movement, and saved monsters

There is an instance of the Monster class in the Menu class so the player can influence the monster's evolution
through feeding or training.

Name and Level are public so the Unit class can easily modify them.
*/

class Monster
{
    public:
        Monster();
        void idle_animation();
        void movement();
        void switch_monster(int feed, int train);

        void set_saved(int index, int value);
        int get_saved(int index);

        char name[30];
        int level;

    private:
        char sprite_name[30];

        int x, y;

        int frame;

        float delta_animation;
        float start_animation;
        float timer_animation;
        float duration_animation;

        float delta_movement;
        float start_movement;
        float timer_movement;
        float duration_movement;

        int x_speed, y_speed;
        int x_dir, y_dir;

        int random[5];

        int saved[7];
};

/*
This constructor sets an array saved to all 0's, indicating you've collected no monsters.

The monster's name and sprite location are set, and the first element of the saved array is set to 1 to indicate
you've collected the first monster.

Level is set to 1, the monsters initial location is set, and an animation timer and movement is set to be 1 second.

Finally, an array random is set to a random number. This array will influence monster movement.
*/

Monster::Monster()
{
    for (int i = 0; i < 7; i++)
    {
        saved[i] = 0;
    }

    strcpy(name, "Orb");
    strcpy(sprite_name, "assets/Orb1.png");
    saved[0] = 1;

    level = 1;

    x = 120;
    y = 120;

    frame = 1;

    start_animation = 0.0;
    start_movement = 0.0;

    duration_animation = 1.0;
    duration_movement = 1.0;

    for (int i = 0; i < 5; i++)
    {
        random[i] = Random.RandInt();
    }
}

/*
The switch monster function checks the players feed and training to change the sprite of the monster, the name, and sets
indicates you've collected that monster by altering the saved array.
*/

void Monster::switch_monster(int feed, int train)
{
    if (level == 2 && feed >= 2 && train >= 2 && strcmp(name, "Mason") == 0)
    {
        strcpy(name, "Gato");
        saved[3] = 1;
        
    }
    else if (level == 2 && strcmp(name, "Mason") == 0)
    {
        strcpy(name, "Skibidi");
        saved[4] = 1;
    }

    if (level == 2 && feed >= 2 && train >= 2 && strcmp(name, "Glorb") == 0)
    {
        strcpy(name, "Chicken");
        saved[5] = 1;
    }
    else if (level == 2 && strcmp(name, "Glorb") == 0)
    {
        strcpy(name, "Sphere");
        saved[6] = 1;
    }

    if (level == 1 && feed >= 2 && train >= 2)
    {
        strcpy(name, "Mason");
        saved[1] = 1;
        level++;
    }
    else if (level == 1)
    {
        strcpy(name, "Glorb");
        saved[2] = 1;
        level++;
    }
}

/*
Simply returns if the monster is collected at a given index
*/

int Monster::get_saved(int index)
{
    return saved[index];
}

// sets a value for array saved at a given index

void Monster::set_saved(int index, int value)
{
    saved[index] = value;
}

/*
Using an array of random numbers, the time of movement is randomized, the x/y direction, and x/y speed are randomized.

This creates the illusion that the monster is moving naturally.
*/

void Monster::movement()
{
    timer_movement = TimeNow();

    delta_movement = timer_movement - start_movement;

    if (random[0] >= 16384)
    {
        duration_movement = 1.0;
    }
    else
    {
        duration_movement = 0.5;
    }

    if (random[1] >= 16384)
    {
        x_dir = 1;
    }
    else
    {
        x_dir = -1;
    }

    if (random[2] >= 16384)
    {
        y_dir = 1;
    }
    else
    {
        y_dir = -1;
    }

    if (random[3] >= 16384)
    {
        x_speed = 1;
    }
    else
    {
        x_speed = .5;
    }

    if (random[4] >= 16384)
    {
        y_speed = 1;
    }
    else
    {
        y_speed = .5;
    }

    x += x_dir * x_speed;
    y += y_dir * y_speed;
    
    if (x < 1)
    {
        x = 1;
    }

    if (x > 255)
    {
        x = 255;
    }

    if (y < 55)
    {
        y = 55;
    }

    if (y > 135)
    {
        y = 135;
    }

    if (delta_movement > duration_movement)
    {
        for (int i = 0; i < 5; i++)
        {
            random[i] = Random.RandInt();
        }
        
        start_movement = timer_movement;
    }
}

/*
The idle animation function draws the monster to the screen at its position and uses its name to open the correct sprite.

Using the animation timer, the sprite switches frames every 1 second.
*/

void Monster::idle_animation()
{
    timer_animation = TimeNow();

    delta_animation = timer_animation - start_animation;

    if (delta_animation > duration_animation)
    {
        if (frame == 1)
        {
            strcpy(sprite_name, "");
    
            strcat(sprite_name, "assets/");
            strcat(sprite_name, name);
            strcat(sprite_name, "1.png");

            frame = 2;
        }
        else if (frame == 2)
        {

            strcpy(sprite_name, "");
    
            strcat(sprite_name, "assets/");
            strcat(sprite_name, name);
            strcat(sprite_name, "2.png");
            frame = 1;
        }
        start_animation = timer_animation;
    }

    FEHImage mon;
    mon.Open(sprite_name);
    mon.Draw(x, y);
}

/*
The Menu class is for checking states, influencing data like feed and train, and changing screens

An instance of the Menu class is in the Game class specifically to 'change states' where each state is a screen
like the instructions screen or the info screen
*/

class Menu
{
    public:
        Menu();
        void start();
        void credits();
        void saved();
        void main();
        void info();
        void feed();
        void train();
        void instructions();
        int get_state();
        void set_pointers(int *x_t, int *y_t);

    private:
        int state;
        int *x, *y;

        Monster monster;

        float timer_main;
        float start_main;
        float delta_main;
        float duration_main;

        float timer_train;
        float start_train;
        float delta_train;
        float duration_train;    

        int feed_pts;
        int train_pts;

        int train_counter;

};

/*
The Menu constructor sets many counters to 0.

It additionally sets the duration it takes for an evolution through the duration_main variable
and also the time you stay in the train menu if you don't click space fast enough.
*/

Menu::Menu()
{
    state = 0;
    start_main = 0.0;
    duration_main = 45.0;

    start_train = 0.0;
    duration_train = 10.0;

    feed_pts = 0;
    train_pts = 0;

    train_counter = 0;
}

/*
The start function of the Menu class is the starting scene.

The starting scene displays options to play the game, look at your saved monsters, credits, and instructions.

It additionally has the monster in its position to remind the player what monster they currently have.
*/

void Menu::start()
{
    LCD.SetFontScale(.5);
    LCD.SetFontColor(LIGHTBLUE);

    LCD.WriteLine("Play Monster Simulator!");
    LCD.WriteLine("1: Game");
    LCD.WriteLine("2: Saved Monsters");
    LCD.WriteLine("3: Credits");
    LCD.WriteLine("4: Instructions");

    LCD.FillCircle(50, 220, 15);
    LCD.FillCircle(120, 220, 15);
    LCD.FillCircle(190, 220, 15);
    LCD.FillCircle(260, 220, 15);
    LCD.DrawCircle(50, 220, 15);
    LCD.DrawCircle(120, 220, 15);
    LCD.DrawCircle(190, 220, 15);
    LCD.DrawCircle(260, 220, 15);

    LCD.SetFontScale(1.0);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("1", 43, 210);
    LCD.WriteAt("2", 113, 210);
    LCD.WriteAt("3", 183, 210);
    LCD.WriteAt("4", 255, 210);

    monster.idle_animation();

    if (*x > 35 && *x < 65 && *y > 205 && *y < 235)
    {
        state = 1;
        Sleep(.1);
    }
    else if (*x > 105 && *x < 135 && *y > 205 && *y < 235)
    {
        state = 2;
    }
    else if (*x > 175 && *x < 205 && *y > 205 && *y < 235)
    {
        state = 3;
    }
    else if (*x > 245 && *x < 275 && *y > 205 && *y < 235)
    {
        state = 4;
    }
}

/*
The main function of the Menu class is the main scene where the game occurs.

This starts the evolution timer as well as presents options to give info, feed, or train.

This function utilizes the movement and idle animation of the Monster class to have the monster move around 
while also having collisions at the edges of the screen (excluding UI).

There are ways to get back to the starting screen.

You can also reset the evolution to the starting monster while still retaining your collected monsters
in the saved monster screen on the starting menu.
*/

void Menu::main()
{
    timer_main = TimeNow();

    delta_main = timer_main - start_main;

    LCD.SetFontColor(LIGHTBLUE);
    LCD.SetFontScale(0.5);

    LCD.WriteLine("1: Info");
    LCD.WriteLine("2: Feed");
    LCD.WriteLine("3: Train");
    LCD.WriteLine("Press r to restart monster");
    LCD.WriteLine("Press p to restart everything (warning)");
    LCD.WriteLine("Press ESC to main menu");
    
    LCD.FillCircle(100, 220, 15);
    LCD.FillCircle(160, 220, 15);
    LCD.FillCircle(220, 220, 15);
    LCD.DrawCircle(100, 220, 15);
    LCD.DrawCircle(160, 220, 15);
    LCD.DrawCircle(220, 220, 15);

    LCD.SetFontScale(1.0);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("1", 93, 210);
    LCD.WriteAt("2", 153, 210);
    LCD.WriteAt("3", 213, 210);

    LCD.SetFontColor(LIGHTBLUE);
    LCD.SetFontScale(0.5);

    monster.movement();
    monster.idle_animation();

    if (delta_main > duration_main)
    {
        monster.switch_monster(feed_pts, train_pts);
        feed_pts = 0;
        train_pts = 0;
        train_counter = 0;
        start_main = timer_main;
    }

    if (*x > 85 && *x < 115 && *y > 205 && *y < 235)
    {
        state = 5;
    }
    else if (*x > 145 && *x < 175 && *y > 205 && *y < 235)
    {
        state = 6;
    }
    else if (*x > 205 && *x < 235 && *y > 205 && *y < 235)
    {
        state = 7;
    }

    if(Keyboard.isPressed(KEY_R))
    {
        strcpy(monster.name, "Orb");
        monster.level = 1;
    }

    if(Keyboard.isPressed(KEY_P))
    {
        strcpy(monster.name, "Orb");
        monster.level = 1;
        for (int i = 1; i < 7; i++)
        {
            monster.set_saved(i, 0);
        }
        state = 0;
    }

    if(Keyboard.isPressed(KEY_ESCAPE))
    {
        state = 0;
    }
}

/*
The info function is the info screen that displays the monster's name, its feed and train.

It's important to note that feed and train will be reset each evolution.
*/

void Menu::info()
{

    LCD.Write("Your current monster is: ");
    LCD.WriteLine(monster.name);
    LCD.WriteLine("Your feed and train is at");
    LCD.WriteLine(feed_pts);
    LCD.WriteLine(train_pts);
    LCD.WriteAt("Press ESC to game menu", 1, 220);

    if(Keyboard.isPressed(KEY_ESCAPE))
    {
        state = 1;
        Sleep(.2);
    }
}

/*
A gray box is displayed at 7 locations of the saved screen until the play unlocks the monster.

Even if they reset the evolution, the saved monster will be displayed, allowing the player to unlock all evolutions.
*/

void Menu::saved()
{
    LCD.SetFontColor(GRAY);
    LCD.WriteAt("Press ESC to main menu", 1, 220);

    if (monster.get_saved(0) == 1)
    {
        FEHImage mon1;
        mon1.Open("assets/Orb1.png");
        mon1.Draw(5, 5);
    }

    if (monster.get_saved(1) == 1)
    {
        FEHImage mon2;
        mon2.Open("assets/Mason1.png");
        mon2.Draw(70, 5);
    }
    else {LCD.DrawRectangle(70,5,40,40); LCD.FillRectangle(70,5,40,40);}

    if (monster.get_saved(2) == 1)
    {
        FEHImage mon3;
        mon3.Open("assets/Glorb1.png");
        mon3.Draw(140, 5);
    }
    else {LCD.DrawRectangle(140,5,40,40); LCD.FillRectangle(140,5,40,40);}

    if (monster.get_saved(3) == 1)
    {
        FEHImage mon4;
        mon4.Open("assets/Gato1.png");
        mon4.Draw(200, 5);
    }
    else {LCD.DrawRectangle(210,5,40,40); LCD.FillRectangle(210,5,40,40);}

    if (monster.get_saved(4) == 1)
    {
        FEHImage mon5;
        mon5.Open("assets/Skibidi1.png");
        mon5.Draw(10, 150);
    }
    else {LCD.DrawRectangle(10,130,40,40); LCD.FillRectangle(10,130,40,40);}

    if (monster.get_saved(5) == 1)
    {  
        FEHImage mon6;
        mon6.Open("assets/Chicken1.png");
        mon6.Draw(100, 110);
    }
    else {LCD.DrawRectangle(100, 130,40,40); LCD.FillRectangle(100,130,40,40);}

    if (monster.get_saved(6) == 1)
    {
        FEHImage mon7;
        mon7.Open("assets/Sphere1.png");
        mon7.Draw(200, 130);
    }
    else {LCD.DrawRectangle(200,130,40,40); LCD.FillRectangle(200,130,40,40);}
    
    if(Keyboard.isPressed(KEY_ESCAPE))
    {
        state = 0;
    }
}

/*
Presents the instructions for the game
*/

void Menu::instructions()
{
    LCD.SetFontColor(LIGHTBLUE);
    LCD.SetFontScale(.5);
    LCD.WriteLine("Evolve your monsters");
    LCD.WriteLine("Feed twice and Train twice");
    LCD.WriteLine("for a good evolution");
    LCD.WriteLine("Collect all 7 monsters");
    LCD.WriteLine("Each evolution takes 45 sec");
    LCD.WriteLine("Press space 15x to successfully train");
    LCD.WriteLine("Hover buttons to select them");
    LCD.WriteLine("Move mouse above button after hovering");
    LCD.WriteAt("Press ESC to main menu", 1, 220);

    if(Keyboard.isPressed(KEY_ESCAPE))
    {
        state = 0;
    }
}

/*
Presents the credits for the game
*/

void Menu::credits()
{
    LCD.SetFontColor(LIGHTBLUE);
    LCD.SetFontScale(0.5);

    LCD.WriteLine("Created by Tom Williams");
    LCD.WriteLine("and Aditya Dhati");
    LCD.WriteLine("Inspired by Tamagotchi");
    LCD.WriteLine("and Digimon games");
    LCD.WriteAt("Press ESC to main menu", 1, 220);

    if(Keyboard.isPressed(KEY_ESCAPE))
    {
        state = 0;
    }
}

/*
This screen will display that the feed has increased. Therefore, the feed_pts member variable is increased.
*/

void Menu::feed()
{
    feed_pts++;
    LCD.WriteLine("Plus 1 Feed!");
    Sleep(2.0);
    state = 1;
}

/*
This screen will display a training game. If space is clicked 15x, train_pts member variable is increased.
*/

void Menu::train()
{
    LCD.WriteLine("Press space repeatedly!");
    LCD.WriteLine(train_counter);

    timer_train = TimeNow();

    delta_train = timer_train - start_train;

    if (Keyboard.isPressed(KEY_SPACE))
    {
        train_counter++;
        Sleep(.1);
    }

    if (train_counter >= 15)
    {
        train_pts++;
        train_counter = 0;
        state = 1;
    }
    else if (delta_train > duration_train)
    {
        train_counter = 0;
        start_train = timer_train;
        state = 1;
    }
}

// just returns the current state (which describes what screen the game is on)

int Menu::get_state()
{
    return state;
}

/*
This obtains the x and y coordinates of the screen that is passed from the Game class which is passed from the main function.
*/

void Menu::set_pointers(int *x_t, int *y_t)
{
    x = x_t;
    y = y_t;
}

/*
The Game class's important tasks are to keep track of mouse location, drawing to the screen, and changing the scene based 
on the state in the Menu class.
*/
class Game
{
    public:
        Game(int x_t, int y_t);
        void update();
        void draw();
        void events();

    private:
        int x, y;
        Menu game_menu;
};

/*
The constructor obtains the x and y postions and passes their pointers to the Game's instance of the Menu object.
*/

Game::Game(int x_t, int y_t)
{
    x = x_t;
    y = y_t;
    game_menu.set_pointers(&x, &y);
}

/*
Checks if the x or y is being pressed and will display on the screen for debug purposes
*/

void Game::events()
{
    if(LCD.Touch(&x, &y))
    {
        LCD.WriteLine(x);
        LCD.WriteLine(y);
    }
}

//simply runs LCD.Update(); just makes it cleaner if everything is in the game class

void Game::update()
{
    LCD.Update();
}

/*
The draw function of the Game class is a very important function.

This checks for what state the Menu object is on currently and runs the appropriate Menu function or scene.

This is the entry point for the menus, monsters, etc.
*/

void Game::draw()
{
    LCD.Clear(NAVY);

    switch (game_menu.get_state())
    {
        case 0:
            game_menu.start();
            break;
        case 1:
            game_menu.main();
            break;
        case 2:
            game_menu.saved();
            break;
        case 3:
            game_menu.credits();
            break;
        case 4:
            game_menu.instructions();
            break;
        case 5:
            game_menu.info();
            break;
        case 6:
            game_menu.feed();
            break;
        case 7:
            game_menu.train();
            break;
        default:
            cout << "bleh" << endl;
    }
}

/*
The main functions key role is to create the x and y position for the screen and initalizing a Game object.

In the main game loop, important functions of the Game class are called.
*/

int main()
{
    int xPos, yPos;
    Game game(xPos, yPos);
    
    LCD.Clear(NAVY);
    
    while (1) 
    {
        game.draw();
        game.events();
        game.update();
    }

    return 0;
}
