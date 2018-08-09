
$(function(){

	var doc = $(document),
		win = $(window),
		canvas = $('#paper'),
		context = canvas[0].getContext('2d');
		instructions = $('#instructions');

  	context.canvas.height = window.innerHeight -170;

  	canvas[0].width = canvas[0].offsetWidth;
	canvas[0].height = canvas[0].offsetHeight;

	var drawing = false;
	var cursor = {};
	var prev = {};

	//initialize drawing when the user touches the drawing area
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

});

var category = null;
var data = null;

$('#defCat').on('show.bs.modal', function (e) {
  $("#buttonsCategories").modal('hide');	
})

/*
	Lauches the script with the drawing to propose a category to the user.
*/
function analyseImage (data){
 	
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

	if (category == null || category ==""){
		socket.emit('selectCat', {'image':data, 'category':category});
		
		socket.on('categoryPrediction', function(cat){
			$("#load").modal('hide');	
			category = cat.category;
			$("#conf").html("Enregistrer le dessin dans la catégorie "+cat.category+" ?");
			$("#confSave").modal();	
 		});
 	}
}

/*
	Saves drawing in the folder according to its category.
*/
function saveImage(data){

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
	//result of the saving (confirmation or error)
	socket.on('confMsg', function(msg){
		$("#message").html(msg.message);
		$('#confMsg').modal();
 	});

 	//reset canvas
	context.clearRect(0, 0, canvas[0].width, canvas[0].height);
	$('#instructions').fadeIn();
	category = null;
				
}
^
/*
	Gets back to the beginning.
*/
function notSave(data){

	category = null;
	$("#buttonsCategories").modal('hide');
			
}

/*
	Gets the the name of a new category written by the user and asks him to confirm it. 
*/
function setOther(obj){

	category = $("#category-text").val();
	$("#category-text").val("");
	category = category.replace(/[\s]+$/gi,"")
	
	if (category !=null && category !=""){
		//clean the word to get rid of the accents and the plural mark
		category = category.substring(0,1).toUpperCase()+category.substring(1).toLowerCase();
		category = category.replace(/[èéêë]/gi,"e");
		category = category.replace(/[s]$/gi, "" );
		category = category.replace(/[âà]/gi,"a");
		category = category.replace(/[ù]/gi,"u");
		category = category.replace(/[ç]/gi,"c");
		$("#conf").html("Enregistrer le dessin dans la catégorie "+category+" ?");
		$("#confSave").modal();
	}else{
		//If the user didn't write anything sends an error message
		$("#message").html("Impossible d'enregistrer le dessin sans catégorie");
		$('#confMsg').modal();
	}
}

/*
	Gets the category from the selected buton and asks the user to confirm his choice.
*/
function setCategory(obj){

	$("#buttonsCategories").modal('hide');
   	category = document.getElementById(obj.id).value;
   	$("#conf").html("Enregistrer le dessin en tant que "+category+" ?");
	$("#confSave").modal();
	
}

document.getElementById('clear').addEventListener('click', function() {

	var canvas=$('#paper'),
  		context = canvas[0].getContext('2d');

  	context.clearRect(0, 0, canvas[0].width, canvas[0].height);
	$('#instructions').fadeIn();
	category = null;

}, false);
