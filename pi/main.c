
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPiI2C.h>
#include <string.h>
#include <time.h>
#include <softPwm.h>

#define MAXTIMINGS 85
#define DHTPIN 0 // Pin to temp sensor (17)
#define uchar unsigned char
#define LedPinRed    2 // Pins to LEDs (27)
#define LedPinGreen  1 // (18)
#define LedPinBlue 3 // (22)
#define BtnPin 4 // (23)
#define BtnPin2 5 // (23)

// Prototype Methods
int read_temp();
void write_word(int data);
void send_command(int comm);
void send_data(int data);
void init();
void clear();
void write(int x, int y, char data[]);
int* getCurrDate();
void correctDate(int* year, int* month, int* day);
int checkSameDates(int y1, int m1, int d1, int y2, int m2, int d2);
void ledInit(void);
void ledColorSet(uchar r_val, uchar g_val, uchar b_val);
void halt(int type);
void makeEventCall(int id, char * event);
void initializeAll();
void waitForRestart();
void waitForStart();
void startTracking();

///// Temp Sensor /////
int dht11_dat[5] = {0,0,0,0,0};
int initialized = 0;
int curr_year, curr_month, curr_day;
int start_year, start_month, start_day;
int twoWeek_year, twoWeek_month, twoWeek_day;
int exp_year, exp_month, exp_day;
int hitRoomTemp = 0;
int displayText = 1;
int restart = 1;
int medID = 0;

int read_temp() {
	
	if(initialized == 0) {
	//	int * date = getCurrDate();
	//	curr_year = *date;
	//	curr_month = *(date + 1);
	//	curr_day = *(date + 2);
		
		curr_year = 2017;
		curr_month = 6;
		curr_day = 23;	
			
		exp_year=2019; exp_month=7; exp_day=9;
		initialized = 1;
		makeEventCall(medID, "COUNTDOWN_STARTED");
	}
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	float f; // fahrenheit
		
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	// pull pin down for 18 milliseconds
	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, LOW);
	delay(18);
	// then pull it up for 40 microseconds
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40); 
	// prepare to read the pin
	pinMode(DHTPIN, INPUT);

	// detect change and read data
	for (i = 0; i < MAXTIMINGS; i++) {
		counter = 0;
		while (digitalRead(DHTPIN) == laststate) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) break;
		}
		laststate = digitalRead(DHTPIN);

		if (counter == 255) break;

		// ignore first 3 transitions
		if ((i >= 4) && (i%2 == 0)) {
			// shove each bit into the storage bytes
			dht11_dat[j/8] <<= 1;
			if (counter > 16) dht11_dat[j/8] |= 1;
			j++;
		}
	}

	// check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	// print it out if data is good
	char firstLine[64];
	char secondLine[64];
	if ((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)) ) {
		// Get temp in F
		f = dht11_dat[2] * 9. / 5. + 32;
		
		// Check for extremes / dates
		if(f > 46 && f <= 100) { // Hits room temp CHANGE TO 77
			if(hitRoomTemp == 0) {
				hitRoomTemp = 1;
				start_year = curr_year;
				start_month = curr_month; 
				start_day = curr_day;
					
				twoWeek_year = start_year;
				twoWeek_month = start_month;
				twoWeek_day = start_day + 14; ///////////?CHANGE TO 14//////
				correctDate(&twoWeek_year, &twoWeek_month, &twoWeek_day);
			}
		} else if (f < 36 || f > 100) { // Hits extreme
			halt(1); return 0;
		}
		if(hitRoomTemp == 1) {
			if(checkSameDates(curr_year, curr_month, curr_day, 
			twoWeek_year, twoWeek_month, twoWeek_day) == 1)  {
				halt(2); return 0;
			}
		}
		
		if(checkSameDates(curr_year, curr_month, curr_day, 
		exp_year, exp_month, exp_day) == 1) {
			halt(2); return 0;
		}

		ledColorSet(0x94,0x00,0xd3);  //green
	
		// Display onto LCD
		// First Line
		char temp[64];
		snprintf(temp, sizeof temp, "%.02f", f);
		
		strcpy(firstLine, "Temp: ");
		strcat(firstLine, temp);
		strcat(firstLine, " F");
		
		// Second Line
		int val1 = curr_year * 365 + curr_month * 30 + curr_day;
		int val2 = exp_year * 365 + exp_month * 30 + exp_day;
		int val3 = twoWeek_year * 365 + twoWeek_month * 30 + twoWeek_day;
		int diff1 = val2 - val1;
		int diff2 = val3 - val1;
		int diff;
		if(diff1 >= diff2) {diff = diff2;}
		else {diff = diff1;}
		
		char daysLeft[64];
		snprintf(daysLeft, sizeof daysLeft, "%.02d", diff);
		
		strcpy(secondLine, daysLeft);
		strcat(secondLine, " days left");
	} else {
		// Display onto LCD
		strcpy(firstLine, "Temp: -- F");
		strcpy(secondLine, "-- days left");
	}
	
	clear();
	write(0, 0, firstLine);
	write(0, 1, secondLine);	
	
	curr_day += 1;
	correctDate(&curr_year, &curr_month, &curr_day);
			
	printf("Start: %d %d %d\n", start_year, start_month, start_day);
	printf("Curr: %d %d %d\n", curr_year, curr_month, curr_day);
	printf("Exp: %d %d %d\n", exp_year, exp_month, exp_day);
	printf("2 Weeks: %d %d %d\n", twoWeek_year, twoWeek_month, twoWeek_day);
	printf("Temperature = %.1f *F\n\n", f);
	
	return 1;
}
int checkSameDates(int y1, int m1, int d1, int y2, int m2, int d2) {
	if(y1 == y2 && m1 == m2 && d1 == d2) {
		return 1;
	} else if(y1 == y2 && m1 == m2 && d1 > d2) {
		return 1;
	} else if(y1 == y2 && m1 > m2) {
		return 1;
	} else if(y1 > y2) {
		return 1;
	} else {
		return 0;
	}
}
int* getCurrDate() {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int * date = (int*)malloc(3 * sizeof(int));
	date[0] = tm.tm_year + 1900;
	date[1] = tm.tm_mon + 1;
	date[2] = tm.tm_mday;
	return date;
}
void correctDate(int* year, int* month, int* day) {
	if(*day > 30) {	
		*day = *day - 30;
		*month += 1;
	}
	if(*month > 12) {
		*month = 1;
		*year += 1;
	}		
}
void halt(int type) {
	// 1 - Extreme Temp; 2 - Expired
	clear();
	ledColorSet(0x00,0xc5,0xcd);  //red
	if(type == 1) {
		write(0, 0, "ERROR");
		write(0, 1, "Extreme Temp");
		// STORAGE_EXCEPTION
		makeEventCall(medID,"STORAGE_EXCEPTION");
	} else if(type == 2) {
		write(0, 0, "ERROR");
		write(0, 1, "Expired");
		// EXPIRED
		makeEventCall(medID,"EXPIRED");
	}
}
void makeEventCall(int id, char * event) {
	char * base_script = "./scripts/register_event.sh";
	char id_ext[64], year[64], month[64], day[64];
	snprintf(id_ext, sizeof id_ext, "%d", id);
	snprintf(year, sizeof year, "%d", curr_year);
	snprintf(month, sizeof month, "%02d", curr_month);
	snprintf(day, sizeof day, "%02d", curr_day);

	char full_script[64];
	strcpy(full_script, base_script);
	strcat(full_script, " ");
	strcat(full_script, id_ext);
	strcat(full_script, " ");
	strcat(full_script, event);
	strcat(full_script, " ");
	strcat(full_script, year);
	strcat(full_script, "-");
	strcat(full_script, month);
	strcat(full_script, "-");
	strcat(full_script, day);
	strcat(full_script, "T12:00:00.000Z");
	//printf(full_script);
	
	system(full_script);
}

///// LCD Display /////
int LCDAddr = 0x27;
int BLEN = 1;
int fd;
void write_word(int data){
	int temp = data;
	if ( BLEN == 1 )
		temp |= 0x08;
	else
		temp &= 0xF7;
	wiringPiI2CWrite(fd, temp);
}
void send_command(int comm){
	int buf;
	// Send bit7-4 firstly
	buf = comm & 0xF0;
	buf |= 0x04;			// RS = 0, RW = 0, EN = 1
	write_word(buf);
	delay(2);
	buf &= 0xFB;			// Make EN = 0
	write_word(buf);

	// Send bit3-0 secondly
	buf = (comm & 0x0F) << 4;
	buf |= 0x04;			// RS = 0, RW = 0, EN = 1
	write_word(buf);
	delay(2);
	buf &= 0xFB;			// Make EN = 0
	write_word(buf);
}
void send_data(int data){
	int buf;
	// Send bit7-4 firstly
	buf = data & 0xF0;
	buf |= 0x05;			// RS = 1, RW = 0, EN = 1
	write_word(buf);
	delay(2);
	buf &= 0xFB;			// Make EN = 0
	write_word(buf);

	// Send bit3-0 secondly
	buf = (data & 0x0F) << 4;
	buf |= 0x05;			// RS = 1, RW = 0, EN = 1
	write_word(buf);
	delay(2);
	buf &= 0xFB;			// Make EN = 0
	write_word(buf);
}
void init(){
	send_command(0x33);	// Must initialize to 8-line mode at first
	delay(5);
	send_command(0x32);	// Then initialize to 4-line mode
	delay(5);
	send_command(0x28);	// 2 Lines & 5*7 dots
	delay(5);
	send_command(0x0C);	// Enable display without cursor
	delay(5);
	send_command(0x01);	// Clear Screen
	wiringPiI2CWrite(fd, 0x08);
}
void clear(){
	send_command(0x01);	//clear Screen
}
void write(int x, int y, char data[]){
	int addr, i;
	int tmp;
	if (x < 0)  x = 0;
	if (x > 15) x = 15;
	if (y < 0)  y = 0;
	if (y > 1)  y = 1;

	// Move cursor
	addr = 0x80 + 0x40 * y + x;
	send_command(addr);
	
	tmp = strlen(data);
	for (i = 0; i < tmp; i++){
		send_data(data[i]);
	}
}

///// LEDs /////
void ledInit(void) {
	softPwmCreate(LedPinRed,  0, 100);
	softPwmCreate(LedPinGreen,0, 100);
	softPwmCreate(LedPinBlue, 0, 100);
}
void ledColorSet(uchar r_val, uchar g_val, uchar b_val) {
	softPwmWrite(LedPinRed,   r_val);
	softPwmWrite(LedPinGreen, g_val);
	softPwmWrite(LedPinBlue,  b_val);
}
void addMeds(int id) {
	write(0, 0, "Added        ");
	write(0, 1, "Medication   ");
	char * base_script = "./scripts/register_medication.sh";
	char id_ext[64];
	snprintf(id_ext, sizeof id_ext, "%d", id);
	
	char full_script[64];
	strcpy(full_script, base_script);
	strcat(full_script, " ");
	strcat(full_script, id_ext);
//	printf(full_script);
	
	system(full_script);
}
void initializeAll() {
	init();
	initialized = 0;
	hitRoomTemp = 0;
	clear();
}
void waitForStart() {
	write(0, 0, "Waiting For");
	write(0, 1, "Medication");
	while (1) {
		if(digitalRead(BtnPin) == 0){
			delay(50);
			if(digitalRead(BtnPin) == 0) {
				medID += 1;
				addMeds(medID);
				break;
			}
		}	
	}
}
void waitForRestart() {
	while(1) {
		if(digitalRead(BtnPin2) == 0) {
			delay(1000);
			if(digitalRead(BtnPin2) == 0){
				restart = 1;
				printf("\nRESTARTING\n");
				write(0, 0, "RESTARTING");
				write(0, 1, "");
				break;
			}
		}
	}
}
void startTracking() {
	while(1) {
		if(read_temp() == 0) break;
		delay(1000); // wait 1sec to refresh (CHANGE TO 4 SEC)
	}
}

int main (void) {
	while(restart == 1) {
		restart = 0;
		///// Temp Sensor /////
		if (wiringPiSetup () == -1) exit (1);
			
		///// LCD Display /////
		fd = wiringPiI2CSetup(LCDAddr);
		initializeAll();
		
		///// LEDs /////
		ledInit();
		
		waitForStart();
		startTracking();
		waitForRestart();
	}
	return 0;
	 
}
