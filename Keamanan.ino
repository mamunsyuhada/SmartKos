void keamananOn() {
  /*Maling*/
  if (Firebase.getString("maling") == "true" ) {
    digitalWrite(alarm, HIGH);
  }
  else if (Firebase.getString("maling") == "false" ) {
    Firebase.setString("pintu", "aman");
    Firebase.setString("jendela", "aman");
    digitalWrite(alarm, LOW);
  }
  /*Pintu*/
  int pintu = digitalRead(sensorPintu);
  if (pintu == LOW) {
    Serial.println("Pintu ==> Ada penyusup, pintu kos dibuka PAKSA!!");
    Firebase.setString("pintu", "Ada penyusup, pintu kos dibuka PAKSA!!");
    Firebase.setString("maling", "true");
  }
  /*Jendela*/
  int jendela = digitalRead(sensorJendela);
  if (jendela == LOW) {
    Firebase.setString("jendela", "Ada penyusup, jendela kos dibuka PAKSA!!");
    Serial.println("Pintu ==> Ada penyusup, jendela kos dibuka PAKSA!!");
    Firebase.setString("maling", "true");
  }
}

