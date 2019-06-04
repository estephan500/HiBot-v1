/* huzzah and 240 x 320 ili9341... a match made in heaven */

#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ILI9341.h> // Include Adafruit_ILI9341 library to drive the display
// Declare pins for the display:
#define TFT_DC 16 // WAS ORIG 9
#define TFT_RST 12  // WAS ORIG 8~ // You can also connect this to the Arduino reset in which case, set this #define pin to -1!
#define TFT_CS 15  // WAS ORIG 10~
// The rest of the pins are pre-selected as the default hardware SPI for Arduino Uno (SCK = 13 and SDA = 11)
// Create display:
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
//#include <Fonts/FreeSerif24pt7b.h>  // Add a custom font

const int screenwidth = 240;
const int screenheight = 320;
//const int numdotrows = 20, numdotcols = 15;
//const int numdotrows = 4*5, numdotcols = 3*5;
//const int numdotrows = 4, numdotcols = 3;
const int numdotrows = 320/6, numdotcols = 240/6; //8 good 7 good 6 good... 40x53
const int dotwidth = screenwidth / numdotcols;
const int dotheight = screenheight / numdotrows;
const int maxmenu = 8;
int menuitem=0;
bool firstrun = true;
String menunames[] = {"a poem          ","pong                 ","life is good           ", "rocket circuit          ", "dot circuit          ", "linear division          ","sparkle          ","sparkleVi        ","ballbot       "};
float ballx,bally,xvel,yvel; // global ball variables... embarrassing



uint16_t colorof(uint8_t r, uint8_t g, uint8_t b) { 
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3); 
}

// CLASS DOT

class Dot {
  public:
    //constructor
    Dot(int x, int y, bool lit, bool tobelit, bool altpad);
    // methods
    void light(bool currentnotfuture);
    void douse(bool currentnotfuture);
    void toggle();
    void draw(uint16_t color);
    bool look(bool currentnotfuture);
    void shift();
    void altset(bool setas);
    bool lookalt();
  private:
    int _x;
    int _y;
    bool _lit;
    bool _tobelit;
    bool _altpad;
};

// DOT METHODS
Dot::Dot(int x, int y, bool lit, bool tobelit, bool altpad) {
  _x = x;
  _y = y;
  _lit = lit;
  _tobelit = tobelit;
  _altpad = altpad;
}

void Dot::light(bool currentnotfuture) {
  if (currentnotfuture) {
    _lit = true;
  }
  else {
    _tobelit = true;
  }
 
}

void Dot::douse(bool currentnotfuture) {
  if (currentnotfuture) {
    _lit = false;
  }
  else {
    _tobelit = false;
  }
}

void Dot::altset(bool setas) {
    _altpad = setas;
}

bool Dot::lookalt() {
    return _altpad;
}

void Dot::toggle() {
  _lit = not(_lit);
}

void Dot::draw(uint16_t color) {
  uint16_t usecolor;
  int drawx = (dotwidth) * _x;
  int drawy = (dotheight) * _y;
  if (_lit) {
    usecolor=color;
  }
  else {
    usecolor=0x0000;
  }
  tft.fillRect(drawx,drawy,dotwidth,dotheight,usecolor);
}

bool Dot::look(bool currentnotfuture) {
  if (currentnotfuture) {
    return _lit;
  }
  else {
    return _tobelit;
  }
}

void Dot::shift() {
  _lit = _tobelit;
}


//INSTANTIATE DOTS

Dot *dots[numdotcols][numdotrows];


void waitbutton(int pin, bool shouldclear) {
  while (digitalRead(pin) != HIGH ) {
    delay(5); // DLY FOR WAITBUTTON CYCLE
  };
  if (shouldclear) {
    tft.fillScreen(0x0000);
  }
}

void message(String s) {
  tft.setTextColor(colorof(0,0,255));  // Set color of text. First is the color of text and after is color of background
  tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
  tft.setCursor(2, 100);  // Set position (x,y)
  tft.println(s); 
}


void g1drawit(int linestyle, int xx, int yy, int xxx, int yyy, int radius) {
  if (linestyle==0) {
    tft.fillCircle(xx,yy,radius,colorof(xx+15,int(yy/2),254));
  }
  else {
    tft.drawLine(xx,yy,xxx,yyy,colorof(xx+15,int(yy/2),254 ));
  }
}

void g1eraseit(int linestyle, int xx, int yy, int xxx, int yyy,int radius, uint16_t erasecol) {
    if (linestyle==0) {
      tft.fillCircle(xx,yy,radius+1,erasecol);
    }
    else {
      tft.drawLine(xx,yy,xxx,yyy,erasecol);
    }
}

void game0()  {
  tft.fillScreen(0x0000);
  while (digitalRead(5) == HIGH ) {};
  tft.fillScreen(colorof(255,255,255));
  tft.fillCircle(64,64,50,colorof(226,186,104));
  tft.fillCircle(40,20,10,colorof(226,186,104));
  tft.fillCircle(85,20,10,colorof(226,186,104));
  tft.fillCircle(116,40,10,colorof(114,77,3));
  tft.fillCircle(46,35,6,colorof(0,0,0));
  tft.fillCircle(79,35,6,colorof(0,0,0));
  tft.fillCircle(62,38,6,colorof(0,0,0));
  tft.fillCircle(62,33,6,colorof(226,186,104));

  
  tft.setTextColor(colorof(0,0,255));  // Set color of text. First is the color of text and after is color of background
  tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
  tft.setCursor(2, 100);  // Set position (x,y)
  tft.println("I want to go\nto Mount Splashmore\n\nTake me take me\ntake me take me\nnow\n\nNow now now now now\n\nMount Splashmore,\ntake me there\nright now");  
  waitbutton(4,true);
}

void game1(int linestyle, int maxradius, int del, int speedlimit, bool doerase, uint16_t erasecolor)  {  // xxxxxxxxxxx xxxxxxxxxxxx xxxxxxxxxxxx
  const int lastcell = 19;
  const int xmax = screenwidth-1; //239   
  const int ymax = screenheight-1; //319;
  int x[20];   
  int y[20];
  int xb[20];   
  int yb[20];
  int radiusvel = 1;
  int ptnew = 10;
  int ptold = 11;
  int i;
  int xvel;
  int yvel;
  int xbvel;
  int ybvel;
  int fullcount;
  int radius = 5;
  bool quitter = false;

  tft.fillScreen(0x0000);

  xvel = random(4,speedlimit);
  yvel = random(4,speedlimit);
  for (i=0;i<20;i++) {
    x[i]=120;
    y[i]=160; 
  }

  xbvel = random(4,speedlimit);
  ybvel = random(4,speedlimit);
  for (i=0;i<20;i++) {
    xb[i]=120;
    yb[i]=160; 
  }
  
  while (not quitter) {  //  ***** full game loop
    
    if (doerase) {
      if (linestyle == 0 or linestyle == 1) {
        g1eraseit(linestyle,x[ptold],y[ptold],x[ptnew],y[ptnew], radius, erasecolor);
      }
      if (linestyle == 2) {
        g1eraseit(linestyle,x[ptold],y[ptold],xb[ptold],yb[ptold], radius, erasecolor);
      }
    }
    
    int previousx = x[ptnew];  // store the x's and y's for building the next one
    int previousy = y[ptnew];
    int previousxb = xb[ptnew];
    int previousyb = yb[ptnew];
    
    ptnew += 1;  // prepare the next index numbers
    if (ptnew > lastcell) {ptnew = 0;}
    ptold += 1;
    if (ptold > lastcell) {ptold = 0;}
    
    int newx = previousx + xvel;  // only for the plain x set, discover & correct new coords
    int newy = previousy + yvel;
    
    if (newx > xmax) {
      xvel = -xvel;
      newx = xmax +1 - (newx-xmax);
    }
    if (newx < 0) {
      xvel = -xvel;
      newx = 0 -1 - (newx-0);
    }
    if (newy > ymax) {
      yvel = -yvel;
      newy = ymax +1 - (newy-ymax);
    }
    if (newy < 0) {
      yvel = -yvel;
      newy = 0 -1 - (newy-0);
    }
    x[ptnew] = newx;  // ... when satisfied, lock them into the array
    y[ptnew] = newy;  // done for plain x's

    int newxb = previousxb + xbvel;  // NOW for the plain b set, discover & correct new coords
    int newyb = previousyb + ybvel;
    
    if (newxb > xmax) {
      xbvel = -xbvel;
      newxb = xmax +1 - (newxb-xmax);
    }
    if (newxb < 0) {
      xbvel = -xbvel;
      newxb = 0 -1 - (newxb-0);
    }
    if (newyb > ymax) {
      ybvel = -ybvel;
      newyb = ymax +1 - (newyb-ymax);
    }
    if (newyb < 0) {
      ybvel = -ybvel;
      newyb = 0 -1 - (newyb-0);
    }
    xb[ptnew] = newxb;  // ... when satisfied, lock them into the array
    yb[ptnew] = newyb;  // done for b's
    
    if (linestyle == 0 or linestyle == 1) {
      g1drawit(linestyle,x[ptnew],y[ptnew],x[ptold],y[ptold],radius);
    }
    if (linestyle == 2) {
      g1drawit(linestyle,x[ptnew],y[ptnew],xb[ptnew],yb[ptnew],radius); // draw the new...
    }

    
    if (digitalRead(5) == HIGH ){  // BUTTON 5 RANDOMIZES
      xvel = random(-speedlimit,speedlimit);
      yvel = random(-speedlimit,speedlimit);
      for (i=0;i<20;i++) {
        x[i]=120;
        y[i]=160; 
      }
      tft.fillScreen(0x0000);
    }
  
    if (digitalRead(4) == HIGH ){  // BUTTON 4 QUITS
      quitter = true;
    }
    
    fullcount++;  // PERIODIC RADIUS CHANGE
    if (fullcount>99999) {fullcount=0;}
    if (fullcount%3==0)  {
      radius+= radiusvel;
      if (radius > maxradius or radius<5) {
        radiusvel = -radiusvel;
        radius+= radiusvel;
      }
    }

    delay(del); // dly for game1

    
  }  // end of quitter loop
  tft.fillScreen(0x0000);
} // end of game1 procedure

void game2() { //+++++ooooo+++oooo+++++ooooooo++++++ooooooo+++
  const int xmax = 239;
  const int ymax = 319;
  int x= 120;
  int y= 160;
  int previousx = x;
  int previousy = y;
  int xvel=2;
  int yvel=0;
  int ygrav=1;
  int xgrav=0;
  int del = 10;
  int radius = 10;
  bool quitter = false;
  
  tft.fillScreen(0x0000);

  while (not quitter) {  //  ***** full game loop

    // draw it
    g1drawit(0,x,y,previousx,previousy,radius);
    
    // remember it
    previousx = x;
    previousy = y;
  
    // gravity effect
    xvel += xgrav;
    yvel += ygrav;
    
    // change it and correct it

    int newx = previousx + xvel;  // copied from game1 and modified. discover & correct new coords
    int newy = previousy + yvel;
    
    if (newx > xmax) {
      xvel = -xvel;
      newx = xmax +1 - (newx-xmax);
    }
    if (newx < 0) {
      xvel = -xvel;
      newx = 0 -1 - (newx-0);
    }
    if (newy > ymax) {
      yvel = -yvel;
      newy = ymax +1 - (newy-ymax);
    }
    if (newy < 0) {
      yvel = -yvel;
      newy = 0 -1 - (newy-0);
    }
    x = newx;  // ... when satisfied, lock them into the array
    y = newy;  // done for plain x's
  
    delay(del); //dly for game2

    if (digitalRead(4) == HIGH ){  // BUTTON 4 QUITS
      quitter = true;
    }

    if (digitalRead(5) == HIGH ){  // RANDOMIZE
      xgrav = random (-1,2);
      ygrav = random (-1,2);
      radius = random (2,20);
      xvel=1;
      yvel=1;
      tft.fillScreen(0x0000);
    }
    
    // erase old
    g1eraseit(0,previousx,previousy,x,y,radius,colorof(0,0,0));
    
  } // end of quitter loop

  tft.fillScreen(0x0000);
  
} // End Game2

void g3place(int x, int y, uint16_t color) {
  int drawx = (dotwidth) * x;
  int drawy = (dotheight) * y;
  tft.fillRect(drawx,drawy,dotwidth,dotheight,color);
} 

void g3pretty() {
  for (int xscan=0; xscan<numdotcols;xscan++) {
    for (int yscan=0; yscan<numdotrows;yscan++) {
      g3place(xscan,yscan,colorof((xscan+yscan % 2)*128+90, xscan*(255/numdotcols),yscan*(255/numdotrows) ));
    }
  }
}

void g3drawfield() {
  for (int xscan=0; xscan<numdotcols;xscan++) {
    for (int yscan=0; yscan<numdotrows;yscan++) {
      dots[xscan][yscan]->draw(0xFFFF);
    }
  }
}


void g3shiftall() {
  for (int xscan=0; xscan<numdotcols;xscan++) {
    for (int yscan=0; yscan<numdotrows;yscan++) {
      dots[xscan][yscan]->shift();
    }
  }
}

int countneighbors(int x,int y) {   // COUNT THE NEIGHBOR DOTS
  int hstart = max(x-1, 0);
  int hend = min(x+1, numdotcols-1);
  int count = 0;



  // the three TOP LAYER cells
  if (y-1 > 0) {
    for (int xscan = hstart; xscan<hend+1; xscan++) {
      if (dots[xscan][y-1]->look(true)) {
        count++;
      }
    }
  }


  // the two MIDDLE LAYER cells
  if (x-1 >= 0) {
    if (dots[x-1][y]->look(true)) {
        count++;
    }
  }


  if (x+1 < numdotcols) {
    if (dots[x+1][y]->look(true)) {
        count++;
    }
  }


  // the two BOTTOM LAYER cells

  if (y+1 < numdotrows) {
    for (int xscan = hstart; xscan<hend+1; xscan++) {

      if (dots[xscan][y+1]->look(true)) {
        count++;
        
      }
      

    }

  }


  return count;
} // end of countneighbors


void g3liferulepass() {
  for (int xscan=0; xscan<numdotcols;xscan++) {
    for (int yscan=0; yscan<numdotrows;yscan++) { 
       int c = countneighbors(xscan,yscan);
       
       if(dots[xscan][yscan]->look(true)) {  // he was alive
          if (c<2 or c>3) {
            dots[xscan][yscan]->douse(false); //snuffed

          }
          else {
            dots[xscan][yscan]->light(false); //survives
          }
          
       } // end of he was alive
       
       else {  // he was dead
          if (c==3) {
            dots[xscan][yscan]->light(false); // born

          }
          else{
            dots[xscan][yscan]->douse(false); // not born
          }
       } // end of he was dead

    }

  } // at this point the whole second array should be populated with the next set
}

void g3fillrandom() {
  
  for (int xscan=0; xscan<numdotcols;xscan++) {  // fill with random
      for (int yscan=0; yscan<numdotrows;yscan++) {
        int randall = random(0,2);
        bool notedge = (xscan >15 and xscan<(numdotcols-15) and yscan>12 and yscan < (numdotrows-12));
        if (randall == 1 and notedge) {
          dots[xscan][yscan]->light(true);
        }
        else {
          dots[xscan][yscan]->douse(true);
        }
      }
  } // end scans
}

void game3() {  //=====three=====three=====three=====three=====three=====three=====three=====three=====three=====three
  bool quitter = false;
  tft.fillScreen(0x0000);




  g3fillrandom();

  
  g3drawfield(); // should show the randoms AND WE ARE SEEDED AND READY TO START


  while (not quitter) {
    
    g3liferulepass();
    g3shiftall();
    g3drawfield();
    //message("did g3drawfield");

    
    if (digitalRead(4) == HIGH or digitalRead(5) == HIGH){  // BUTTON 4 or 5 
      if (digitalRead(4) == HIGH) {
        quitter = true;
      }
      else {
        g3fillrandom();
        g3drawfield();
      }
      
    }

    
  }
  tft.fillScreen(0x0000);

}

void g4cleargrid() {
  for (int xscan=0; xscan<numdotcols;xscan++) {  // fill with random
      for (int yscan=0; yscan<numdotrows;yscan++) {
        dots[xscan][yscan]->douse(true);      
      }
  } // end scans
}

void g4initialpaddle(int padx, int padwidth) {
  for (int xscan=padx; xscan < padx + padwidth; xscan++) {
    dots[xscan][numdotrows-1]->light(true);
    dots[xscan][numdotrows-1]->draw(0xFFFF);
    dots[xscan][numdotrows-1]->altset(true); 
    
  }
}


int g4ballmove(int gotpongscore) {                        //fballmo
  //Serial.print("ballmove called with ballx=");
  //Serial.print(ballx);
  //Serial.print(" and bally=");
  //Serial.println(bally);
  const int xmax = numdotcols-1;
  const int ymax = numdotrows-2;  //was -1 until I started the paddle crappe
  
  float previousx = ballx;
  float previousy = bally; 

  float newx = previousx + xvel;  
  float newy = previousy + yvel;


  if (newx > xmax) {
      xvel = -xvel;
      newx = xmax +1 - (newx-xmax);
    }
    if (newx < 0) {
      xvel = -xvel;
      newx = 0 -1 - (newx-0);
    }
    if ( newy > ymax and dots[int(newx)][int(newy)]->lookalt() ) {   // HIT WITH PADDLE  * * * * * * *
      gotpongscore++;
      //drawscore
      tft.fillRect(200,20,40,20,colorof(0,0,0));
      tft.setTextColor(colorof(0,0,255));  // Set color of text. First is the color of text and after is color of background
      tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
      tft.setCursor(200, 20);  // Set position (x,y)
      tft.println(gotpongscore);  
      //drawscore
      yvel = -yvel;
      newy = ymax  +1    - (newy-ymax);
      switch (random(1,5)) {
        case 1:
          xvel = -1;
          //Serial.println("was 1 so minus.8");
          break;
        case 2:
          xvel = 1;
          //Serial.println("was 2 so minus.25");
          break;
        case 3:
          xvel = 1;
          //Serial.println("was 3 so plus.25");
          break;
        case 4: 
          xvel = -0.5; 
          //Serial.println("was 4 so plus.4"); 
          break;
      }
    }
    if (newy < 0) {
      yvel = -yvel;
      newy = 0 -1 - (newy-0);
    }
    ballx = newx;  // ... when satisfied, lock them into the array
    bally = newy;  // done for plain x's

    //DRAW NEW
    dots[int(ballx)][int(bally)]->light(true);
    dots[int(ballx)][int(bally)]->draw(colorof(ballx*6,255-ballx*6,255));  
    
    //ERASE OLD
    if (not(int(ballx) == int(previousx) and int(bally) == int(previousy))) {
      dots[int(previousx)][int(previousy)]->douse(true);
      dots[int(previousx)][int(previousy)]->draw(colorof(255,255,0));
    }

    //Serial.print("... ballmove ENDING with ballx=");
    //Serial.print(ballx);
    //Serial.print(" and bally=");
    //Serial.println(bally);

    return gotpongscore;
}
  

void game4() {
  
  tft.fillScreen(0x0000);
  int paddlex=27;
  bool endlife = false;
  bool quitter = false;
  const int paddlewidth=7,paddlemax=numdotcols-paddlewidth;  // therefore 40 minus 7 = 33. 

  while (not quitter) {
    endlife = false;
    g4cleargrid();
    
    g4initialpaddle(paddlex,paddlewidth);

    ballx=int(numdotcols/2);
    bally=int(numdotrows/2);
    xvel=1; //5 
    yvel=-1;  //-.75
    int pongscore = 0;
    
    while (not endlife) {  // loop of one pong session
  
      pongscore = g4ballmove(pongscore);  
      if (bally > numdotrows-1) { endlife = true;  }
      
      delay(25);  //dly for pong
  
      //Serial.print(String("") +"---padx="+paddlex+" wid="+paddlewidth+"--");
  
      if (digitalRead(4) == HIGH or digitalRead(5) == HIGH){  // PRESSED BUTTON 4 OR 5
          
          
          if (digitalRead(4) == HIGH) {
            
            // LEFT BUTTON LEFT BUTTON LEFT BUTTON LEFT BUTTON LEFT BUTTON LEFT BUTTON LEFT BUTTON 
            //Serial.println(" ");
            //Serial.println(String("") +"**LEFT paddlex="+paddlex+"  paddlemax="+paddlemax);
            
            if (paddlex>0) {
              paddlex--;
              dots[paddlex][numdotrows-1]->light(true);
              dots[paddlex][numdotrows-1]->draw(0xFFFF);
              dots[paddlex][numdotrows-1]->altset(true);
              dots[paddlex + paddlewidth][numdotrows-1]->douse(true);
              dots[paddlex + paddlewidth][numdotrows-1]->draw(0xFFFF);
              dots[paddlex + paddlewidth][numdotrows-1]->altset(false);
            }
  
            // Aleftbutton
            //Serial.println(String("") +"lAFTER== paddlex="+paddlex+"  paddlemax="+paddlemax);
            //Serial.println("DONE ");
            
          }
          if (digitalRead(5) == HIGH) {
            
            // RIGHT BUTTON RIGHT BUTTON RIGHT BUTTON RIGHT BUTTON RIGHT BUTTON RIGHT BUTTON 
            //Serial.println(" ");
            //Serial.println(String("") +"**RIGHT paddlex="+paddlex+"  paddlemax="+paddlemax);
  
            
            if(paddlex<paddlemax) {
              paddlex++;
              dots[paddlex-1][numdotrows-1]->douse(true); // i had to also subtract one from paddlex. never clear why. to match pdw-1 I guess.
              dots[paddlex-1][numdotrows-1]->draw(0xFFFF);
              dots[paddlex-1][numdotrows-1]->altset(false);
              dots[paddlex + paddlewidth-1][numdotrows-1]->light(true);
              dots[paddlex + paddlewidth-1][numdotrows-1]->draw(0xFFFF);
              dots[paddlex + paddlewidth-1][numdotrows-1]->altset(true);
            }
            
            // Arightbutton
            //Serial.println(String("") +"rAFTER== paddlex="+paddlex+"  paddlemax="+paddlemax);
            //Serial.println("DONE ");
            
          }
      } // end button capture
  
    
    
    } // end endlife loop
    tft.fillCircle(120,160,80,colorof(200,100,40));
    delay(600); //delay for you lost your pong ball
    waitbutton(5,true);
    if (digitalRead(4) == HIGH and digitalRead(5) == HIGH) {
      endlife=true;
      quitter = true;
    }
  
  } // end quitter loop
}

void setup()  // Start of setup XX  XX  XX  XX  XX  XX  XX  
{
  //Serial.begin(115200);
  randomSeed(analogRead(0)); 

  //SET UP GRID OF DOTS
  for(int y=0;y<numdotrows;y++) {
    for (int x=0;x<numdotcols;x++) {
        dots[x][y] = new Dot(x,y,true, true,false); 
    }
  }
  
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  
  // Display setup:
  tft.begin();  // Initialize display
  tft.fillScreen(0x0000);  // Fill screen with black
  //tft.setRotation(0);  // Set orientation of the display. Values are from 0 to 3. If not declared, orientation would be 0,
                         // which is portrait mode.
}  // End of setup

void loop() {
  if (firstrun and false) {
    firstrun = false;
    //game1(2,5,20,7,true);
    game3(); // to force someone to top of UI...
  }
  tft.setCursor(120, 150);  // Set position (x,y)
  tft.setTextColor(colorof(255,150,0),colorof(0,0,0));  // Set color of text. First is the color of text and after is color of background
  tft.setTextSize(20);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
  tft.println(menuitem);  // Print a text or value
  tft.setCursor(15, 120);  // Set position (x,y)
  tft.setTextColor(colorof(224,2,2),colorof(0,0,0));  // Set color of text. First is the color of text and after is color of background
  tft.setTextSize(2);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
  tft.println(menunames[menuitem]);  // Print a text or value
  
  if (digitalRead(5) == HIGH ){
    if (menuitem == 0) {game0();}
    if (menuitem == 1) {game4();}
    if (menuitem == 2) {game3();}
    if (menuitem == 3) {game1(0,15,0,5,true,colorof(50,50,50));}
    if (menuitem == 4) {game1(0,5,20,12,true,colorof(50,50,50));}
    if (menuitem == 5) {game1(1,5,20,12,false,colorof(50,50,50));}
    if (menuitem == 6) {game1(2,5,20,7,true,colorof(50,50,50));}
    if (menuitem == 7) {game1(2,5,20,7,true,colorof(0,0,0));}
    if (menuitem == 8) {game2();}
    }

  if (digitalRead(4) == HIGH ){
    menuitem ++;
    if (menuitem > maxmenu) {menuitem = 0;}
    while (digitalRead(4) == HIGH ) {};
    
  }
}
