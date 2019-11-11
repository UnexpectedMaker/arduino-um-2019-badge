/*
 *  UM_BADGE_2019.cpp
 *
 * 	Driver library for the Unexpected maker 2019 Hackable Badge
 *  https://www.tindie.com/products/seonr/unexpected-maker-badge-2019/
 * 
 * 	See examples folder for implementation
 */

#include <UM_Badge_2019.h>

UM_Badge_2019::UM_Badge_2019()
{
    // Calculate buffer length and assign memory
    buffer = (uint16_t *)malloc(2 * 24);
    memset(buffer, 0, 2 * 24);

    // Set pins to outputs
    pinMode(_clk, OUTPUT);
    pinMode(_dat, OUTPUT);
    pinMode(_lat, OUTPUT);

    // Pull latch low
    digitalWrite(_lat, LOW);
}


void UM_Badge_2019::updateBadge()
{
    // Time to bitbang all the bits per LED channel to get the data from the buffer into the LED driver
    // Pull the latch pin low
    digitalWrite(_lat, LOW);

    // Iterate through all of the LEDs on the badge
    for (int16_t c = _leds - 1; c >= 0; c--)
    {
        // 1Iterate through the bits (12 bits) 
        for (int8_t b = 11; b >= 0; b--)
        {
            // Pull the clock pin low
            digitalWrite(_clk, LOW);
            // Write the data for the current bit - shifted each time for the entire 12 bits, MSB first
            digitalWrite(_dat, ( ( buffer[c] & (1 << b) ) ? HIGH : LOW ) );
            // Pull the clock pin high
            digitalWrite(_clk, HIGH);
        }
    }

    // Pull the clock pin low
    digitalWrite(_clk, LOW);

    // Pull the latch pin high to lock in the data
    digitalWrite(_lat, HIGH);
    // Pull the latch pin low to finish populated the LED driver
    digitalWrite(_lat, LOW);
}

void UM_Badge_2019::setLED(uint8_t lednum, uint16_t pwm)
{
    // Clamp the max PWM value for the LED
    pwm = constrain( pwm, 0, 4095);

    // If the LED we are trying to change is out of range, exit early
    if ( lednum > _leds )
        return;

    // Set the PWM value for the LED in the buffer
    buffer[lednum] = pwm;
}

// Turn off all of the LEDS on the badge
void UM_Badge_2019::clear()
{
    // Set all LEDs to intensity 0
    setALL( 0 );
}

void UM_Badge_2019::setALL( uint16_t pwm )
{
    // Clamp the intensity between 0 and 4095
    pwm = constrain( pwm, 0, 4095);

    // Set the LEDs to the required intensity
    for(uint8_t i=0; i<_leds; i++)
        setLED(i, pwm);

    // Auto update the LEDs
    updateBadge();
}

void UM_Badge_2019::drawSegment( uint8_t seg , uint16_t v, bool update = false )
{
    // Clamp the intensity between 0 and 4095
    v = constrain( v, 0, 4095);

    // Grab the start and end LED index for the segment
    int s = segments[ seg ][0]; // start of segment
    int e = segments[ seg ][1]; // end of segment

    // Set the LEDs to the required intensity
    for ( int i = s; i <= e; i++ )
      setLED( i, v );
    
    // Auto update the LEDs if update is true
    if ( update )
        updateBadge();
}

void UM_Badge_2019::drawWall( uint8_t w, uint16_t v, bool update = false )
{
    // Clamp the intensity between 0 and 4095
    v = constrain( v, 0, 4095);

    // 4 segments to each wall
    for ( int w_seg = 0; w_seg < 4; w_seg++ )
    {
        // Find the index for the wall segment
        int seg = walls[ w ][ w_seg ];
        // Draw the segment
        drawSegment( seg, v );
    }

    // Auto update the LEDs if update is true
    if ( update )
        updateBadge();
}

void UM_Badge_2019::drawBoxInner( uint16_t v, bool update = false )
{
    // Clamp the intensity between 0 and 4095
    v = constrain( v, 0, 4095);

    // 4 sides to the box
    for ( int b = 0; b < 4; b++ )
    {
        // grab the start and end LED index for the side
        int s = segments[ box_inner[ b ] ][ 0 ]; // start of side
        int e = segments[ box_inner[ b ] ][ 1 ]; // end of side

        // Set the LEDs to the required intensity
        for ( int i = s; i <= e; i++ )
            setLED( i, v );
    }

    // Auto update the LEDs if update is true
    if ( update )
        updateBadge();
}

// Light up the outer box/ring of LEDs
void UM_Badge_2019::drawBoxOuter( uint16_t v, bool update = false )
{
    // Clamp the intensity between 0 and 4095
    v = constrain( v, 0, 4095);

    // 4 sides to the box
    for ( int b = 0; b < 4; b++ )
    {
        
        int s = segments[ box_outer[ b ] ][ 0 ]; // start of side
        int e = segments[ box_outer[ b ] ][ 1 ]; // end of side

         // Set the LEDs to the required intensity
        for ( int i = s; i <= e; i++ )
            setLED( i, v );
    }

    // Auto update the LEDs if update is true
    if ( update )
        updateBadge();
}