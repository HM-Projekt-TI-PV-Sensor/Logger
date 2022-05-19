
int eingang= A0; //Das Wort „eingang“ steht jetzt für den Wert „A0“ (Bezeichnung vom Analogport 0)
int LED = 10; //Das Wort „LED“ steht jetzt für den Wert 10
int sensorWert = 0; //Variable für den Sensorwert mit 0 als Startwert

void setup()//Hier beginnt das Setup.
{
Serial.begin(9600); //Die Kommunikation mit dem seriellen Port wird gestartet. Das benötigt man, um sich den tatsächlich ausgelesenen Wert später im serial monitor anzeigen zu lassen.
pinMode (LED, OUTPUT); //Der Pin mit der LED (Pin 10) ist jetzt ein Ausgang
//Der analoge Pin muss nicht definiert werden.
}

void loop()
{//Mit dieser Klammer wird der Loop-Teil geöffnet.
sensorWert =analogRead(eingang); //Die Spannung an dem Fotowiderstand auslesen und unter der Variable „sensorWert“ abspeichern.
Serial.print("Sensorwert = " ); //Ausgabe am Serial-Monitor: Das Wort „Sensorwert: „
Serial.println(sensorWert); //Ausgabe am Serial-Monitor. Mit dem Befehl Serial.print wird der Sensorwert des Fotowiderstandes in Form einer Zahl zwischen 0 und 1023 an den serial monitor gesendet.

if (sensorWert > 512 ) //Wenn der Sensorwert über 512 beträgt….
{
digitalWrite(LED, HIGH); //…soll die LED leuchten…
}

else //andernfalls…
{
digitalWrite(LED, LOW); //….soll sie nicht leuchten.
}

delay (50);//Eine kurze Pause, in der die LED an oder aus ist

}//Mit dieser letzten Klammer wird der Loop-Teil geschlossen.

//Wenn nun der Sensorwert bei normaler Helligkeit bspw. nur den Wert 100 hat 
//(Der Wert ist abhängig von den verwendeten Widerständen, von der 
//Helligkeit und von der Stromrichtung), dann nimmt man anstelle des Wertes 
//512 einen wesentlich kleineren Wert, bei dem die LED zu leuchten beginnen
//soll. Bspw. nimmt man dann den Wert 90. Den aktuellen Sensorwert kann 
//man sich nun mit Hilfe des „Serial monitor“ anzeigen lassen. Dazu klickt man 
//oben auf „Tools“ und anschließend auf „serial monitor“. 


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
