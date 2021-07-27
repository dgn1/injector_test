// injector test 
// for playing around with the MR2 injectors (seems no fuel -- so control directly). 
//
// by Mike 2021
//
// MIT license.
//
// inputs: 
//        a0 -- pot -- delay (relevant to 10 signal)
//        a2 -- pot -- period of firing (relevant to 10 signal)
//       d21 -- digital input -- ecu pin 10 signal
//       d20 -- digital input -- ecu pin 20 signal
//
// outputs:
//        d2 - digital output - cylinders 1,2
//        d3 - digital output - cylinders 3,4
//
// 
//
int onetwo = 2;                                 // pin for digital output to activate injectors for cylinders 1 and 2
int threefour = 4;                              // pin for digital output to activate injectors for cylinders 3 and 4
int ecu_10_pin = 21;                            // ecu trigger input (phase 0 signal -- used as standard)
int ecu_20_pin = 20;                            // ecu trigger input (the other phase 0 signal -- used as standard)
int sensorPin1 = A0;                            // input for delay 
int sensorPin2 = A2;                            // input for duration
int phase_base = 0;                             // phase delay basis (from adc)
int period_base = 0;                            // period duration basis (from adc)
int adc_max = 1023;                             // adc max 
int ms_per_dial = 100;                          // scaling factor (used for both phase and period)
float phase = 0;                                // calculated phase (lag)
float period = 0;                               // calculate period (T)
unsigned long previousMillis = 0;               // for leading and lagging
int ecu_10 = 0;                                 // signal 10 value 
int ecu_20 = 0;                                 // signal 20 value
unsigned long currentMillis = 0;                // current time stamp
unsigned long last = 0;                         // last time stamp

  
void setup() {                                  // set up the pins and provide diag output
  pinMode(ecu_10_pin, INPUT_PULLUP);            // input from #10 line output of ECU
  pinMode(ecu_20_pin, INPUT_PULLUP);            // input from #20 line output of ECU
  pinMode(onetwo, OUTPUT);                      // cylinders 1 and 2 (tied together in factory schematics)
  pinMode(threefour, OUTPUT);                   // cylinders 3 and 4 (tied together in factory schematics)
  digitalWrite(onetwo, HIGH);                   // default high (inactive)
  digitalWrite(threefour, HIGH);                // default high (inactive)
  // Serial.begin(9600);                        // if you want diagnostic messages on serial console, use this (uncomment this and subsequent serial print commands)
}

void loop() {
  currentMillis = millis();                     //  get time
  ecu_10 = digitalRead(ecu_10_pin);             // get #10
  ecu_20 = digitalRead(ecu_20_pin);             // get #20
  phase_base = analogRead(sensorPin1);          // get pot pos for phase
  period_base = analogRead(sensorPin2);         // get pot pos for period
  // Serial.print("phi_b = ");
  // Serial.print(phase_base);
  // Serial.print("\tT = \t");
  // Serial.println(period_base);
  phase = (float(phase_base)/float(adc_max))*float(ms_per_dial);            // calculate phase
  // Serial.print("\tphi = \t");
  // Serial.println(phase);
  period = (float(period_base)/float(adc_max))*float(ms_per_dial);          // calculate duration 
  //
  //if ( ecu_10 == HIGH ) {               // low-activated; do nothing      <-- commented out as not needed; likely good form to have, though
  //  digitalWrite(onetwo, HIGH);
  //}
  if ( ecu_10 == LOW ) {
    last = millis();
    // trigger low; with appropirate pulse and delay as set by pots
    while ( currentMillis - last < int(phase) )
      {
      //Serial.print("c = ");
      //Serial.print(currentMillis);
      //Serial.print("\tl = ");
      //Serial.print(last);
      //Serial.print("\tphi = \t");
      //Serial.println(int(phase));
      currentMillis = millis();
      }
    //Serial.println("escaped!");
    digitalWrite(onetwo, LOW);
    delay(int(period));                 // keep active as long as required...
    digitalWrite(onetwo, HIGH);         // reset the state
    Serial.print("10 : phi = ");
    Serial.print(int(phase));
    Serial.print("\tT = ");
    Serial.println(int(period));
    //Serial.println(" ms");
  }
  //if ( ecu_20 == HIGH ) {               // low-activated; do nothing   <-- commented out as not needed; likely good form to have, though
  //  digitalWrite(threefour, HIGH);
  //}
  if ( ecu_20 == LOW ) {
    // trigger low; with appropirate pulse and delay as set by pots 
    delay(int(phase));                  // simple way...
    digitalWrite(threefour, LOW);     
    delay(int(period));
    digitalWrite(threefour, HIGH);         // reset the state
    Serial.print("\t\t\t\t\t\t\t\t20 : phi = ");
    Serial.print(int(phase));
    Serial.print("\tT = ");
    Serial.println(int(period));
  }




}
