// Including libraries
const http = require('http');
const fs =require('fs');
const staticServ = require('node-static');
const spawn = require("child_process").spawn;


var server = http.createServer(function (request, response) {
	request.addListener('end', function () {
        fileServer.serve(request, response);
    }).resume();
}),
	io=require('socket.io').listen(server),
	fileServer = new staticServ.Server('./');

server.listen(8080);

function statPath(path) {
  try {
    return fs.statSync(path);
  } catch (ex) {}
  return false;
}

var i=0;
io.sockets.on('connection', function (socket) {
	socket.on('selectCat', function(data){
	 	var buf = new Buffer(data.image, 'base64');

	    fs.writeFile("./public/output.jpg", buf,function(err){
	    	if(err) throw(err);
	    });

	    var pythonProcess = spawn("python2", ["../Translations/prediction/scripts/label_image.py", "/Users/erasme/Desktop/interpretabble/Interpretabble_training/public/output.jpg"]);
    
		pythonProcess.stdout.on('data', (data) =>{
			var rep = data.toString();
			rep = rep.replace(/[\n]/gi, "" )
			socket.emit('categoryPrediction',{'category' : rep});
		});

		/*pythonProcess.stderr.on('data', function(data){
			console.log("err " + data);
		});*/
	});

	socket.on('sImage', function(data){
	  	var buf = new Buffer(data.image, 'base64');

		var date = new Date(),
	    	name = date.getFullYear()+date.getMonth().toString().replace(/^(\d)$/,'0$1')+date.getDate().toString().replace(/^(\d)$/,'0$1'),
	    	path = "./public/Data/"+data.category,
	    	//path = "../Translations/prediction/tf_files/drawings/"+data.category,
	    	exist = statPath(path);

	    //create category's folder
	    if (exist == false){
	    	fs.mkdirSync(path);
	    	path = path +"/"+name+i.toString().replace(/^(\d)$/,'0$1')+".jpg";
	    }else{
	    	path = path +"/"+name+i.toString().replace(/^(\d)$/,'0$1')+".jpg";
	    	exist = statPath(path);
		    //verify file's existence
    		while(exist && exist.isFile()) {
		      i = i+1;
		      path = "./public/Data/"+data.category+"/"+name+i.toString().replace(/^(\d)$/,'0$1')+".jpg";
		      //path = "../Translations/prediction/tf_files/drawings/"+data.category+"/"+name+i.toString().replace(/^(\d)$/,'0$1')+".jpg",
		      exist = statPath(path);
		    }
	    }
	    
	    fs.writeFile(path, buf, function(err){
	    	if(err){
	    		socket.emit('confMsg',{'message' :"Impossible d'enregistrer le dessin en tant que " + data.category});
	    		console.log(err);
	    		throw(err);
	    	}else{
	    		i = i+1;
	    		socket.emit('confMsg',{'message' :"Le dessin a bien été enregistré en tant que " + data.category});
	    	}
	    });   
  	})
});