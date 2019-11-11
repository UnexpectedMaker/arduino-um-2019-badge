#include "UM_Badge_2019.h"
UM_Badge_2019 badge = UM_Badge_2019();

#define BUTTON_IO 3
// Button tracking
int buttonPress = LOW;
int demo = 0;
long nextButton = 0;

// Display variables - these are used for various effects in each demo 
int outerBrightness = 256;
int innerBrightness = 512;
int outerInc = 256;
int innerInc = 256;
int innerStep = 0;
int edgeIndex = 8;
int edgeStep = 0;
int seg = 0;
int wall = 0;
unsigned long nextWallChange = 0;
int current_Inner = 0;

unsigned long animationStep = 0;

// Indexes for animated sections of walls - used in various demos
int wall_index[4] = { 0, 1, 2, 3 };
int outer_walls_index[12] = { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
int inner_walls_index[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
int intensities[3] = { 1200, 400, 200 };

// Setup flag for the current demo, set to true on every demo switch to ensure variables are reset
bool setupDemo = true;

void setup()
{
  // Set the button IO to be an input
  pinMode( BUTTON_IO, INPUT );
}

void loop()
{
  // Process the button press
  processButton();

  if ( animationStep < millis() )
  {
    animationStep = millis() + 100;
  
    switch ( demo )
    {
      case 0:
        showDemo_0();
        break;
  
       case 1:
        showDemo_1();
        break;
  
       case 2:
        showDemo_2();
        break;
  
       case 3:
        showDemo_3();
        break;
       
    }
  }
}

void showDemo_0()
{
   // if we just swictehd to this demo mode, reset the variables specific to this demo
  if ( setupDemo )
  {
    setupDemo = false;
    
    outerBrightness = 256;
    innerBrightness = 512;
    
    outerInc = 256;
    innerInc = 256;
    innerStep = 0;
    
    edgeIndex = 8;
    edgeStep = 0;
    
    seg = 0;
    wall = 0;
  }
  
  int pwm = 1024;
  
  for ( int p = 0; p < 12; p++ )
  {
    badge.setLED( outer_walls_index[p], pwm);
    pwm = constrain( pwm - 128, 0, 1024 );
    outer_walls_index[p]--;
    if ( outer_walls_index[p] < 12 )
      outer_walls_index[p] = 23;
  }

  if ( outer_walls_index[0] == 12 )
  {
    for ( int iw = 0; iw < 8; iw++ )
    {
       badge.setLED( iw, ( ( iw == current_Inner ) ? 1024 : 0 ) );
    }
    current_Inner++;
    if ( current_Inner > 7 )
      current_Inner = 0;
  }

  for ( int i = 8; i < 12; i++ )
  {
    badge.setLED( i, innerBrightness );
  }
  
  innerBrightness += innerInc;
  if ( innerBrightness < 256 || innerBrightness > 4095 )
    innerInc = -innerInc;

  innerBrightness = constrain( innerBrightness, 256, 4095 );

  badge.updateBadge();
}

void showDemo_1()
{
   // if we just swictehd to this demo mode, reset the variables specific to this demo
  if ( setupDemo )
  {
    setupDemo = false;

    wall_index[0] = 0;
    wall_index[1] = 1;
    wall_index[2] = 2;
    wall_index[3] = 3;

    nextWallChange = 0;
  }

  if ( nextWallChange < millis() )
  {
    nextWallChange = millis() + 100;
    
    badge.drawWall( wall_index[0], 2048, false );;
    badge.drawWall( wall_index[1], 512, false );
    badge.drawWall( wall_index[2], 128, false );
    badge.drawWall( wall_index[3], 32, false );
    badge.updateBadge();
  
    for ( int ws = 0; ws < 4; ws++ )
    {
      wall_index[ws]--;
      if ( wall_index[ws]< 0 )
        wall_index[ws] = 3;
    }
  }
}

void showDemo_2()
{
  // if we just swictehd to this demo mode, reset the variables specific to this demo
  if ( setupDemo )
  {
    setupDemo = false;

    outerBrightness = 256;
    innerBrightness = 512;

    outerInc = 256;
    innerInc = 32;
    innerStep = 0;

    edgeIndex = 8;
    edgeStep = 0;
  }

  badge.drawBoxOuter( outerBrightness, false );

  outerBrightness += outerInc;

  if ( outerBrightness < 256 || outerBrightness > 2048 )
    outerInc = -outerInc;

  outerBrightness = constrain( outerBrightness, 256, 2048 );

  badge.setLED( edgeIndex, ( ( edgeStep == 0) ? 1024 : 0 ) );
  
  if ( edgeStep < 3 )
  {
    edgeStep++;
  }
  else
  {
    edgeStep = 0;
    edgeIndex++;
    if ( edgeIndex > 11 )
    {
      edgeIndex = 8;

      if ( innerStep == 0 )
        innerStep = 512;
      else
        innerStep = 0;
    }
  }
   
  badge.drawBoxInner ( innerStep, false );

  // Last thing we do is write the data to the TLC5947
  badge.updateBadge();
}

void showDemo_3()
{
  // if we just swictehd to this demo mode, reset the variables specific to this demo
  if ( setupDemo )
  {
    setupDemo = false;

    outerBrightness = 1000;
    innerBrightness = 512;

    outerInc = 450;
    innerInc = 32;
    innerStep = 0;

    edgeIndex = 8;
    edgeStep = 0;
  }

  if ( nextWallChange < millis() )
  {
    nextWallChange = millis() + 200;
    badge.drawBoxOuter( intensities[innerStep + edgeStep], false );

    innerStep++;

    for ( int i = 8; i < 12; i++ )
      badge.setLED( i, intensities[innerStep + edgeStep] );
  
    innerStep++;
    
    badge.drawBoxInner( intensities[innerStep + edgeStep], false );

    innerStep = 0;
    edgeStep++;
    if ( edgeStep == 3)
      edgeStep = 0;

    // Last thing we do is write the data to the TLC5947
    badge.updateBadge();
  }
}


void processButton()
{
  if ( buttonPress != digitalRead( BUTTON_IO ) )
  {
    buttonPress = digitalRead( BUTTON_IO );
    
    if ( !buttonPress && nextButton < millis() )
    {
      demo++;
      if ( demo == 4 )
        demo = 0;

      setupDemo = true;

      badge.clear();
      
      nextButton = millis() + 50;
    }
  }
}
