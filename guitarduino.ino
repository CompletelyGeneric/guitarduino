
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
named according to scientific pitch notation
**/
float E_2 = 82.41;
float A_2 = 110.00;
float D_3 = 146.83;
float G_3 = 196.00;
float B_3 = 246.94;
float E_4 = 329.63;


class GuitarduinoDisplay
{
		Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 8, PIN,
		NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
		NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
		NEO_GRB            + NEO_KHZ800);

 		uint16_t noteBrightness;
		char lastNoteName;
		float pitchOffset;

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
	
	void setPitch(float referenceFreq, float inputFreq)
	{
		pitchOffset = inputFreq - referenceFreq; //if pitchOffset is positive, the note is sharp, if negative, the note is flat
		clearPitch();
		if(pitchOffset > 5.00)
		{
			matrix.setPixelColor(32, (100 + noteBrightness*255/255), 0, 0);

		}
		if(pitchOffset <= 5.00 && pitchOffset > 1.00)
		{
			matrix.setPixelColor(24, (50 + noteBrightness*255/255), (50 + noteBrightness*255/255), (noteBrightness*51/255));
		}
		if(pitchOffset <= 1.00 && pitchOffset >= -1.00)
		{
			matrix.setPixelColor(16,(noteBrightness*50/255), (100 + noteBrightness*255/255), (noteBrightness*50/255));
		}
		if(pitchOffset < -1.00 && pitchOffset >= -5.00)
		{
			matrix.setPixelColor(8, (50 + noteBrightness*255/255), (50 + noteBrightness*255/255), (noteBrightness*51/255));
		}
		if(pitchOffset < -5.00)
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
	display.setNote('B');
	display.setNote('E');
	display.setPitch(E_2, 75.00);
	Serial.begin(115200); // use the serial port
	TIMSK0 = 0; // turn off timer0 for lower jitter
	ADCSRA = 0xe5; // set the adc to free running mode
	ADMUX = 0x40; // use adc0
	DIDR0 = 0x01; // turn off the digital input for adc0 
}


//Took the FHT code from the source below for debugging.
//REMINDER TO REPLACE IT

/*
fht_adc.pde
guest openmusiclabs.com 9.5.12
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/

void loop() 
{
	while(1) 
	{ // reduces jitter
		cli();  // UDRE interrupt slows this way down on arduino1.0
		for (int i = 0 ; i < FHT_N ; i++) 
		{ // save 256 samples
			while(!(ADCSRA & 0x10)); // wait for adc to be ready
			ADCSRA = 0xf5; // restart adc
			byte m = ADCL; // fetch adc data
			byte j = ADCH;
			int k = (j << 8) | m; // form into an int
			k -= 0x0200; // form into a signed int
			k <<= 6; // form into a 16b signed int				
			fht_input[i] = k; // put real data into bins
		}
		fht_window(); // window the data for better frequency response
		fht_reorder(); // reorder the data before doing the fht
		fht_run(); // process the data in the fht
		fht_mag_log(); // take the output of the fht
		sei();
		//Serial.write(255); // send a start byte
		//Serial.write(fht_log_out, FHT_N/2); // send out the data
		
	}
}
