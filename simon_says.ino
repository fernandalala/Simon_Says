#include <Arduino.h>

const int LED_PINS[] = {26, 27, 14, 12}; 
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

const int BUTTON_PINS[] = {13, 32, 33, 25}; 
const int NUM_BUTTONS = sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]);

const int BUZZER_PIN = 15;

int gameSequence[100]; 
int sequenceLength = 1;
int playerTurnIndex = 0;
bool gameActive = false;
bool gameOver = false;
unsigned long lastButtonPressTime = 0;
const long DEBOUNCE_DELAY = 50; 

const int TONE_FREQS[] = {262, 330, 392, 440}; 

void playTone(int freq, int duration) {
    tone(BUZZER_PIN, freq, duration);
    delay(duration); 
    noTone(BUZZER_PIN);
}

void generateNextStep() {
    gameSequence[sequenceLength - 1] = random(NUM_LEDS);
    Serial.print("Sequence generated (LED indices): ");
    for (int i = 0; i < sequenceLength; i++) {
        Serial.print(gameSequence[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void playSequence() {
    Serial.println("Playing sequence...");
    delay(1000);

    for (int i = 0; i < sequenceLength; i++) {
        int currentLED = gameSequence[i];
        Serial.print("Flashing LED: ");
        Serial.println(currentLED);

        digitalWrite(LED_PINS[currentLED], HIGH);
        playTone(TONE_FREQS[currentLED], 300);
        digitalWrite(LED_PINS[currentLED], LOW);

        delay(300); 
    }
    playerTurnIndex = 0;
    Serial.println("Your turn! Enter the sequence...");
}


void startGame() {
    Serial.println("\n--- Game Started! ---");
    Serial.println("Level: 1");
    Serial.println("Watch the sequence...");
    gameActive = true;
    gameOver = false;
    sequenceLength = 1;
    playerTurnIndex = 0;
    generateNextStep();
    playSequence();
}

void resetGame() {
    Serial.println("\n--- Game Reset! ---");
    Serial.println("Simon Says!");
    Serial.println("Press any button to start the game.");
    gameOver = false;
    gameActive = false;
    sequenceLength = 1;
    playerTurnIndex = 0;
    for (int i = 0; i < NUM_LEDS; i++) { 
        digitalWrite(LED_PINS[i], LOW);
    }
    noTone(BUZZER_PIN); 
}

void advanceGame() {
    sequenceLength++;
    Serial.print("\n--- Advancing to Level: ");
    Serial.print(sequenceLength);
    Serial.println(" ---");

    generateNextStep();
    playSequence(); 
}

void endGame() {
    Serial.println("\n--- Game Over! ---");
    gameOver = true;
    gameActive = false;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < NUM_LEDS; j++) {
            digitalWrite(LED_PINS[j], HIGH);
        }
        playTone(100, 200);
        for (int j = 0; j < NUM_LEDS; j++) {
            digitalWrite(LED_PINS[j], LOW);
        }
        delay(200);
    }

    Serial.print("Final Score: ");
    Serial.println(sequenceLength - 1);
    delay(2000);

    Serial.println("Press any button to restart...");
}

void handlePlayerInput() {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (digitalRead(BUTTON_PINS[i]) == HIGH) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                lastButtonPressTime = millis();
                int pressedButton = i;

                Serial.print("Player pressed: ");
                Serial.println(pressedButton);

                digitalWrite(LED_PINS[pressedButton], HIGH);
                playTone(TONE_FREQS[pressedButton], 200);
                digitalWrite(LED_PINS[pressedButton], LOW);

                if (pressedButton == gameSequence[playerTurnIndex]) {
                    playerTurnIndex++;
                    Serial.print("Correct! Current index: ");
                    Serial.println(playerTurnIndex);
                } else {
                    Serial.println("Wrong button! Game Over.");
                    endGame();
                    return; 
                }
            }
        }
    }
}

void setup() {
    Serial.begin(460800);
    Serial.println("--- Simon Says Game ---");
    Serial.println("Connect to Serial Monitor at 115200 baud.");
    Serial.println("-----------------------");
    randomSeed(analogRead(0));

    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(LED_PINS[i], OUTPUT);
        digitalWrite(LED_PINS[i], LOW);
    }

    for (int i = 0; i < NUM_BUTTONS; i++) {
        pinMode(BUTTON_PINS[i], INPUT_PULLDOWN);
    }

    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println("\nSimon Says!");
    Serial.println("Press any button to start the game.");
}

void loop() {
    if (!gameActive && !gameOver) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (digitalRead(BUTTON_PINS[i]) == HIGH) {
                if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                    lastButtonPressTime = millis();
                    startGame();
                    break;
                }
            }
        }
    } else if (gameActive && !gameOver) {
        if (playerTurnIndex == sequenceLength) {
            delay(500);
            advanceGame();
        } else {
            handlePlayerInput();
        }
    } else if (gameOver) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (digitalRead(BUTTON_PINS[i]) == LOW) {
                if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                    lastButtonPressTime = millis();
                    resetGame();
                    break;
                }
            }
        }
    }
}