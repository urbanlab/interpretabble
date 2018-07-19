// Including libraries
const http = require('http');
const fs =require('fs');
const staticServ = require('node-static');

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

var i=100;
io.sockets.on('connection', function (socket) {
  socket.on('sImage', function(data){
    var buf = new Buffer(data.image, 'base64'),
      date = new Date(),
      name = date.getFullYear()+date.getMonth().toString().replace(/^(\d)$/,'0$1')+date.getDate().toString().replace(/^(\d)$/,'0$1'),
      path = "./public/Data/"+data.category+"/"+name+i.toString().replace(/^(\d)$/,'0$1')+".jpg",
      exist = statPath(path);

    //verify file's existence
    while(exist && exist.isFile()) {
      i = i+1;
      path = "./public/"+data.category+"/"+name+i.toString().replace(/^(\d)$/,'0$1')+".jpg";
      exist = statPath(path);
    }
    try{
      fs.writeFile(path, buf);
      i = i+1;
      socket.emit('confMsg',{'message' :"Le dessin a bien été enregistré en tant que " + data.category});
    }catch(err){
      socket.emit('confMsg',{'message' :"Impossible d'enregistrer le dessin"});
      console.log(err);
    }
  })
});
