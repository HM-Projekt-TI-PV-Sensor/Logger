#include <SD.h>
#include <RTClib.h>

#define PIN_SPI_CS 10

RTC_DS3231 rtc;
File logFile;
String fileName = "log.txt";
String lastInput = "";

void initRTC();
void initSD();
void createFile();
void askForCommand();
void processCommand();
void helpCommand();
void writeCommand(String* input);
void listCommand();
void purgeCommand();

void setup() {
  Serial.begin(9600);
  while (!Serial);
  initRTC();
  initSD();
  createFile();
}

void initRTC() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    return;
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("RTC started");
}

void initSD() {
  if (!SD.begin(PIN_SPI_CS)) {
    Serial.println(F("SD CARD FAILED!"));
    while (1);
  }

  Serial.println(F("SD card init"));
}

void createFile() {
  if (!SD.exists(fileName)) {
    Serial.println(fileName + " doesn't exist. Creating " + fileName + " file...");
    // create a new file by opening a new file and immediately close it
    logFile = SD.open(fileName, FILE_WRITE);
    logFile.close();
  }
}

void askForCommand() {
  Serial.println("Please enter a command. /help for help");
  while (Serial.available() == 0);
  lastInput = Serial.readString();
  processCommand();
}

void processCommand() {
  if (lastInput.length() == 0) {
    Serial.println("No input found.");
    return;
  }
  char first = lastInput.charAt(0);
  if (first != '/') {
    Serial.println("Missing '/' char");
    return;
  }
  if (lastInput.startsWith("/help")) {
    helpCommand();
  } else if (lastInput.startsWith("/write")) {
    if (lastInput.length() < 8 || lastInput.charAt(6) != ' ') {
      Serial.println("Noargs");
      return;
    }
    String param = lastInput.substring(7, lastInput.length() - 1);
    writeCommand(&param);
  } else if (lastInput.startsWith("/list")) {
    listCommand();
  } else if (lastInput.startsWith("/purge")) {
    purgeCommand();
  } else {
    Serial.println("Unkown command: " + lastInput);
  }
}

void helpCommand() {
  Serial.println("Commandlist:");
  Serial.println("/help         > Lists all available commands.");
  Serial.println("/write <data> > Writes data to the SD card.");
  Serial.println("/list         > Lists all written data.");
  Serial.println("/purge        > Deletes all stored data.");
}

void writeCommand(String *input) {
  Serial.print("Writing ");
  Serial.print(*input);
  Serial.println();
  logFile = SD.open(fileName, FILE_WRITE);
  logFile.println(*input);
  logFile.close();
}

void listCommand() {
  Serial.println("List of written data:");
  logFile = SD.open(fileName, FILE_READ);
  if (logFile.available() == 0) {
    Serial.println("File is empty.");
  } else {
    int last = logFile.available();
    Serial.print("Reading ");
    Serial.print(last);
    Serial.println(" bytes...");
    String line = "------------------";
    Serial.println(line);
    for (int i = 0; i < last; i++) {
      logFile.seek(i);
      Serial.print((char) logFile.peek());
    }
    Serial.println(line);
  }
  logFile.close();
}

void purgeCommand() {
  logFile = SD.open(fileName, FILE_READ);
  int size = logFile.available();
  logFile.close();
  Serial.print("Purging bytes: ");
  Serial.print(size);
  Serial.println();
  SD.remove(fileName);
  logFile = SD.open(fileName, FILE_WRITE);
  logFile.close();
}

void loop() {
  askForCommand();
}
