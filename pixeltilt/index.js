const SerialPort = require('serialport');
const express = require('express');
const path = require('path');
const app = express();
const server = require('http').createServer(app);
let io = require('socket.io')(server);
let portpath = "/dev/ttyUSB0";
const port = new SerialPort(portpath, {baudRate: 28800});

let cnx = {};


app.use(express.static(path.join(__dirname, 'public')));

function rangemap(inval)
{
    let in_lo = -50.0;
    let in_hi = 50.0;
    let out_lo = 0.0;
    let out_hi = 255.0;

    let clipped = inval > in_hi  ? in_hi : (inval < in_lo ? in_lo : inval);
    let retval = (clipped - in_lo) * (out_hi-out_lo)/(in_hi-in_lo);

    return retval;
}



app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.get('js/script.js', (req,res) => {
  res.sendFile(path.join(__dirname, '/public/js/script.js'));
    });

server.listen(8080);
io.sockets.on('connection',
	      (socket) =>
	      {
		  cnx[socket.id] = socket;

		  socket.on("tilt", (data) =>
			    {
				let cur = parseFloat(data);
				cur = -1.0 * Math.round(rangemap(cur));
				//console.log(cur);
				port.write(Buffer.from([cur]));			
				});

		  socket.on('disconnect',
			    () =>
			    {
				delete cnx[socket.id];
			    });
	      });	  
