// SCR precharge code for attiny
// 210711
// change digitalRead and digitalWrites to direct register access in final code revision

#define precharge 4     // precharge input pin
#define bus_run   3     // bus_run input pin
#define SCR_1     0     // SCR 1 output pin
#define SCR_2     1     // SCR 2 output pin
#define timeout   123   // max TCNT1 value before it shuts off PWM outputs, this should kick in right before the next 60Hz half cycle
#define precharge_speed 3000 // adjusts precharge speed, larger = longer time, slower precharge

byte ZC = 0;            // zero crossing status
byte charge_status = 0; // 0 = off, 1 = charging, 2 = run, 3 = precharge -> run
bool bus_run_condition = 0;
bool precharge_condition = 0;
int alpha = 0;          // delay iterator

void setup(void) {
  pinMode(precharge, INPUT_PULLUP);  // precharge, active high
  pinMode(bus_run,   INPUT_PULLUP);  // run, active high
  pinMode(SCR_1, OUTPUT);    // SCR trigger 1, these probably don't need to be defined, but i'm just making sure
  pinMode(SCR_2, OUTPUT);    // SCR trigger 2
  digitalWrite(SCR_1, LOW);  // ensure digital outputs are LOW
  digitalWrite(SCR_2, LOW);  // ensure digital outputs are LOW
  
  // SCR pulsing PWM configuration
  OCR0A = 80;           // set PWM1 duty cycle (about 50µs pulse, pin 9)
  OCR0B = 80;           // set PWM2 duty cycle (about 50µs pulse, pin 10)
  TCCR0B = 0b00000001;  // set Timer1 clock to CLKio/8 (get PWM frequency of 3.9375 kHz)
  TCCR0A =  0;          // ensure SCR outputs are off
  
  // counter configuration
  TCCR1 = 0b00001010;   // CLKio/512 prescaler
  OCR1A = timeout-2;    // delay before turning on SCRs for precharge
  OCR1B = timeout;      // shut off SCRs, this is about how long a 60Hz half cycle period is, minus a slight margin
  GTCCR = 0;            // make sure everything here is disabled!!!!!!!!!!!!!!!!!
  TIMSK = 0b00100000;   // enable OCM B INT, B acts as a timeout and shuts off SCRs

  attachInterrupt(0, ZC_detect, CHANGE);  // enable INT0 for ZCD
}

void ZC_detect() { // ISR to handle zero crossing event  
  TCCR0A =  0; // turn off SCRs, though they should be off at this point already due to the timeout ISR
  
  if(digitalRead(2)) ZC = 1; // determines polarity of AC cycle
  else ZC = 2;

  TCNT0 = 0; // this reduces overal jitter of the SCR PWM
  TCNT1 = 0; // reset counter

  if(charge_status == 2) { // having this here reduces some jitter when PWM first starts in the cycle
    TCCR0A = (ZC == 1) ? 0x81 : 0x21; // enable SCRs based on AC polarity
    ZC = 0; // this for loop doesn't need to be accessed till the next 60Hz half cycle
  }
}

void loop() {
  bus_run_condition = !digitalRead(bus_run);
  precharge_condition = !digitalRead(precharge);

//  if(bus_run_condition && !precharge_condition) { // active low, if bus_run and not precharging // add && precharge_done maybe for soft transistion to run
//  if(bus_run_condition && precharge_condition && (charge_status == 1)) { // To run, UD3 enables both of these. Check that it came from precharge status.
  if(bus_run_condition && precharge_condition) {
    if(charge_status != 2) {
      charge_status = 2; // it is now in run mode
      TIMSK = TIMSK & 0b10111111; // unenable OCM A INT
    }
  }
    
  else if(!bus_run_condition && precharge_condition) { // precharging
    if(charge_status == 0) {
      charge_status = 1;    // it is now in charging mode
      OCR1A = timeout-2;    // reset charging progress
      alpha = 0;            // reset delay iterator
    }
    
    if(charge_status == 1) {
      TIMSK = TIMSK | 0b01000000; // enable OCM A INT

      if(alpha >= precharge_speed) {  // decrement OCR0A at precharge speed rate
        OCR1A -= 1;                   // turn on SCR earlier
        if(OCR1A <= 60) OCR1A = 60;   // limit earliest turn on to about 97 degrees (only needs to go to 90 degrees for max voltage)
        alpha = 0;                    // reset delay iterator
      }
      else alpha++;                   // increment delay iterator
    }
  }

  else { // neither, shut everything off                                          if(!bus_run_condition && !precharge_condition) {
    charge_status = 0;                // not trying to charge or run
    TIMSK = TIMSK & 0b10111111;       // unenable OCM A INT

//    TCCR1A =  0; // turn off everything. enabling this immediately turns off the output, disabling turns off the output on the next ZCD
  }
}

ISR(TIMER1_COMPA_vect) { // timer 1 interrupt
  if(TCNT1 > OCR1A) { // this prevents the initial full pulse before precharging, which is bad
    TCCR0A = ((ZC == 1) && (ZC != 0)) ? 0x81 : 0x21; // enable SCRs based on AC polarity  ********************** check for ZC = 2 maybe? <<kinda done
  }
}

ISR(TIMER1_COMPB_vect) { // timer 1 interrupt
  TCCR0A =  0; // timeout, shut SCRs off before start of next 60Hz half cycle
}
