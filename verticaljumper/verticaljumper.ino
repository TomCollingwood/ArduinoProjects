/*

Vertical jump platformer arduino game code

Feel free to edit and use at your pleasure.

Used some code from the TFTHorizon example & TFT Etch A Sketch

By Tom Collingwood

 */

#include <Esplora.h>
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>


typedef double matrix_3d_t[4][4];

// initial position of the cursor
int xPos = EsploraTFT.width() / 2;
int yPos = 0;
int prevxPos = EsploraTFT.width() / 2;
int prevyPos = EsploraTFT.width() / 2;

int slidervalue = Esplora.readSlider();
int prevslidervalue= Esplora.readSlider();
int blue, green, red;

int pressstate=0;
float upvelocity=0;

int platwidth=20;

// We store platform x and ys here (so there are 5 platforms here)
int plats[10]={0,0,25,0,50,0,75,0,100,0};

bool dead = false;
int deadstate=0;

int speedy=0;

int step=0;

void setup() {
  // initialize the display
  EsploraTFT.begin();

  // clear the background

  EsploraTFT.background(150, 89, 43);


  for(int i=0; i<10; i+=2)
  {
      plats[i+1]=(random()) % 159;
    
  }
}
 

void loop() {

if(!dead)
{
  for(int i=0; i<10; i+=2)
  {
    EsploraTFT.stroke(0, 255, 0);
    EsploraTFT.line(plats[i+1],plats[i],plats[i+1]+platwidth,plats[i]);
    EsploraTFT.stroke(150, 89, 43);
    EsploraTFT.line(plats[i+1],plats[i]-1,plats[i+1]+platwidth,plats[i]-1);
    plats[i]=plats[i]+1;
    if(plats[i]>127) 
    {
      //sets the platform to the top of screen
      plats[i]=0; 
      //sets random x position
      plats[i+1]= (random()) % 159;
    }
  }
  step++;

  int tilt = Esplora.readAccelerometer(X_AXIS);

  // the values are 100 when tilted to the left
  // and  -100 when tilted to the right
  // map these values to the start and end points
  speedy = map(tilt, -100, 100, 5, -5);

  xPos+=speedy;

  // don't let the point go past the screen edges
  if (xPos > 159) {
    (xPos = 0);
  }

  if (xPos < 0) {
    (xPos = 159);
  }
  if (yPos > 127) {
    (dead=true);
  }

  if (yPos < 0) {
    (yPos = 0);
  }

   // Erase previous position of player
  EsploraTFT.stroke(150, 89, 43);
  EsploraTFT.point(prevxPos, prevyPos);
  EsploraTFT.point(prevxPos+1, prevyPos);
  EsploraTFT.point(prevxPos+1, prevyPos+1);
  EsploraTFT.point(prevxPos, prevyPos+1);
  EsploraTFT.point(prevxPos+1, prevyPos+2);
  EsploraTFT.point(prevxPos, prevyPos+2);
  // Create new position of player
  EsploraTFT.stroke(0, 0, 255);
  EsploraTFT.point(xPos, yPos);
  EsploraTFT.point(xPos+1, yPos);
  EsploraTFT.point(xPos+1, yPos+1);
  EsploraTFT.point(xPos, yPos+1);
  EsploraTFT.point(xPos+1, yPos+2);
  EsploraTFT.point(xPos, yPos+2);
  prevxPos=xPos;
  prevyPos=yPos;


  upvelocity-=0.3f;
  
  yPos-=upvelocity;

  // Check for collisions with platforms
  for(int i=0; i<10; i+=2)
  {
    if(yPos >= plats[i]-3 && prevyPos < plats[i] && xPos < plats[i+1]+platwidth && xPos > plats[i+1])
    {
      yPos=plats[i]-3;
      upvelocity=4.5f;
    }
  }
  }
  // If dead
  else{
    deadstate++;
    if(deadstate==1) EsploraTFT.background(150, 89, 43);
    EsploraTFT.text("U r ded, m8",20,50);
    EsploraTFT.text("Get rekt.",20,70);
    EsploraTFT.text("Press button to restart.",20,90);

    // Press button to restart
    if(Esplora.readButton(SWITCH_1)==LOW)
    {
      EsploraTFT.background(150, 89, 43);
      xPos = EsploraTFT.width() / 2;
      yPos = 0;

      pressstate=0;
      upvelocity=0;

      step=0;
      dead=false;
      deadstate=0;
    }
  }
  delay(33);
}
