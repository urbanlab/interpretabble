
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
	//Hide the buttons of the categories
	$(".col-md-3").hide();


});

var category = null;
var data = null;

function saveImage (data){

	//$("#conf").html("Analyse du dessin...");
	$("#load").modal();	
	
	var url = location.host,
  		socket = io.connect(url),
  		canvas=$('#paper'),
  		context = canvas[0].getContext('2d');

    //set and fill background to white
	context.globalCompositeOperation = "destination-over";
	context.fillStyle = 'white';
	context.fillRect(0,0,canvas[0].width,canvas[0].height);

  	//prepare image to send its Url to the server
  	var dl = canvas[0].toDataURL('image/jpeg');
	data = dl.replace(/^data:image\/\w+;base64,/, "");

	//Define images's category through the prediction
	if (category == null || category ==""){
		socket.emit('selectCat', {'image':data, 'category':category});
		socket.on('categoryPrediction', function(cat){
			$("#conf").html("Enregistrer le dessin en tant que "+cat.category+" ?");
			category = cat.category;
			$("#confSave").modal();	

		});
	}
	
}

function save(data){
	var url = location.host,
  		socket = io.connect(url),
  		canvas=$('#paper'),
  		context = canvas[0].getContext('2d');

    //set and fill background to white
	context.globalCompositeOperation = "destination-over";
	context.fillStyle = 'white';
	context.fillRect(0,0,canvas[0].width,canvas[0].height);

  	//prepare image to send its Url to the server
  	var dl = canvas[0].toDataURL('image/jpeg');
	data = dl.replace(/^data:image\/\w+;base64,/, "");
			
	socket.emit('sImage', {'image':data, 'category':category});
	socket.on('confMsg', function(msg){
		$("#message").html(msg.message);
		$('#confirmation').modal();

	});

	//clear canvas and buttons
	context.clearRect(0, 0, canvas[0].width, canvas[0].height);
	$('#instructions').fadeIn();
	category = null;
	$(".col-md-3").hide();					
}

function notSave(data){
	category = null;
	$(".col-md-3").show();		
}

function setOther(obj){
	category = $("#category-text").val();
	if (category !=null && category !=""){
		category = category.substring(0,1).toUpperCase()+category.substring(1).toLowerCase();
		$("#conf").html("Enregistrer le dessin en tant que "+category+" ?");
		save();
		
	}else{
		$("#message").html("Impossible d'enregistrer le dessin sans catégorie");
		$('#confirmation').modal();
		
	}
	
}
function setCategory(obj){
   	category = document.getElementById(obj.id).value;
	$("#conf").html("Enregistrer le dessin en tant que "+category+" ?");
	$("#confSave").modal();
	
}

document.getElementById('clear').addEventListener('click', function() {
	var canvas=$('#paper'),
  		context = canvas[0].getContext('2d');
  	context.clearRect(0, 0, canvas[0].width, canvas[0].height);
	$('#instructions').fadeIn();
	if(category!=null) document.getElementById(category).disabled = false;
	else category = null;
	$(".col-md-3").hide();
}, false);
