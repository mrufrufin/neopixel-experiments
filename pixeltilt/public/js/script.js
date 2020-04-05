let socket = io();
let tiltslider = document.getElementById("tiltslider");
let debug = false;
let disp = document.getElementById("display");

function handleOrientation(evt)
{
    let gamma = evt.gamma;
    if(debug == false)
	{
	    disp.innerHTML = gamma;
	    tiltslider.value = gamma;
	    socket.emit('tilt', gamma);
	};
    
};


tiltslider.onchange = function()
    {
	if(debug)
	    {
		let gamma = tiltslider.value;
		disp.innerHTML = gamma;
		socket.emit('tilt',gamma);
	    };
    };

window.addEventListener("deviceorientation", handleOrientation);

