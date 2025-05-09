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
      int DLYX = 10;
      int DLYY = 10;
      int CENTREX;
      int CENTREY;
      int CENTRE_DBAND = 5; // PREVENT "JOYCON DRIFT"
      int TRIGGER = 0;
      int TRIGGER1 = 0;
      int TRIGGER2 = 0;
      int AVERAGEX[10] = {0,0,0,0,0,0,0,0,0,0};
      int AVERAGEY[10] = {0,0,0,0,0,0,0,0,0,0};
      int AVCOUNTX;
      int AVCOUNTY;

      // XY QUADRATURE
      int h[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      int hq[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      int v[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      int vq[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


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

AVCOUNTX = AVCOUNTX + 1;
AVCOUNTY = AVCOUNTY + 1;
if (AVCOUNTX == 11)
{
  AVCOUNTX == 0;
}
if (AVCOUNTY == 11)
{
  AVCOUNTY == 0;
}

//  for (int i=0; i < 5; i++) 
//  {
//  AVERAGEX = AVERAGEX + DLYX;
//  }
//  AVERAGEX = AVERAGEX/5;    

// YV IS UP/DOWN
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
      

  if (YV > (130 + CENTRE_DBAND)) 
  {
    //UP

    DLYX = map(YV, 130, 225,30000, 2000);
    v[0]  = 0; v[1]  = 0; v[2]  = 1; v[3]  = 1;
    vq[0] = 1; vq[1] = 0; vq[2] = 0; vq[3] = 1;
    TRIGGER = 1;


  } 
  else if (YV < (130 - CENTRE_DBAND)) 
  {
    //DOWN

        DLYX = map(YV, 130, 40, 30000, 2000);
    v[0]  = 1; v[1]  = 0; v[2]  = 0; v[3]  = 1;
    vq[0] = 0; vq[1] = 0; vq[2] = 1; vq[3] = 1;
    TRIGGER = 1;
  }
  else
  {
    DLYX = 30000;
    v[0]  = 0; v[1]  = 0; v[2]  = 0; v[3]  = 0;
    vq[0] = 0; vq[1] = 0; vq[2] = 0; vq[3] = 0;
    TRIGGER = 0;
  }





// XV IS LEFT/RIGHT  
  if (XV > (130 + CENTRE_DBAND))
  {
    //RIGHT
    DLYY = map(XV, 130, 225, 30000, 2000);
    h[0]  = 0; h[1]  = 0; h[2]  = 1; h[3]  = 1;
    hq[0] = 1; hq[1] = 0; hq[2] = 0; hq[3] = 1;
    TRIGGER2 = 1;
  } 
  else if (XV < (130 - CENTRE_DBAND)) 
  {
    //LEFT


    DLYY = map(XV, 130, 40, 30000, 2000);
    h[0]  = 1; h[1]  = 0; h[2]  = 0; h[3]  = 1;
    hq[0] = 0; hq[1] = 0; hq[2] = 1; hq[3] = 1;

    TRIGGER2 = 1;
  }
  else
  {
    DLYY = 30000;
    h[0]  = 0; h[1]  = 0; h[2]  = 0; h[3]  = 0;
    hq[0] = 0; hq[1] = 0; hq[2] = 0; hq[3] = 0;
   TRIGGER2 = 0;
  }


// PROBABLY UNSTABLE AT 20MS - 20X8 = 160MS X2 = .3 SECONDS - BUTTONS MAY BE IGNORED
// if (TRIGGER == 1) // FALL INTO HERE ONLY IF PAST DEADBAND VALUE. IF DRIFTING, BUTTONS WON'T WORK
// {


//  for (int i=0; i < 5; i++) 
//  {
//  AVERAGEX = AVERAGEX + DLYX;
//  }
//  AVERAGEX = AVERAGEX/5;

//  for (int i=0; i < 10; i++) 
//  {
//  AVERAGEY = AVERAGEY + DLYY;
//  }
//  AVERAGEY = AVERAGEY/5;





  for(int i = 0; i < 4; i++) 
  {
    digitalWrite(P_OUT_H_PULSE, h[i]);

    digitalWrite(P_OUT_HQ_PULSE, hq[i]);

    digitalWrite(P_OUT_V_PULSE, v[i]);

    digitalWrite(P_OUT_VQ_PULSE, vq[i]);

  }
 // AVERAGE = (XY + XV / 2)
 if (TRIGGER == 1 || TRIGGER2 == 1)
 {
  delayMicroseconds(DLYY + DLYX);
 }
  //   if (DLYX > DLYY)
  //   {
  //     delayMicroseconds(AVERAGEX);     
  //   }
  //   else
  //   {
  //     delayMicroseconds(AVERAGEY);     
  //   }
  // }
  // else if (TRIGGER == 0 && TRIGGER2 == 1)
  // {
  //   //DLYY
  //     delayMicroseconds(AVERAGEY);
  // }
  // else if (TRIGGER == 1 && TRIGGER2 == 0)
  // {
  //     delayMicroseconds(AVERAGEX);
  // }


//  TRIGGER = 0;

//}
// else
// {

// }

// //
//   for(int i = 0; i < 4; i++) 
//   {

//    // delay(DLYY);
//   }
// //  TRIGGER2= 0;
// //}
// else
// {
 
//}


//     Serial.print("NUNCHUCK X:");
//     Serial.print(WiiChk.joyx(), DEC);
//     Serial.print(",NUNCHUCK Y:");
//     Serial.print(WiiChk.joyy(), DEC);
//     Serial.print("  \t");
//     Serial.print('\n');
//     Serial.print("DELAY-Y: ");
//     Serial.print(DLYY);
//     Serial.print("  \t");   
//     Serial.print("DELAY-X: ");
//     Serial.print(DLYX);
//     Serial.print('\n');
//      Serial.print('\n');
//     Serial.print("----------------------------------------------------------------------------------------------");
//     Serial.print('\n');
// delay(500);

}