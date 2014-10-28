var five = require("johnny-five"), board, myMotor, myLed;

board = new five.Board();

board.on("ready", function() {

	// LED Control
	myLed = new five.Led(13)
	myLed.on();

	/// REPL /// 
	this.repl.inject({
		led: myLed
	});

	/// LOGGING
	console.log("You can interact with the RGB LED via the variable 'led' e.g. led.on();\n Hit control-D to exit.\n >> ");
});