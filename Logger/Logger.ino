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
String readableTimestamp();
uint32_t unixTimestamp();
void askForCommand();
void processCommand();
void helpCommand();
void writeCommandT(String* input);
void writeCommandU(String* input);
void listCommand();
void purgeCommand();
void timeCommand();

void setup() {
  Serial.begin(9600);
  while (!Serial);
  initRTC();
  initSD();
  createFile();
}

void initRTC() {
  if (! rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    return;
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println(F("RTC started"));
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
    Serial.print(fileName);
    Serial.print(F(" doesn't exist. Creating "));
    Serial.print(fileName);
    Serial.print(F(" file..."));
    Serial.println();
    logFile = SD.open(fileName, FILE_WRITE);
    logFile.close();
  }
}

String readableTimestamp() {
  DateTime current = rtc.now();
  char buff[] = "DD-MM-YYYY hh:mm:ss";
  return String(current.toString(buff));
}

uint32_t unixTimestamp() {
  DateTime current = rtc.now();
  uint32_t stamp = current.unixtime();
  return stamp;
}

void askForCommand() {
  Serial.println(F("Please enter a command. /help for help"));
  while (Serial.available() == 0);
  lastInput = Serial.readString();
  processCommand();
}

void processCommand() {
  if (lastInput.length() == 0) {
    Serial.println(F("No input found."));
    return;
  }
  char first = lastInput.charAt(0);
  if (first != '/') {
    Serial.println(F("Missing '/' char"));
    return;
  }
  if (lastInput.startsWith("/help")) {
    helpCommand();
  } else if (lastInput.startsWith("/writeT")) {
    if (lastInput.length() < 9 || lastInput.charAt(7) != ' ') {
      Serial.println(F("To few arguments for /writeT <data>"));
      return;
    }
    String param = lastInput.substring(8, lastInput.length() - 1);
    writeCommandT(&param);
  } else if (lastInput.startsWith("/writeU")) {
    if (lastInput.length() < 9 || lastInput.charAt(7) != ' ') {
      Serial.println(F("To few arguments for /writeU <data>"));
      return;
    }
    String param = lastInput.substring(8, lastInput.length() - 1);
    writeCommandU(&param);
  } else if (lastInput.startsWith("/list")) {
    listCommand();
  } else if (lastInput.startsWith("/purge")) {
    purgeCommand();
  } else if (lastInput.startsWith("/time")) {
    timeCommand();
  } else {
    Serial.print(F("Unkown command: "));
    Serial.print(lastInput);
    Serial.println();
  }
}

void helpCommand() {
  Serial.println(F("Commandlist:"));
  Serial.println(F("/help           > Lists all available commands."));
  Serial.println(F("/writeT <data>  > Writes data to the SD card with timestamp."));
  Serial.println(F("/writeU <data>  > Writes data to the SD card with unix timestamp."));
  Serial.println(F("/list           > Lists all written data."));
  Serial.println(F("/purge          > Clears all data."));
  Serial.println(F("/time           > Shows RTC time."));
}

void writeCommandT(String *input) {
  Serial.print(F("Writing "));
  Serial.print(*input);
  Serial.println();
  logFile = SD.open(fileName, FILE_WRITE);
  logFile.print(readableTimestamp());
  logFile.print(F(" > "));
  logFile.print(*input);
  logFile.println();
  logFile.close();
}

void writeCommandU(String *input) {
  Serial.print(F("Writing "));
  Serial.print(*input);
  Serial.println();
  logFile = SD.open(fileName, FILE_WRITE);
  logFile.print(unixTimestamp());
  logFile.print(F(" > "));
  logFile.print(*input);
  logFile.println();
  logFile.close();
}

void listCommand() {
  Serial.println(F("List of written data:"));
  logFile = SD.open(fileName, FILE_READ);
  if (logFile.available() == 0) {
    Serial.println(F("File is empty."));
  } else {
    int last = logFile.available();
    Serial.print(F("Reading "));
    Serial.print(last);
    Serial.println(F(" bytes..."));
    String line = F("------------------");
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
  Serial.print(F("Purging bytes: "));
  Serial.print(size);
  Serial.println();
  SD.remove(fileName);
  logFile = SD.open(fileName, FILE_WRITE);
  logFile.close();
}

void timeCommand() {
  Serial.println(readableTimestamp());
}

void loop() {
  askForCommand();
}
