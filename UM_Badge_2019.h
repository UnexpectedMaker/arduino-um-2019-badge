/*
 *  UM_BADGE_2019.h
 *
 * 	Driver library for the Unexpected maker 2019 Hackable Badge
 *  https://www.tindie.com/products/seonr/unexpected-maker-badge-2019/
 *
 * 	See examples folder for implementation
 */

#ifndef _UM_BADGE_2019_H
    #define _UM_BADGE_2019_H

    #include <Arduino.h>

    #define _clk 2
    #define _dat 0
    #define _lat 1
    #define _leds 24

    class UM_Badge_2019
    {
        public:
            UM_Badge_2019();

            void setLED(uint8_t lednum, uint16_t pwm);
            void updateBadge();
            void clear();
            void setALL( uint16_t pwm );
            void drawSegment( uint8_t seg , uint16_t v, bool update );
            void drawWall( uint8_t w, uint16_t v, bool update );
            void drawBoxInner( uint16_t v, bool update );
            void drawBoxOuter( uint16_t v, bool update );

            int segments[12][2] = {
                {  0,  1 },
                {  6,  7 },
                {  4,  5 },
                {  2,  3 },
                {  8,  8 },
                {  9,  9 },
                { 10, 10 },
                { 11, 11 },
                { 12, 14 },
                { 15, 17 },
                { 18, 20 },
                { 21, 23 }                 
            };

            int walls [4][4] = {
                { 0, 4, 5, 9 },
                { 1, 5, 6, 10 },
                { 2, 6, 7, 11 },
                { 3, 4, 7, 8 }
            };

            int box_inner[4] = { 0, 1, 2, 3 };
            int box_outer[4] = { 8, 9, 10, 11 };

        private:
            uint16_t *buffer;
    };

#endif
