
/*
 * guitarduino.ino
 *
 * Created: 5/4/2017 8:57:07 PM
 * Author: Hugh McElderry
 */ 



#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#define PIN 6






#include <FHT.h> // include the library
#include <gamma.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>

/**
Define the pitch of the notes we want to tune to,
defined as to where we expect to find the note in FHT's output.
**/
uint16_t E_2 = 24;
uint16_t A_2 = 31;
uint16_t D_3 = 42;
uint16_t G_3 = 56;
uint16_t B_3 = 71;
uint16_t E_4 = 94;


class GuitarduinoDisplay
{
		Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 8, PIN,
		NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
		NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
		NEO_GRB            + NEO_KHZ800);

 		uint16_t noteBrightness;
		char lastNoteName;
		int16_t pitchOffset;

		//Definitions for what pixels need to be on to represent the note we want to display
		uint16_t E[19] = {
							1, 2, 3, 4, 5, 6, 7, 9, 12, 15, 
							17, 20, 23, 25, 28, 31, 33, 36, 
							39
							};
		uint16_t A[16] = {
							1, 2, 3, 4, 5, 11, 14, 19, 23,
							27, 30, 33, 34, 35, 36, 37
							};
		uint16_t D[18] = {
							1, 2, 3, 4, 5, 6, 7, 9, 15, 17,
							23, 25, 31, 34, 35, 36, 37, 38
							};
		uint16_t G[19] = {
							1, 2, 3, 4, 5, 6, 7, 9, 15, 17,
							20, 23, 25, 28, 31, 33, 34, 35, 
							36
							};
		uint16_t B[20] = {
							1, 2, 3, 4, 5, 6, 7, 9, 12, 15,
							17, 20, 23, 25, 28, 31, 34, 35, 
							37, 38
							};	
	
	public:
	GuitarduinoDisplay(uint16_t brightnessFactor)
	{
		noteBrightness = brightnessFactor;
		matrix.begin();
		clearPitch();
	}

	/**
	* Accepts a char and displays it. 
	* NOTE: 'E' == E2, 'e' == E4 
	*/	
	void setNote(char currentNoteName)
	{
		if(currentNoteName != lastNoteName) //Save a few cycles by only updating the note display when the note changes
		{
			clearNote(); //Clear the previous note from the display
			if(currentNoteName == 'E')
			{
				for(int i = 0; i < sizeof(E)/sizeof(E[0]); i++)
				{
					matrix.setPixelColor(E[i], (noteBrightness*255/255), (noteBrightness*255/255), (noteBrightness*255/255));
				}
			}

			if(currentNoteName == 'A')
			{
				for(int i = 0; i < sizeof(A)/sizeof(A[0]); i++)
				{
					matrix.setPixelColor(A[i], (noteBrightness*255/255), (noteBrightness*255/255), (noteBrightness*255/255));
				}
			}

			if(currentNoteName == 'D')
			{
				for(int i = 0; i < sizeof(D)/sizeof(D[0]); i++)
				{
					matrix.setPixelColor(D[i], (noteBrightness*255/255), (noteBrightness*255/255), (noteBrightness*255/255));
				}
			}

			if(currentNoteName == 'G')
			{
				for(int i = 0; i < sizeof(G)/sizeof(G[0]); i++)
				{
					matrix.setPixelColor(G[i], (noteBrightness*255/255), (noteBrightness*255/255), (noteBrightness*255/255));
				}
			}

			if(currentNoteName == 'B')
			{
				for(int i = 0; i < sizeof(B)/sizeof(B[0]); i++)
				{
					matrix.setPixelColor(B[i], (noteBrightness*255/255), (noteBrightness*255/255), (noteBrightness*255/255));
				}
			}
			lastNoteName = currentNoteName;
			update();
		}
	}
	
	/**
	* Takes a reference bin and an input bin and maps the offset how in tune the note is, then displays it
	*/
	void setPitch(uint16_t referenceBin, uint16_t inputBin)
	{
		pitchOffset = inputBin - referenceBin; //if pitchOffset is positive, the note is sharp, if negative, the note is flat
		clearPitch();
		if(pitchOffset > 1)
		{
			matrix.setPixelColor(32, (100 + noteBrightness*255/255), 0, 0);

		}
		if(pitchOffset == 1)
		{
			matrix.setPixelColor(24, (50 + noteBrightness*255/255), (50 + noteBrightness*255/255), (noteBrightness*51/255));
		}
		if(pitchOffset == 0)
		{
			matrix.setPixelColor(16,(noteBrightness*50/255), (100 + noteBrightness*255/255), (noteBrightness*50/255));
		}
		if(pitchOffset == -1)
		{
			matrix.setPixelColor(8, (50 + noteBrightness*255/255), (50 + noteBrightness*255/255), (noteBrightness*51/255));
		}
		if(pitchOffset < -1)
		{
			matrix.setPixelColor(0, (100 + noteBrightness*255/255), 0, 0);
		}
		update();
	}

	private:
	void clearNote() //Clears just the note portion of the display
	{
		for(int i = 1; i < 40; i++)
		{
			if(i % 8 != 0)
			{
				matrix.setPixelColor(i, 0, 0, 0);
			}
		}
	}

	/**
	* helper method to set the pitch display to a default state
	*/
	void clearPitch()
	{
		matrix.setPixelColor(0, (noteBrightness*255/255), 0, 0);
		matrix.setPixelColor(32, (noteBrightness*255/255), 0, 0);
		matrix.setPixelColor(8, (noteBrightness*255/255), (noteBrightness*255/255), (noteBrightness*51/255));
		matrix.setPixelColor(24, (noteBrightness*255/255), (noteBrightness*255/255), (noteBrightness*51/255));
		matrix.setPixelColor(16,(noteBrightness*50/255), (noteBrightness*255/255), (noteBrightness*50/255));
	}

	
	void update()
	{
		matrix.show();
	}	
};

GuitarduinoDisplay display(12); //Instantiate our object and set brightness to ~17%

void setup() 
{
	ADCSRB = 0x08; 
	ADCSRA = 0xe7; // free running mode, and set adc prescaler factor to 128
	ADMUX = 0x40; // use adc8
	DIDR2 = 0x01; // turn off digital input 
}




void loop() 
{
	while(1) 
	{ 
		cli();  
		for (int i = 0 ; i < FHT_N ; i++) 
		{
			ADCSRA = 0xf5; // restart adc
			while(!(ADCSRA & 0x10)); // wait for adc to be ready				
			ADCSRA = 0xf5; // restart adc
			byte m = ADCL; // fetch adc data
			byte j = ADCH;
			int k = (j << 8) | m; // form into an int
			k -= 0x0200; // form into a signed int
			k <<= 6; // form into a 16b signed int
			fht_input[i] = k; // put real data into bins
			delayMicroseconds(1100); //Introduce delay to increase resolution
		}
		fht_window();  //FHT magic
		fht_reorder(); 
		fht_run(); 
		fht_mag_log(); 
		sei();
		int maxAmpIndex = 0;
		int maxAmp = 0;
		for(int i = 2; i < FHT_N/2; i++) //start at index two because of weird noise
		{
			if(fht_log_out[i] > maxAmp)
			{
				maxAmp = fht_log_out[i];
				maxAmpIndex = i;
			}	
		}
		//Logic to figure out what note is being played
		if(maxAmpIndex >= 21 && maxAmpIndex <= 27)
		{
			display.setNote('E');
			display.setPitch(E_2, maxAmpIndex);
		}
		if(maxAmpIndex >= 28 && maxAmpIndex <= 36)
		{
			display.setNote('A');
			display.setPitch(A_2, maxAmpIndex);
		}
		if(maxAmpIndex >= 37 && maxAmpIndex <= 49)
		{
			display.setNote('D');
			display.setPitch(D_3, maxAmpIndex);
		}
		if(maxAmpIndex >= 50 && maxAmpIndex <= 65)
		{
			display.setNote('G');
			display.setPitch(G_3, maxAmpIndex);
		}
		if(maxAmpIndex >= 66 && maxAmpIndex <= 80)
		{
			display.setNote('B');
			display.setPitch(B_3, maxAmpIndex);
		}
		if(maxAmpIndex >= 80 && maxAmpIndex <= 114)
		{
			display.setNote('E');
			display.setPitch(E_4, maxAmpIndex);
		}	
	}
}
