#pragma once

#include <Backend.hpp>
#include <FastLED.h>

#include "Zone.hpp"






#define LED_PIN     15
#define NUM_LEDS    119  // all count
#define BRIGHTNESS  10
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB



#define UPDATES_PER_SECOND 100


class ZoneMode
{
    private:
    
        ZoneMode() = delete;
        ~ZoneMode() = delete;

    public:

    inline static CRGB leds[NUM_LEDS] = {};

    static void begin()
    {

        FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
        //  FastLED.setCorrection( TypicalLEDStrip );
        FastLED.setBrightness(  BRIGHTNESS );
    }

    static void update()
    {
        for (auto &zone : DB<Zone>::get()) {

            for (int i = zone.from; i < zone.to; i++) {
            switch (zone.mode) {
                case 0:
                leds[i] = CRGB::Black;
                break;
                // case 1:
                //   res->addData("Use global");
                //   break;
                case 2:
                leds[i] = CRGB::Red;
                break;
                case 3:
                leds[i] = (millis()%1000) > 500 ? CRGB::Orange : CRGB::Red;
                break;
                case 4:
                leds[i] = CRGB::Green;
                break;
                default:
                leds[i] = CRGB::White;
                break;
            }

            }

        }

        FastLED.show(); 
    }
};

