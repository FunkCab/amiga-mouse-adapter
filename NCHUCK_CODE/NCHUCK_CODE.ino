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
      int CENTRE_DBAND = 5; // PREVENT "JOYCON DRIFT"



void setup() 
{

      pinMode(P_OUT_V_PULSE,  OUTPUT);
      pinMode(P_OUT_H_PULSE,  OUTPUT);
      pinMode(P_OUT_VQ_PULSE, OUTPUT);
      pinMode(P_OUT_HQ_PULSE, OUTPUT);
      pinMode(P_OUT_LMB,      OUTPUT);


      WiiChk.get_data(); // Load the data from  the WiiChuck

      WiiChk.print_data();
      CENTREX = WiiChk.joyx();
      CENTREY = WiiChk.joyy();
      // Serial.begin(115200);
}
void loop() 
{
// UPDATE NUNCHUCK DATA
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
      }
      else if (ZV == 1 && CV == 0)  
      {
        digitalWrite(P_OUT_RMB, LOW); // RIGHT MOUSE BUTTON
      }
      else if (ZV == 1 && CV == 1)
      {
        digitalWrite(P_OUT_MMB, LOW); // MIDDLE MOUSE BUTTON
      }
      else
      {
        digitalWrite(P_OUT_LMB, HIGH);
        digitalWrite(P_OUT_RMB, HIGH);
        digitalWrite(P_OUT_MMB, HIGH);
      }
      
      
// XY QUADRATURE
  int h[4] = {0,0,0,0};
  int hq[4] = {0,0,0,0};
  int v[4] = {0,0,0,0};
  int vq[4] = {0,0,0,0};
// YV IS UP/DOWN
  if (YV > (CENTREY + CENTRE_DBAND)) 
  {
    //UP?
    DLYX = map(YV, CENTREY, 255, 20, 1);
    v[0]  = 1; v[1]  = 0; v[2]  = 0; v[3]  = 1;
    vq[0] = 0; vq[1] = 0; vq[2] = 1; vq[3] = 1;
  } 
  else if (YV < (CENTREY - CENTRE_DBAND)) 
  {
    //DOWN?
    DLYX = map(YV, CENTREY, 0, 1, 20);
    v[0]  = 0; v[1]  = 0; v[2]  = 1; v[3]  = 1;
    vq[0] = 1; vq[1] = 0; vq[2] = 0; vq[3] = 1;
  }
// XV IS LEFT/RIGHT  
  if (XV > (CENTREX + CENTRE_DBAND))
  {
    //LEFT?
    DLYY = map(XV, CENTREX, 255, 20, 1);
    h[0]  = 1; h[1]  = 0; h[2]  = 0; h[3]  = 1;
    hq[0] = 0; hq[1] = 0; hq[2] = 1; hq[3] = 1;
  } 
  else if (XV < (CENTREX - CENTRE_DBAND)) 
  {
    //RIGHT?
    DLYY = map(XV, CENTREX, 0, 1, 20);
    h[0]  = 0; h[1]  = 0; h[2]  = 1; h[3]  = 1;
    hq[0] = 1; hq[1] = 0; hq[2] = 0; hq[3] = 1;
  }
// PROBABLY UNSTABLE AT 20MS - 20X8 = 160MS X2 = .3 SECONDS - BUTTONS MAY BE IGNORED
if (XV > CENTRE_DBAND || XV < -CENTRE_DBAND || YV > CENTRE_DBAND || YV < -CENTRE_DBAND) // FALL INTO HERE ONLY IF PAST DEADBAND VALUE. IF DRIFTING, BUTTONS WON'T WORK
{
  for(int i = 0; i < 4; i++) 
  {
    digitalWrite(P_OUT_H_PULSE, h[i]);
    digitalWrite(P_OUT_HQ_PULSE, hq[i]);
    delay(DLYX);
  }

  for(int i = 0; i < 4; i++) 
  {
    digitalWrite(P_OUT_V_PULSE, v[i]);
    digitalWrite(P_OUT_VQ_PULSE, vq[i]);
    delay(DLYY);
  }
}
}