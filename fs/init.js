// Load Mongoose OS API
load('api_timer.js');
load('api_gpio.js');
load('api_tft.js');


//LED Status Lights
let RED_LED = 2;

GPIO.set_mode(RED_LED, GPIO.MODE_OUTPUT);

// Primary loop run every 2nd second printing current datetime to TFT.
Timer.set(2000, 1, function() {

	let state = GPIO.toggle(RED_LED);
	print("state = ", state);
	TFT.PrintDate(32, 205, 5);

}, null);
