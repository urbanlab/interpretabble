
$(function(){
	var doc = $(document),
		win = $(window),
		canvas = $('#paper'),
		context = canvas[0].getContext('2d');
		instructions = $('#instructions');

	canvas[0].width = canvas[0].offsetWidth;
	canvas[0].height = canvas[0].offsetHeight;

	var drawing = false;
	var cursor = {};
	var prev = {};



	document.getElementById("paper").addEventListener('touchstart',function(e){
		e.preventDefault();
	//	alert(e.changedTouches[0].pageX+"  "+ canvas.offset().left );
		prev.x = e.changedTouches[0].pageX - canvas.offset().left ;
		prev.y = e.changedTouches[0].pageY - canvas.offset().top;

		drawing=true;

		instructions.fadeOut();
	});

	doc.bind('touchend',function(){
		drawing = false;
	});

	document.getElementById("paper").addEventListener('touchmove',function(e){


		//readjustement of the x and y (because of the margings)
		var realX = e.changedTouches[0].pageX-canvas.offset().left,
			realY = e.changedTouches[0].pageY-canvas.offset().top;
		if(drawing){

			drawLine(prev.x, prev.y, realX, realY);
			prev.x = realX;
			prev.y = realY;
		}
	});

	function drawLine(fromx, fromy, tox, toy){
    context.beginPath();
		context.moveTo(fromx, fromy);
		context.lineTo(tox, toy);
    context.lineWidth= 5;
    context.lineCap='round';
		context.stroke();
	}
});

var category;

function saveImage (data){
  var url = 'http://192.168.81.35:8080',
      socket = io.connect(url);
  if (category==null){
    alert("Vous devez indiquer la catégorie du dessin");
  }else{
    var canvas=$('#paper'),
      context = canvas[0].getContext('2d');
    if(confirm("Enregistrer le dessin en tant que "+ category+" ?")){
			//set and fill background to white
			context.globalCompositeOperation = "destination-over";
			context.fillStyle = 'white';
			context.fillRect(0,0,canvas[0].width,canvas[0].height);

      //send image Url to server
      var dl = canvas[0].toDataURL('image/jpeg');
			data = dl.replace(/^data:image\/\w+;base64,/, "");
      socket.emit('sImage', {'image':data, 'category':category});
		  socket.on('confMsg', function(msg){
				alert(msg.message);
			});

      //clear canvas and buttons
      context.clearRect(0, 0, canvas[0].width, canvas[0].height);
      document.getElementById(category).disabled = false;
			$('#instructions').fadeIn();
      category=null;
    }
  }
}

function setCategory(obj){
   category = document.getElementById(obj.id).value;
   document.getElementById("Ambulance").disabled = false;
   document.getElementById("Maison").disabled = false;
   document.getElementById("Fleche").disabled = false;
   document.getElementById("Biberon").disabled = false;
   document.getElementById("Lit").disabled = false;
   document.getElementById("Bouche").disabled = false;
   document.getElementById("Lune").disabled = false;
	 document.getElementById("Rien").disabled = false;
   document.getElementById(obj.id).disabled = true;
}

document.getElementById('clear').addEventListener('click', function() {
  var canvas=$('#paper'),
		context = canvas[0].getContext('2d');
  context.clearRect(0, 0, canvas[0].width, canvas[0].height);
	$('#instructions').fadeIn();
	if(category!=null) document.getElementById(category).disabled = false;
	category=null;

}, false);
