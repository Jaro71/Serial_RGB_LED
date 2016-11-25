#define red_led 9    //Zdefiniowane numery pinów
#define green_led 10
#define blue_led 11

void setup() //Main
{
  pinMode(red_led, OUTPUT); //Definiujemy czy nasze piny mają być wejściem, czy wyjściem
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  analogWrite(red_led, 128); //Przypisuje pinowi warto�� PWM (Modulacji szeroko�ci impulsów)
  analogWrite(green_led, 0); //Generuje sygnał kwadratowy
  analogWrite(blue_led, 128);
  Serial.begin(9600); //Transmisja szeregowa w bitach na sekundę
}

void loop()
{
  if (Serial.available()){ //Sprawdza, czy mo�na odczyta� dane z portu szeregowego, 
    char led_specifier = Serial.read(); //Odczyt danych z bufora portu szeregowego
    int led_brightness = Serial.parseInt(); //Umożliwia odczytanie wartości całkowitej podanej przez kontroler
    write_leds(led_specifier, led_brightness);
  }
}

void write_leds(char led, int brightness)
{
  if (led == 'r'){
    analogWrite(red_led, 255 - brightness);
    return;
  }
  if (led == 'g'){
    analogWrite(green_led, 255 - brightness);
    return;
  }
  if (led == 'b'){
    analogWrite(blue_led, 255 - brightness);
    return;
  }
  
  return;
}

