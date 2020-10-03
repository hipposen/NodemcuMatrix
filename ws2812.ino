

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel((i+j) & 255));
    }
    strip->show();
    delay(wait);
     client.loop();
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(((i * 256 / strip->numPixels()) + j) & 255));
    }
    strip->show();
    delay(wait);
     client.loop();
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip->numPixels(); i=i+3) {
        strip->setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip->show();

      delay(wait);
       client.loop();

      for (uint16_t i=0; i < strip->numPixels(); i=i+3) {
        strip->setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip->numPixels(); i=i+3) {
        strip->setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip->show();

      delay(wait);
       client.loop();

      for (uint16_t i=0; i < strip->numPixels(); i=i+3) {
        strip->setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip->numPixels(); i++) {
    strip->setPixelColor(i, c);
    strip->show();
    delay(wait);
     client.loop();
  }
}

void color(uint32_t c) {
  for(uint16_t i=0; i<strip->numPixels(); i++) {
    strip->setPixelColor(i, c);
    client.loop();
  }
   strip->show();
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void kitt()
{
  int NUMPIXELS=strip->numPixels();
  
  strip->clear();
  midpix=NUMPIXELS/2;
 
  mixpixp=midpix;
  mixpixn=midpix-1;

    for(int i=0;i<=midpix;i++)
    {

          strip->setPixelColor(mixpixp, strip->Color(177,0,10)); // Moderately bright green color.
          strip->setPixelColor(mixpixn, strip->Color(177,0,10)); // Moderately bright green color.
          mixpixn-=1;
          mixpixp+=1;

          strip->show();
          delay(150);
    }
    mixpixn=0;
    mixpixp=NUMPIXELS;
    for(int i=0;i<=midpix;i++)
    {
          strip->setPixelColor(mixpixp, strip->Color(redVal,greVal,bluVal)); // Moderately bright green color.
          strip->setPixelColor(mixpixn, strip->Color(redVal,greVal,bluVal)); // Moderately bright green color.
          mixpixn+=1;
          mixpixp-=1;
          strip->show();
          delay(150);
    }

}


void kitt2()
{
  int NUMPIXELS=strip->numPixels();
  
  strip->clear();
  midpix=NUMPIXELS/2;
 
  mixpixp=midpix;
  mixpixn=midpix-1;

    for(int i=0;i<=midpix;i++)
    {

          strip->setPixelColor(mixpixp, strip->Color(10,177,10)); // Moderately bright green color.
          strip->setPixelColor(mixpixn, strip->Color(10,177,10)); // Moderately bright green color.
          mixpixn-=1;
          mixpixp+=1;

          strip->show();
          delay(150);
    }
    mixpixn=0;
    mixpixp=NUMPIXELS;
    for(int i=0;i<=midpix;i++)
    {
          strip->setPixelColor(mixpixp, strip->Color(redVal,greVal,bluVal)); // Moderately bright green color.
          strip->setPixelColor(mixpixn, strip->Color(redVal,greVal,bluVal)); // Moderately bright green color.
          mixpixn+=1;
          mixpixp-=1;
          strip->show();
          delay(150);
    }

}
