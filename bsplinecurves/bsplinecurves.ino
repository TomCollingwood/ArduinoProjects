/*

 ORIGINAL CODE WAS: 
 Esplora TFT EtchASketch

 This example for the Arduino TFT and Esplora draws
 a white line on the screen, based on the position
 of the joystick. To clear the screen, shake the
 Esplora, using the values from the accelerometer.

 This example code is in the public domain.

 Created 15 April 2013 by Scott Fitzgerald

 http://www.arduino.cc/en/Tutorial/EsploraTFTEtchASketch

 EDITED BY TOM COLLINGWOOD INTO A B SPLINE PEN TOOL
  CODE ALSO USED FROM:
    http://krazydad.com/tutorials/makecolors.php (for colours)
    Peter Cominos (for B-spline method)

 */

#include <Esplora.h>
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>


typedef double matrix_3d_t[4][4];

// initial position of the cursor
int xPos = EsploraTFT.width() / 2;
int yPos = EsploraTFT.height() / 2;
int prevxPos = EsploraTFT.width() / 2;
int prevyPos = EsploraTFT.height() / 2;


int buttstate=0;


int step=0;

int _x1 =0;
int _y1 =0;
int _x2 =0;
int _y2 =0;
int _x3 =0;
int _y3 =0;
int _x4 =0;
int _y4 =0;

void drawcurve(int sx1,int sy1,int sx2,int sy2,int x3,int y3,int x4,int y4, bool blacks,bool temp)
{

      float frequency1=0.3f;
    float frequency2=0.3f;
    float frequency3=0.3f;
    int phase1=0;
    int phase2=2;
    int phase3=4;
    int center = 128;
    int width = 127;
    int len = 50;

    static int red, green, blue;


    static int colorstep=0;

   int n = 10;
    matrix_3d_t G;
    matrix_3d_t M;

    G[0][0]=sx1;
    G[0][1]=sy1;
    G[0][2]=0;
    G[0][3]=0;
    G[1][0]=sx2;
    G[1][1]=sy2;
    G[1][2]=0;
    G[1][3]=0;
    G[2][0]=x3;
    G[2][1]=y3;
    G[2][2]=0;
    G[2][3]=0;
    G[3][0]=x4;
    G[3][1]=y4;
    G[3][2]=0;
    G[3][3]=0;

    M[0][0]=-1.0/6.0;
    M[0][1]=3.0/6.0;
    M[0][2]=-3.0/6.0;
    M[0][3]=1.0/6.0;
    M[1][0]=3.0/6.0;
    M[1][1]=-6.0/6.0;
    M[1][2]=3.0/6.0;
    M[1][3]=0;
    M[2][0]=-3.0/6.0;
    M[2][1]=0;
    M[2][2]=3.0/6.0;
    M[2][3]=0;
    M[3][0]=1.0/6.0;
    M[3][1]=4.0/6.0;
    M[3][2]=1.0/6.0;
    M[3][3]=0;

    if (!temp && !blacks)
    {
      colorstep-=n;
    }


    matrix_3d_t C; /* Coefficient Matrix */
    double t1, t2, t3, dt, x1, y1, z1, w1, x2, y2, z2, w2;
    int ci;
    dt = 1.0 / n;
    /* Compute_Coefficient_Matrix: C = M * G */

    for (int r = 0; r < 4; r++)
    {
      for (int c = 0; c < 4; c++) \
      {
        C[r][c] = 0.0;

        for (int i = 0; i < 4; i++)
        {
          C[r][c] += ((M)[r][i] * (G)[i][c]);
        }
      }
    }

    x1 = (C)[3][0]; y1 = (C)[3][1]; z1 = (C)[3][2]; w1 = (C)[3][3];
    if (!(fabs(w1) > 0)) w1 = 1;
    for (ci = 1; ci <= n; ci++)
    {
    t1 = ci * dt;
    t2 = t1 * t1;
    t3 = t2 * t1;
    x2 = t3 * C[0][0] + t2 * C[1][0] + t1 * C[2][0] + C[3][0];
    y2 = t3 * C[0][1] + t2 * C[1][1] + t1 * C[2][1] + C[3][1];
    z2 = t3 * C[0][2] + t2 * C[1][2] + t1 * C[2][2] + C[3][2];
    w2 = t3 * C[0][3] + t2 * C[1][3] + t1 * C[2][3] + C[3][3];
    if (!(fabs(w2) > 0)) w2 = 1;

    if(!blacks) {
      //Credit: http://krazydad.com/tutorials/makecolors.php
      colorstep++;
      red = sin(frequency1*colorstep + phase1) * width + center;
      green = sin(frequency2*colorstep + phase2) * width + center;
      blue = sin(frequency3*colorstep + phase3) * width + center;
      EsploraTFT.stroke(blue, green, red);
    }

    else EsploraTFT.stroke(0, 0, 0);
    EsploraTFT.line(x1, y1, x2, y2);
    x1 = x2; y1 = y2; z1 = z2; w1 = w2;
    }
     /* draw_cubic_curve_segment */
}

void setup() {
  // initialize the display
  EsploraTFT.begin();

  // clear the background
  EsploraTFT.background(0, 0, 0);
}

void loop() {



  prevxPos=xPos;
  prevyPos=yPos;

  step++;
  int xAxis = Esplora.readJoystickX();    // read the X axis
  int yAxis = Esplora.readJoystickY();    // read the Y axis

  // update the position of the line
  // depending on the position of the joystick
  if (xAxis < 10 && xAxis > -10) {
    xPos = xPos;
  } else {
    xPos = xPos + (map(xAxis, -512, 512, 2, -2));
  }
  if (yAxis < 10 && yAxis > -10) {
    yAxis = yAxis;
  } else {
    yPos = yPos + (map(yAxis, -512, 512, -2, 2));
  }

  // don't let the point go past the screen edges
  if (xPos > 159) {
    (xPos = 159);
  }

  if (xPos < 0) {
    (xPos = 0);
  }
  if (yPos > 127) {
    (yPos = 127);
  }

  if (yPos < 0) {
    (yPos = 0);
  }

   // draw the point
  EsploraTFT.stroke(0, 0, 0);
  EsploraTFT.point(prevxPos, prevyPos);
  // draw the point
  EsploraTFT.stroke(255, 255, 255);
  EsploraTFT.point(xPos, yPos);
  

  if(Esplora.readButton(SWITCH_1)==LOW)
  {
    buttstate++;
    if(buttstate==1)
    {

      
      EsploraTFT.stroke(0, 0, 0);
      drawcurve(_x2,_y2,_x3,_y3,_x4,_y4,_x4,_y4,true,false);
    _x1=_x2;
    _y1=_y2;
    _x2=_x3;
    _y2=_y3;
    _x3=_x4;
    _y3=_y4;

    _x4=xPos;
    _y4=yPos;

    
    drawcurve(_x1,_y1,_x2,_y2,_x3,_y3,_x4,_y4,false,false);

    //clamped temp
    
    drawcurve(_x2,_y2,_x3,_y3,_x4,_y4,_x4,_y4,false,true);
    
    }
  }
  else
  {
    buttstate=0;
  }

  // check the accelerometer values and clear
  // the screen if it is being shaken
  if (Esplora.readButton(SWITCH_2)==LOW) {
    EsploraTFT.background(0, 0, 0);
  }

  delay(33);
}
