#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define IN1  5
#define IN2  16
#define IN3  0
#define IN4  4
#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0
#define ENA 12
#define ENB 13


//WiFi Status LED
#define wifiLed    2   //D4

int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; //Define integer to remember the toggle state for relay 2


// Update these with values sui able for your network.

const char* ssid = ""; //WiFI Name 
#define password "" //WiFi Password

const char* mqttServer = "*link of mqtt server*";
const char* mqttUserName = ""; // MQTT username
const char* mqttPwd = ""; // MQTT password
const char* clientID = "Esp1"; // client id

#define sub1 "TopicTien"
#define sub2 "TopicLui"
#define sub3 "TopicTrai"
#define sub4 "TopicPhai"

//#define pub1 "Rabbit"
//#define pub2 "Cat"


WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (80)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
 delay(10);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}

void reconnect() {
 while (!client.connected()) {
 if (client.connect(clientID, mqttUserName, mqttPwd)) {
 Serial.println("MQTT connected");
      // ... and resubscribe
      client.subscribe(sub1);
      client.subscribe(sub2);
      client.subscribe(sub3);
      client.subscribe(sub4);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if (strstr(topic, sub1))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      tien(MAX_SPEED);
      toggleState_1 = 0;
      
    }   
  }

  else if (strstr(topic, sub2))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      lui(MAX_SPEED);
      toggleState_1 = 0;
      
    }   
  }

  else if (strstr(topic, sub3))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      queotrai(MAX_SPEED);
      toggleState_1 = 0;
      
    }   
  }

  else if (strstr(topic, sub4))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      queophai(MAX_SPEED);
      toggleState_1 = 0;
      
    }   
  }
  
  else
  {
    Serial.println("unsubscribed topic");
  }
}



void setup() {
  Serial.begin(115200);
    
  motor_1_dung();
  motor_2_dung();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF

  
  //During Starting WiFi LED should TURN OFF
 digitalWrite(wifiLed, HIGH);

  setup_wifi();
   client.setServer(mqttServer, 1883);
   client.setCallback(callback);
}

void motor_1_dung() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
 
void motor_2_dung() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
 
void motor_1_tien(int speed) { //speed: từ 0 - MAX_SPEED
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(IN1, HIGH);// chân này không có PWM
  digitalWrite(IN2, LOW);
  analogWrite(ENA,170);
 
}
 
void motor_1_lui(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(IN1, LOW);// chân này không có PWM
  digitalWrite(IN2, HIGH);
  analogWrite(ENA,170);
  
}
 
void motor_2_tien(int speed) { //speed: từ 0 - MAX_SPEED
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(IN3, HIGH);// chân này không có PWM
  digitalWrite(IN4, LOW);
  analogWrite(ENB,170);
  
}
 
void motor_2_lui(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(IN3, LOW);// chân này không có PWM
  digitalWrite(IN4, HIGH);
  analogWrite(ENB,170);
  
}

void tien(int speed){
  motor_1_tien(speed);
  motor_2_tien(speed);
  delay(500);
  motor_1_dung();
  motor_2_dung();
}

void lui(int speed){
  motor_1_lui(speed);
  motor_2_lui(speed);
  delay(500);
  motor_1_dung();
  motor_2_dung();
}

void queotrai(int speed){
  motor_1_tien(speed);
  motor_2_lui(speed);
  delay(100);
  motor_1_dung();
  motor_2_dung();
  }

void queophai(int speed){
  motor_1_lui(speed);
  motor_2_tien(speed);
  delay(100);
  motor_1_dung();
  motor_2_dung();
  }
  
void loop() {
  
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  else{
    digitalWrite(wifiLed, LOW);
    
  }
  client.loop();
}
