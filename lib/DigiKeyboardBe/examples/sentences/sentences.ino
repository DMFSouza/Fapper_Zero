#include "DigiKeyboardFr.h"

// some rendom sentences
char* sentences[9] = {
  "Hello world!",
  "Bonjour le monde!",
  "Hola mundo!",
  "It is a beautiful day!",
  "C'est un jour magnifique!",
  "Estas un dia hermoso!",
  "How are you?",
  "Comment allez-vous?",
  "Como estas?"
};

void setup() {
  DigiKeyboardFr.delay(5000); //wait for cmputer to recognize keyboard
}

void loop() {
  DigiKeyboardFr.print("Let's write some sentences!");
  DigiKeyboardFr.delay(2000);
  for (int i = 0; i < 27; i++) {
    DigiKeyboardFr.sendKeyStroke(KEY_BACKSPACE);
  }
  // for each sentence
  for (int i = 0; i < 9; i++) {
    // print sentence
    DigiKeyboardFr.print(sentences[i]);
    
    // wait for the people to read it
    DigiKeyboardFr.delay(2000);
    
    // delete it
    for (int j = 0; j < strlen(sentences[i]); j++) {
      DigiKeyboardFr.sendKeyStroke(KEY_BACKSPACE);
    }
    
    //wait
    DigiKeyboardFr.delay(1000);
  }
  DigiKeyboardFr.print("Done !");
  DigiKeyboardFr.delay(2000);
  for (int i = 0; i < 6; i++) {
    DigiKeyboardFr.sendKeyStroke(KEY_BACKSPACE);
  }
}