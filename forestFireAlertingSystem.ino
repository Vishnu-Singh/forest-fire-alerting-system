int gas_pin=A2;
int x_pin=A0;
int lm35_pin=A1;
int buzzer=8;
int pump=7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(gas_pin,INPUT);
  pinMode(x_pin,INPUT);
  pinMode(lm35_pin,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(pump,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  calculateTemp();
  gas();
  tilt();
  //delay(1000);
}


void calculateTemp(){
    int val=analogRead(lm35_pin);  
    int t=val*0.48828125;
    if(t>35.0){
      digitalWrite(pump,HIGH);
      digitalWrite(buzzer,HIGH);
    }else{
      digitalWrite(pump,LOW);
      digitalWrite(buzzer,LOW);
    }
    Serial.print("temp:");
    Serial.print(t);
    Serial.print(";");
}


void gas(){
  int val=analogRead(gas_pin);
  Serial.print("co2:");
  Serial.print(val);
  Serial.print(";");
}

void tilt(){
  int x_value=analogRead(x_pin);
  if(x_value<260){
    digitalWrite(buzzer,HIGH);
  }else{
    digitalWrite(buzzer,LOW);
  }
  Serial.print("tilt:");
  Serial.print(x_value);
  Serial.println(";");
}
