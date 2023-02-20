#ifndef DISPLAY_FUNCTIONS_H_
#define DISPLAY_FUNCTIONS_H_

#define NUM_COLORS 5

static const CRGB TwinkleColors[NUM_COLORS] =

{
		CRGB::Blue,
		CRGB::Red,
		CRGB::Aqua,
		CRGB::Violet,
		CRGB::Yellow
};

//// Gradient palette "Fire_1_gp", originally from
//// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/icons/tn/Fire-1.png.index.html
//// converted for FastLED with gammas (2.6, 2.2, 2.5)
//// Size: 12 bytes of program space.
//
//DEFINE_GRADIENT_PALETTE( Fire_1_gp ) {
//    0, 255,  0,  0,
//  127, 255, 55,  0,
//  255, 255,255,  0};

// Gradient palette "bhw4_025_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_025.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw4_025_gp ) {
    0,  67, 25, 18,
   43, 113,  1,  1,
  124,  19,  8,  5,
  255, 255,197, 63};

//// Gradient palette "bhw2_greenman_gp", originally from
//// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_greenman.png.index.html
//// converted for FastLED with gammas (2.6, 2.2, 2.5)
//// Size: 12 bytes of program space.
//
//DEFINE_GRADIENT_PALETTE( bhw2_greenman_gp ) {
//    0,   1, 22,  1,
//  130,   1,168,  2,
//  255,   1, 22,  1};

// Gradient palette "heartchakra1_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/3/tn/heartchakra1.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( heartchakra1_gp ) {
    0,   1,199,  1,
   51, 121,237,  1,
   84,   1,199,  1,
  109,   8, 60,  9,
  124,   8, 60,  9,
  145,   1,199,  1,
  181,   8, 60,  9,
  229, 103,235,  1,
  255,   9, 95, 18};

// Gradient palette "es_seadreams_11_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/sea_dreams/tn/es_seadreams_11.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_seadreams_11_gp ) {
    0,   1,  1,240,
  255,   1,239,230};

CRGBPalette16 HourPalette = bhw4_025_gp;
CRGBPalette16 MinutesPalette = heartchakra1_gp;
CRGBPalette16 MinutePalette = es_seadreams_11_gp;

void DrawTwinkle() {
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[random(NUM_LEDS)] = TwinkleColors[random(NUM_COLORS)];
		FastLED.show(40);
		delay(5);
		server.handleClient();
	}
}

void pride()
{
	static uint16_t sPseudotime = 0;
	static uint16_t sLastMillis = 0;
	static uint16_t sHue16 = 0;
	uint8_t sat8 = beatsin88(87, 220, 250);
	uint8_t brightdepth = beatsin88(341, 96, 224);
	uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
	uint8_t msmultiplier = beatsin88(147, 23, 60);
	uint16_t hue16 = sHue16; //gHue * 256;
	uint16_t hueinc16 = beatsin88(113, 1, 3000);
	uint16_t ms = millis();
	uint16_t deltams = ms - sLastMillis;
	sLastMillis = ms;
	sPseudotime += deltams * msmultiplier;
	sHue16 += deltams * beatsin88(400, 5, 9);
	uint16_t brightnesstheta16 = sPseudotime;

	for (uint16_t i = 0; i < NUM_LEDS; i++) {
		hue16 += hueinc16;
		uint8_t hue8 = hue16 / 256;
		brightnesstheta16 += brightnessthetainc16;
		uint16_t b16 = sin16(brightnesstheta16) + 32768;
		uint16_t bri16 = (uint32_t)((uint32_t) b16 * (uint32_t) b16) / 65536;
		uint8_t bri8 = (uint32_t)(((uint32_t) bri16) * brightdepth) / 65536;
		bri8 += (255 - brightdepth);
		CRGB newcolor = CHSV(hue8, sat8, bri8);
		uint16_t pixelnumber = i;
		pixelnumber = (NUM_LEDS - 1) - pixelnumber;
		nblend(leds[pixelnumber], newcolor, 64);
	}
}

uint16_t XY(uint8_t x, uint8_t y)
{
	uint16_t i;
	if (y & 0x01) {
		// Odd rows run backwards
		uint8_t reverseX = 11 - x;
		i = (y * 12) + reverseX;
	} else {
		// Even rows run forwards
		i = (y * 12) + x;
	}
	return i;
}

void FillLEDsFromPaletteColors(int colorIndex)
{
	int temp_colorIndex = colorIndex;
	for( int i = 0; i < 10; i++) {
		leds[hour_leds[i]] = ColorFromPalette(HourPalette, temp_colorIndex);
		temp_colorIndex +=25;
	}
	temp_colorIndex = colorIndex;
	for( int i = 0; i < 5; i++) {
		leds[minute_leds[i]] = ColorFromPalette(MinutesPalette, temp_colorIndex);
		temp_colorIndex +=5 ;
	}
	leds[minute_led] = ColorFromPalette(MinutePalette, colorIndex);
	leds[second_led] = ColorFromPalette(RainbowColors_p, colorIndex);
}

void BackgroundDisplay() {
	int x, y;
	for (x = 0; x < 3; x++) {
		for (y = 1; y < 6; y++) {
			leds[XY(x, y)] = CRGB(16, 16, 16);
			leds[XY(x + 6, y)] = CRGB(16, 16, 16);
		}
	}
	for (x = 3; x < 6; x++) {
		for (y = 1; y < 6; y++) {
			leds[XY(x, y)] = CRGB(32, 32, 32);
			leds[XY(x + 6, y)] = CRGB(32, 32, 32);
		}
	}
	for (x = 0; x < 12; x++) {
		leds[XY(x, 0)] = CRGB(128,128,128);
		leds[XY(x, 6)] = CRGB(128,128,128);
	}
}

void TimeDisplay(byte h, byte m, byte s) {
	int x, y;
	int hour_line;
	switch (h) {
	case 1 ... 12:
	hour_line = h - 1;
	break;
	case 13 ... 23:
	hour_line = h - 13;
	break;
	case 0:
		hour_line = 11;
		break;
	}

	for (y = 1; y < 6; y++) {
		hour_leds[y - 1] = XY(hour_line, y);
	}
	if (hour_line == 11) {
		for (y = 1; y < 6; y++) {
			hour_leds[y + 4] = XY(0, y);
		}
	}
	else {
		for (y = 1; y < 6; y++) {
			hour_leds[y + 4] = XY(hour_line + 1, y);
		}
	}

	int min_line = m / 5;
	for (y = 1; y < 6; y++) {
		minute_leds[y - 1] = XY(min_line, y);
	}

	int min = m % 5;
	minute_led = XY(min_line, min + 1);

	int sec = s / 5;
	int sec_point = s % 5;
	leds[second_led] = second_lastcolor;
	second_led = XY(sec, sec_point + 1);
	second_lastcolor = leds[second_led];
}

#endif /* DISPLAY_FUNCTIONS_H_ */
