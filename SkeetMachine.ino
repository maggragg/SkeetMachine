
//Breytur fyrir pinnana, rás 1 - 4 o.f.l
const int ch_A = 11; //mark
const int ch_B = 10; //turn
const int ch_C = 9;  //double
const int ch_D = 8;  //Unused

const int ledPin = 0; //prufuljós

//relay pinnar
const int relay_1 = 13; //Motor á mark
const int relay_2 = 3; //Motor á turni
const int relay_3 = 4; //Ljos á marki
const int relay_4 = 5; //Ljos á turni

//skynjarar á vélum
const int sensor_1 = 6; //skynjari á mark
const int sensor_2 = 7; //skynjari á turni

//Stillingar á pinnum við ræsingu
int buttonStateA = 0;
int buttonStateB = 0;
int buttonStateC = 0;
int lastButtonStateA = 0;
int lastButtonStateB = 0;
int lastButtonStateC = 0;
unsigned long time = 0;
unsigned long s1time= 0;
unsigned long s2time= 0;
unsigned long randDelay = 0;
int mark = 0;
int sensor1 = 1;
int sensor2 = 1;

void setup() {
  
  randomSeed(analogRead(0));
  
  //rásirnar skilgreindar sem input frá fjarstýringu
  pinMode(ch_A, INPUT);
  pinMode(ch_B, INPUT);
  pinMode(ch_C, INPUT);
  pinMode(ch_D, INPUT);
  
  //stilla prufuljósið
  pinMode(ledPin, OUTPUT);
  
  //relay pinnarnir skilgreindir sem output
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
 
  //skynjarapinnar skilgreindir sem input
  pinMode(sensor_1, INPUT); // skynjari á marki
  pinMode(sensor_1, INPUT); // skynjari á turni
  
  Serial.begin(9600);
  
}
  
void loop() {
  
 buttonStateA = digitalRead(ch_A);
 buttonStateB = digitalRead(ch_B);
 buttonStateC = digitalRead(ch_C);
 
   if((buttonStateA != lastButtonStateA) && (sensor1 != 0) && (mark == 0)) {
     if(buttonStateA == HIGH) {
       //ef merki kemur inn á ch1 þá senda mark 1
       Serial.println("Button pushed for Mark");
       mark = 1;
       
     } 
    
   }
   
   if((buttonStateB != lastButtonStateB) && (sensor2 != 0) && (mark == 0)) {
     if(buttonStateB == HIGH) {
       //ef merki kemur inn á ch2 þá senda mark 2
       Serial.println("Button pushed for Tower!");
       Serial.println(millis());
       mark = 2;
     } 
   }
   
   if((buttonStateC != lastButtonStateC) && ((sensor1 != 0) || (sensor2 != 0)) && (mark == 0)){
     if(buttonStateC == HIGH) {
       //ef merki kemur inn á ch3 þá senda mark 2
       Serial.println("Button pushed for double!");
       mark = 3;
     } 
   }
   
   //Ef smellt hefur verið á fjarstýringu keyrir þetta random timer af stað og kveikir á viðeigandi ljósi
   if ( mark != 0 ) {
     if (randDelay == 0) {
       randDelay = random(3000);
       time = millis();
       if(mark == 1) {
         digitalWrite(relay_3, HIGH);
       }
       else if (mark == 2) {
         digitalWrite(relay_4, HIGH);
       }
       else if (mark == 3) {
         digitalWrite(relay_3, HIGH);
         digitalWrite(relay_4, HIGH);
       }
         
       Serial.print("Delay in ms: ");
       Serial.println(randDelay);
       
     }
     
     //þegar timerinn er liðinn þá slekkur þetta á ljósi og skýtur dúfu af stað
     if(((time + randDelay) <= millis()) && mark != 0 ) {
       if(mark == 1){
         digitalWrite(relay_1, HIGH);
         Serial.println("CLAY GOES OFF!! mark");
         digitalWrite(relay_3, LOW);
         sensor1 = 0;
         s1time = millis();
       }
       else if (mark == 2) {
         digitalWrite(relay_2, HIGH);
         Serial.println("CLAY GOES OFF!! tower");
         Serial.println(millis());
         digitalWrite(relay_4, LOW);
         sensor2 = 0;
         s2time = millis();
       }
       else if (mark == 3) {
         digitalWrite(relay_1, HIGH);
         Serial.println("CLAY GOES OFF!! mark");
         digitalWrite(relay_2, HIGH);
         Serial.println("CLAY GOES OFF!! tower");
         digitalWrite(relay_3, LOW);
         digitalWrite(relay_4, LOW);
         sensor1 = sensor2 = 0;
         s1time = s2time = millis();
       }

       digitalWrite(ledPin, LOW);
       Serial.print("Delay betwen timer and real time: ");
       Serial.println((millis()) - (time + randDelay));
       mark = 0;
       randDelay = 0;
       
     }
   }
   
   if ( ((s1time + 4000) < millis()) && (sensor1 == 0)) {
     Serial.println("Mark ready again!");
     digitalWrite(relay_1, LOW);
     sensor1 = 1;
   }
   
   if ( ((s2time + 4000) < millis()) && (sensor2 == 0)) {
     Serial.println("Tower ready again!");
     digitalWrite(relay_2, LOW);
     sensor2 = 1;
   }
     
   
   lastButtonStateA = buttonStateA;
   lastButtonStateB = buttonStateB;
   lastButtonStateC = buttonStateC;
   
  
}
