var io = require('socket.io').listen(4321);
var sys = require('sys')
var exec = require('child_process').exec;
var x = 120;
var y = 150;
var laser = 0;
function puts(error, stdout, stderr) {sys.puts(stdout)}
// setup pin 11 for laser output
exec("echo 11 > /sys/class/gpio/export", puts);
exec("echo out > /sys/class/gpio/gpio11/direction", puts);

io.sockets.on('connection', function (socket) {
  socket.emit('connected');
  //Listen for events from user and then execute function from
  //Below
  socket.on('Right',  Moveright );
  socket.on('Left',  Moveleft );
  socket.on('Up',  Moveup );
  socket.on('Down',  Movedown );
  socket.on('Laser',  ToggleLaser );
});

function Moveleft() {
	if (x > 82) {
		x = x - 1;
	}
	exec("echo 6=" + x + " > /dev/servoblaster", puts);
	console.log("moved left");
}

function Moveright() {
	if (x < 182) {
		x = x + 1;
	}
	exec("echo 6=" + x + " > /dev/servoblaster", puts);
	console.log("moved right");
}

function Moveup() {
	if (y < 230) {
		y = y + 1;
	}
	exec("echo 5=" + y + " > /dev/servoblaster", puts);
	console.log("moved up");
}

function Movedown() {
	if (y > 130) {
		y = y - 1;
	}
	exec("echo 5=" + y + " > /dev/servoblaster", puts);
	console.log("moved down");
}

function ToggleLaser() {
	if (laser == 1) {
		laser = 0;
		exec("echo 0 > /sys/class/gpio/gpio11/value", puts);
		console.log("Turned off laser");
	}
	else {
		laser = 1;
		exec("echo 1 > /sys/class/gpio/gpio11/value", puts);
		console.log("Turned on laser");
	}
}
