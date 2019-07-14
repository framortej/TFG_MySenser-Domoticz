int relee = 10;
//int boton = 8;

void setup () {
  pinMode(relee, OUTPUT); //LED 13 como salida
  //pinMode(boton, INPUT); //LED 13 como salida
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(relee, LOW);
  Serial.begin(9600); //Inicializo el puerto serial a 9600 baudios
}

void loop () {
  if (Serial.available()) { //Si está disponible
    char c = Serial.read(); //Guardamos la lectura en una variable char
    if (c == 'H') { //Si es una 'H', enciendo el LED
      Serial.println(c);
      digitalWrite(relee, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
    } else if (c == 'L') { //Si es una 'L', apago el LED
      Serial.println(c);
      digitalWrite(relee, LOW);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  // while(analogRead(A0)> 290){
  // digitalWrite(relee, !digitalRead(relee));
}
