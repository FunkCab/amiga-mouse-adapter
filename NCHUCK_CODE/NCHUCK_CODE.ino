#include <C:\projects\amiga-mouse-adapter\NCHUCK_CODE\NewWiiChk.h>
NewWiiChk WiiChk;

// Output pins
#define P_OUT_V_PULSE   6 //Y1
#define P_OUT_H_PULSE   4 //X1
#define P_OUT_VQ_PULSE  7 //Y2
#define P_OUT_HQ_PULSE  5 //X2
#define P_OUT_LMB       8 //LMB
#define P_OUT_RMB       9 //RMB
#define P_OUT_MMB       10 //MMB
      int DLYX;
      int DLYY;
      int CENTREX;
      int CENTREY;
      int CENTRE_DBAND = 10; // PREVENT "JOYCON DRIFT"
      int TRIGGER = 0;
      int TRIGGER2 = 0;


void setup() 
{
      WiiChk.init();
      pinMode(P_OUT_V_PULSE,  OUTPUT);
      pinMode(P_OUT_H_PULSE,  OUTPUT);
      pinMode(P_OUT_VQ_PULSE, OUTPUT);
      pinMode(P_OUT_HQ_PULSE, OUTPUT);
      pinMode(P_OUT_LMB,      OUTPUT);


      WiiChk.get_data(); // Load the data from  the WiiChuck

      WiiChk.print_data();
      CENTREX = WiiChk.joyx();
      CENTREY = WiiChk.joyy();
     Serial.begin(115200);
}
void loop() 
{
// UPDATE NUNCHUCK DATA
    Serial.print("----------------------------------------------------------------------------------------------");
    Serial.print('\n');
      WiiChk.get_data();

      WiiChk.print_data();

      int XV = WiiChk.joyx();
      int YV = WiiChk.joyy();
      int ZV = WiiChk.zbutton();
      int CV = WiiChk.cbutton();

// CRAP MOUSE BUTTON LOGIC, WILL PROBABLY NEED DEBOUNCE
      if (CV == 1 && ZV == 0)
      {
        digitalWrite(P_OUT_LMB, LOW); // LEFT MOUSE BUTTON
            Serial.print("BUTTON: LMB");
            Serial.print('\n');
      }
      else if (ZV == 1 && CV == 0)  
      {
        digitalWrite(P_OUT_RMB, LOW); // RIGHT MOUSE BUTTON
            Serial.print("BUTTON: RMB");
            Serial.print('\n');
      }
      else if (ZV == 1 && CV == 1)
      {
        digitalWrite(P_OUT_MMB, LOW); // MIDDLE MOUSE BUTTON
        Serial.print("BUTTON: MMB");
        Serial.print('\n');
      }
      else
      {
        digitalWrite(P_OUT_LMB, HIGH);
        digitalWrite(P_OUT_RMB, HIGH);
        digitalWrite(P_OUT_MMB, HIGH);
        Serial.print("BUTTON: NONE");
        Serial.print('\n');
      }
      
      
// XY QUADRATURE
  int h[4] = {0,0,0,0};
  int hq[4] = {0,0,0,0};
  int v[4] = {0,0,0,0};
  int vq[4] = {0,0,0,0};
// YV IS UP/DOWN
  if (YV > (130 + CENTRE_DBAND)) 
  {
    //UP
    DLYX = map(YV, 130, 255, 20, 0);
    v[0]  = 1; v[1]  = 0; v[2]  = 0; v[3]  = 1;
    vq[0] = 0; vq[1] = 0; vq[2] = 1; vq[3] = 1;
    Serial.print("DIRECTION: UP");
    Serial.print('\t');
    TRIGGER = 1;
  } 
  else if (YV < (130 - CENTRE_DBAND)) 
  {
    //DOWN
    DLYX = map(YV, 130, 0, 20, 0);
    v[0]  = 0; v[1]  = 0; v[2]  = 1; v[3]  = 1;
    vq[0] = 1; vq[1] = 0; vq[2] = 0; vq[3] = 1;
    Serial.print("DIRECTION: DOWN");
    Serial.print('\t');
    TRIGGER = 1;
  }
  else
  {
    TRIGGER = 0;
  }
// XV IS LEFT/RIGHT  
  if (XV > (130 + CENTRE_DBAND))
  {
    //RIGHT
    DLYY = map(XV, 130, 255, 20, 1);
    h[0]  = 1; h[1]  = 0; h[2]  = 0; h[3]  = 1;
    hq[0] = 0; hq[1] = 0; hq[2] = 1; hq[3] = 1;
    Serial.print("DIRECTION: RIGHT");
    Serial.print('\t');
    TRIGGER2 = 1;
  } 
  else if (XV < (130 - CENTRE_DBAND)) 
  {
    //LEFT
    DLYY = map(XV, 130, 0, 20, 0);
    h[0]  = 0; h[1]  = 0; h[2]  = 1; h[3]  = 1;
    hq[0] = 1; hq[1] = 0; hq[2] = 0; hq[3] = 1;
    Serial.print("DIRECTION: LEFT");
    Serial.print('\t');
    TRIGGER2 = 1;
  }
  else
  {
   TRIGGER2 = 0;
  }

// PROBABLY UNSTABLE AT 20MS - 20X8 = 160MS X2 = .3 SECONDS - BUTTONS MAY BE IGNORED
if (TRIGGER == 1) // FALL INTO HERE ONLY IF PAST DEADBAND VALUE. IF DRIFTING, BUTTONS WON'T WORK
{
  for(int i = 0; i < 4; i++) 
  {
    digitalWrite(P_OUT_H_PULSE, h[i]);
    digitalWrite(P_OUT_HQ_PULSE, hq[i]);
    delay(DLYX);
  
  
  }
  TRIGGER = 0;
  Serial.print('\n');
  Serial.print("---Y MOUSE OUTPUT ACTIVE---");
}
else
{
   Serial.print('\n');
  Serial.print("---Y MOUSE OUTPUT INACTIVE---");
}

if (TRIGGER2 == 1)
{
  for(int i = 0; i < 4; i++) 
  {
    digitalWrite(P_OUT_V_PULSE, v[i]);
    digitalWrite(P_OUT_VQ_PULSE, vq[i]);
    delay(DLYY);
  }
  TRIGGER2= 0;
  Serial.print('\n');
  Serial.print("---X MOUSE OUTPUT ACTIVE---");
  Serial.print('\n');
}
else
{
 
  Serial.print('\n');
  Serial.print("---X MOUSE OUTPUT INACTIVE---");
  Serial.print('\n');
}


    Serial.print("NUNCHUCK X:");
    Serial.print(WiiChk.joyx(), DEC);
    Serial.print(",NUNCHUCK Y:");
    Serial.print(WiiChk.joyy(), DEC);
    Serial.print("  \t");
    Serial.print('\n');
    Serial.print("DELAY-Y: ");
    Serial.print(DLYY);
    Serial.print("  \t");   
    Serial.print("DELAY-X: ");
    Serial.print(DLYX);
    Serial.print('\n');
     Serial.print('\n');
    Serial.print("----------------------------------------------------------------------------------------------");
    Serial.print('\n');
delay(500);

}