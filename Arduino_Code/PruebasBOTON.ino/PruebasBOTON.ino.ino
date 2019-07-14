int p1 = 10;
int p2 = 8;

void setup () {
  pinMode(p1, OUTPUT); //LED 13 como salida
  pinMode(p2, OUTPUT); //LED 13 como salida
  digitalWrite(p1, LOW);
  digitalWrite(p2, HIGH);
}

void loop () {
}
