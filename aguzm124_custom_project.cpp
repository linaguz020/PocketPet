/*         Angelina Guzman aguzm124@ucr.edu
 *         Discussion Section: 21
 *         Assignment: Custom Lab Project

 *         Pocket Pet:
 *         This project uses a Task Scheduler and synchSMs to create a pet simulator. A Nokia 5110 LCD screen is used
 *         to display the game. The cat and dog are drawn by writing to individual pixels on the LCD. Feed, Play, and
 *         Clean your pet to keep it awake. To clean your pet, put your hand to the ultrasonic sensor for 15 seconds.
 *         If you move your hand, the timer will reset. If total health reaches 0, your pet will fall asleep and you'll
 *         have to start over with a new, randomized pet (using RNG() to pick new pet name).
 *         The main display for the game will show total_health, which decreases every 15s. When your pet reaches less
 *         than 15% total health, !!! will display at the top right hand corner as a warning.

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *         Buzzer Song Imported from Robson Couto
 *         Github: robsoncouto

 *         PCD8544 Library included for Nokia 5110 LCD screen

 *         Demo Link: https://youtu.be/T5G_dGJC7R0

 */

#include <PCD8544.h>
PCD8544 lcd;

/* pet attributes and pet names array */
String petName;
const int numNames = 10;
const int max_name_length = 8;
const char petNames[numNames][max_name_length] = {"MAX", "CHARLIE", "THEO", "SCOUT", "OREO", "BENTLEY", "BOE", "OLLIE", "PEANUT", "DOZER"};


/* define pins and variables for ultrasonic sensor */
#define echoPin 8 // receives an echo from an object at some distance away
#define trigPin 9 // emits an ultrasound at 40kHz
long dur; // how long the pulse lasts
int dist; // distance away from the sensor the object is (in cms)

/* define pins for buttons and buzzers */

#define leftButton 10
#define rightButton 11
#define buzzer 2;

/* KEYBYOARD CAT SONG AND NOTES IMPORTED */
/* modifed to remove excess rest notes */
/* Credit: Robson Couto */
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

int melody[] = {

  // Keyboard cat
  // Score available at https://musescore.com/user/142788/scores/147371

    REST,
    REST,
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_E4, 
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_E4,
    NOTE_A3, NOTE_C4, NOTE_E4, NOTE_C4,
    NOTE_A3, NOTE_C4, NOTE_E4, NOTE_C4,
    NOTE_G3, NOTE_B3, NOTE_D4, NOTE_B3,
    NOTE_G3, NOTE_B3, NOTE_D4, NOTE_B3,

    NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3,
    NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3,
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_E4, 
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_E4,
    NOTE_A3, NOTE_C4, NOTE_E4, NOTE_C4,
    NOTE_A3, NOTE_C4, NOTE_E4, NOTE_C4,
    NOTE_G3, NOTE_B3, NOTE_D4, NOTE_B3,
    NOTE_G3, NOTE_B3, NOTE_D4, NOTE_B3,

    NOTE_G3 
  
};
/* end of imported code from Robson Couto */

const int FULL = 0;
const int MOOD = 1;
const int CLEANLINESS = 2;
double total_health = 0;;
int pet_attributes[3] = {5, 5, 5};

int cursX = 0;
int cursY = 0;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int(*TickFct)(int);
  } task;

const unsigned short tasksNum = 5;
task tasks[tasksNum];

enum SENSOR_States {SENSOR_Init, SENSOR_Read};
int SENSOR_Tick(int state) {
  switch(state) {
    case SENSOR_Init: state = SENSOR_Read; break;
    case SENSOR_Read: state = SENSOR_Read; break;
    default: break;
    }
  switch(state) {
    case SENSOR_Read:
      digitalWrite(trigPin, LOW);
      digitalWrite(trigPin, HIGH);
      digitalWrite(trigPin, LOW);
    
      dur = pulseIn(echoPin, HIGH);
      dist = dur * 0.034 / 2;
      Serial.print("Distance: ");
      Serial.print(dist);
      Serial.println(" cm");
      break;

      default: break;
    }

    return state;
  }

unsigned int j = 0;
enum BUZZER_States {BUZZER_Init, BUZZER_Play};
int BUZZER_Tick(int state) {
  switch(state) {
    case BUZZER_Init: 
    j = 0; 
    state = BUZZER_Play; 
    break;
    
    case BUZZER_Play: 
    state = BUZZER_Play; 
    if(j == 58) {state = BUZZER_Init;}
    break;
    
    default: break;
    }
    switch(state) {
    case BUZZER_Init:
    tone(buzzer, 0);
    break;
    
    case BUZZER_Play: 
    j++;
    tone(buzzer, melody[j]); 
    break;
    
    default: break;
    }
    return state;
  }

int game_req;
int game_ack;
enum IDLE_States {IDLE_Off, IDLE_WaitAck, IDLE_Playing};
int IDLE_Tick(int state) {
  switch(state) {
    case IDLE_Off:
    game_req = 0;
    if(digitalRead(leftButton)) {
      state = IDLE_WaitAck;
      lcd.clear();
      game_req = 1;
      } else {
        state = IDLE_Off;
        }
    break;

    case IDLE_WaitAck:
    if(!game_ack) {
      state = IDLE_WaitAck;
    } else if(game_ack) {
      game_req = 0;
      state = IDLE_Playing;
      }
    break;

    case IDLE_Playing:
    if(game_ack) {
      state = IDLE_Playing;
      } else if (!game_ack) {
          state = IDLE_Off;
          lcd.clear();
        }
    break;

    default: break;
    }
    
  switch(state) {
    case IDLE_Off:
      lcd.setCursor(0,0);
      lcd.print("Welcome to");
      lcd.setCursor(0,1);
      lcd.print("Pocket Pet!");
      cat1(10, 2);
      dog1(50, 2);
      lcd.setCursor(0,5);
      lcd.print("Left to Start!");
    break;

    default: break;
    }
    return state;
  }


int cat = 0;
int dog = 0;
// int disp;
double hold = 0;
int leftButtonHeld;
int rightButtonHeld;
unsigned int x = 0;
unsigned int y = 0;

int end_req;
int end_ack;
enum PLAYING_States {PLAYING_Idle, PLAYING_WaitR, PLAYING_Pick, PLAYING_Feed, PLAYING_Play, PLAYING_Clean_Prompt, PLAYING_Clean, PLAYING_Game, PLAYING_WaitE, PLAYING_WaitE2};
int PLAYING_Tick(int state) {
  switch(state) {
    case PLAYING_Idle:
      petName = "no name";
      game_ack = 0;
      end_req = 0;
      hold = 10;
      if(!game_req) {
        state = PLAYING_Idle;
        } else if (game_req) {
          state = PLAYING_WaitR;
          game_ack = 1;
          }
      leftButtonHeld = 0;
      rightButtonHeld = 0;
      //disp = -1;
    break;

    case PLAYING_WaitR:
      if(game_req) {
        state = PLAYING_WaitR;
        } else if(!game_req) {
          state = PLAYING_Pick;
          }
    break;

    case PLAYING_Pick:
      if(digitalRead(leftButton) && !leftButtonHeld && !rightButtonHeld) {
        leftButtonHeld = 1;
        cat = 1;
        } else {
          leftButtonHeld = 0;
          }
        
        if (digitalRead(rightButton) && !rightButtonHeld && !leftButtonHeld) {
          rightButtonHeld = 1;
          dog = 1;
          } else {
            rightButtonHeld = 0;
            }
        if (digitalRead(leftButton) || digitalRead(rightButton)) {
          lcd.clear();
          state = PLAYING_Game;
          randomizePet();
          leftButtonHeld = 0;
          rightButtonHeld = 0;
          } else {
            state = PLAYING_Pick;
            }
    break;

    case PLAYING_Game:
    x++;
      total_health = ((pet_attributes[FULL] + pet_attributes[MOOD] + pet_attributes[CLEANLINESS]) / 3.0 * 10.0);
      if(total_health > 0 && !end_req) {
        lcd.setCursor(0,cursY);
        if(digitalRead(leftButton) && !leftButtonHeld) {
          lcd.print(" ");
          if(cursY == 3) {cursY = 0;} else {
            cursY++;
            }
          leftButtonHeld = 1;
          } else {leftButtonHeld = 0; lcd.print("-");}
        if(digitalRead(rightButton) && !rightButtonHeld) {
          lcd.clear();
          if(cursY == 0) {
            state = PLAYING_Feed;
            pet_attributes[FULL]++;
            if(pet_attributes[MOOD] > 7) {
                pet_attributes[FULL]++;
            } else if (pet_attributes[FULL] < 3) {
                pet_attributes[FULL]++;
            }
            rightButtonHeld = 0;
            } else if (cursY == 1) {
              state = PLAYING_Play;
              pet_attributes[MOOD]++;
              rightButtonHeld = 0;
              } else if (cursY == 2) {
                hold = 10;
                state = PLAYING_Clean_Prompt;
                rightButtonHeld = 0;
                } else if(cursY == 3) {
                  end_req = 1;
                  state = PLAYING_WaitE;
                  rightButtonHeld = 0;
                  } else {state = PLAYING_Play;}
            cursX = 0;
            cursY = 0;
          } else {rightButtonHeld = 0;}
        }

      if(total_health <= 0) {
        end_req = 1; 
        lcd.clear(); 
        state = PLAYING_WaitE;
        }
      // every 30 ticks (15s) degenerate health by 1
      if(!(x % 30)) { // modified for demo video
        pet_attributes[FULL] -= 1;
        pet_attributes[MOOD] -= 1;
        pet_attributes[CLEANLINESS] -= 1;
        }
    break;

    case PLAYING_Feed:
    if(digitalRead(rightButton)) {
      lcd.clear();
      state = PLAYING_Game;
      } else {rightButtonHeld = 0;}
    break;

    case PLAYING_Play:
    if(digitalRead(rightButton)) {
      lcd.clear();
      state = PLAYING_Game;
      cursX = 0;
      } else {rightButtonHeld = 0;}
    break;

    case PLAYING_Clean_Prompt:
    // TODO
    y++;
    if(dist <= 5 && hold > 0) {
      if(y % 2) {hold --;}
      lcd.setCursor(0,0);
      lcd.print("Timer: ");
      lcd.print(hold);
      state = PLAYING_Clean_Prompt;
    } else if (hold == 0) {
      pet_attributes[CLEANLINESS]++;
      state = PLAYING_Clean;
      }
    else if(dist > 5) {
      state = PLAYING_Clean_Prompt;
      hold = 16;
      }
    break;
    
    case PLAYING_Clean:
    if(digitalRead(rightButton)) {
      lcd.clear();
      state = PLAYING_Game;
      cursX = 0;
      } else {rightButtonHeld = 0;}
    break;

    case PLAYING_WaitE:
    if(!end_ack) {
      state = PLAYING_WaitE;
      } else if(end_ack) {
        state = PLAYING_WaitE2;
        end_req = 0;
        }
    break;

    case PLAYING_WaitE2:
    if(end_ack) {
      state = PLAYING_WaitE2;
      } else if(!end_ack) {
        game_ack = 0;
        state = PLAYING_Idle;
        }
    break;

    default: break;
    }

 switch(state) {
    case PLAYING_Idle:
      cat = 0;
      dog = 0;
      for(int i = 0; i < 3; i++) {
        pet_attributes[i] = 5;
        }
        total_health = 50;
    break;

    case PLAYING_Pick:
      lcd.setCursor(0,0);
      lcd.print("Left     Right");
      lcd.setCursor(0,1);
      lcd.print("for      for");
      lcd.setCursor(0,2);
      lcd.print("cat      dog");
      cat1(10, 3);
      dog1(50, 3);
    break;

    case PLAYING_Feed:
    lcd.setCursor(0,0);
    lcd.print("FULL Status:");
    healthBar(pet_attributes[FULL], 0, 1);
    if(pet_attributes[FULL] <= 3) {
      lcd.setCursor(75, 0);
      lcd.print("!!");
      }
    if(cat) {cat1(30,3);} else if (dog) {dog1(30,3);}
    foodBowl(10, 4);
    break;

    case PLAYING_Play:
    lcd.setCursor(0,0);
    lcd.print("MOOD Status:");
    healthBar(pet_attributes[MOOD], 0, 1);
    if(pet_attributes[MOOD] <= 3) {
      lcd.setCursor(75, 0);
      lcd.print("!!");
      }
    if(cat) {cat1(30,3);} else if (dog) {dog1(30,3);}
    break;

    case PLAYING_Clean_Prompt:
    if(hold == 16) {
      lcd.setCursor(0,0);
      lcd.print("Use sensor to clean ");
      lcd.print(petName);
      lcd.setCursor(0,3);
      lcd.print("Wait for the timer to say 0!");
      } else if (hold == 15) {
        lcd.clear();
        }
    break;

    case PLAYING_Clean:
    lcd.setCursor(0,0);
    lcd.print("CLEAN Status:");
    healthBar(pet_attributes[CLEANLINESS], 0, 1);
    if(pet_attributes[CLEANLINESS] <= 3) {
      lcd.setCursor(75, 0);
      lcd.print("!!!");
      }
    if(cat) {cat1(30,3);} else if (dog) {dog1(30,3);}
    break;

    case PLAYING_Game:
      lcd.setCursor(5,0);
      lcd.print("> Feed");
      lcd.setCursor(5,1);
      lcd.print("> Play");
      lcd.setCursor(5,2);
      lcd.print("> Clean");
      lcd.setCursor(5,3);
      lcd.print("> Reset");
      lcd.setCursor(5,4);
      lcd.print(petName); 
      lcd.setCursor(5,5);
      lcd.print("health: "); 
      lcd.print(total_health);
      if(cat) {
        cat1(60, 2);
        }
      if(dog) {
        dog1(60, 2);
        }
      if(total_health <= 15) {
        lcd.setCursor(70,0);
        lcd.print("!!!");
        }
    break;

    default: break;
    }

    return state;
  }
  
unsigned int k = 0;
//int secs;
enum ENDGAME_States {ENDGAME_WaitR1, ENDGAME_WaitR2, ENDGAME_PO, ENDGAME_RESET};
int ENDGAME_Tick(int state) {
  switch(state) {
    case ENDGAME_WaitR1:
    end_ack = 0;
    if(!end_req) {
      state = ENDGAME_WaitR1;
      } else if (end_req) {
        state = ENDGAME_WaitR2;
        end_ack = 1;
        }
    break;
    
    case ENDGAME_WaitR2:
    if(end_req) {state = ENDGAME_WaitR2;}
    else if(!end_req) {
      k = 0;
      if(total_health <= 0) {
        state = ENDGAME_PO;
        lcd.clear();
        } else {state = ENDGAME_RESET; lcd.clear();}
      }
    break;

    case ENDGAME_PO:
    k++;
    if(k <= 15) {
      state = ENDGAME_PO;
      } else {
        lcd.clear();
        state = ENDGAME_WaitR1;
        end_ack = 0;
        }
    break;

    case ENDGAME_RESET:
    k++;
    if(k <= 10) {
      state = ENDGAME_RESET;
      } else {
        lcd.clear();
        state = ENDGAME_WaitR1;
        end_ack = 0;
        }
    break;

    default: break;
    }

  switch (state) {
    case ENDGAME_PO:
      lcd.setCursor(0,0);
      lcd.print("Oh no! ");
      lcd.print(petName);
      lcd.print(" reached 0 health! ");
      lcd.setCursor(0,4);
      lcd.print("Try again!");
    break;

    case ENDGAME_RESET:
    lcd.setCursor(0, 0);
    lcd.print("RESETTING");
    break;

    default: break;
    }
    return state;
  }


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(84,48);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  
  unsigned char i = 0;
  tasks[i].state = SENSOR_Init;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SENSOR_Tick;
  i++;
  tasks[i].state = BUZZER_Init;
  tasks[i].period = 250;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &BUZZER_Tick;
  i++;
  tasks[i].state = IDLE_Off;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &IDLE_Tick;
  i++;
  tasks[i].state = PLAYING_Idle;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &PLAYING_Tick;
  i++;
  tasks[i].state = ENDGAME_WaitR1;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &ENDGAME_Tick;
  
  tone(buzzer,0);
}

void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    // if(!(i%10)) {RNG();} // checks RNG
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
     }
   }
}
