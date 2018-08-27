var langs = JSON.parse(data);
var mom         = this;
var messages    = [];
var errors      = [];
var SpeechRecognition = window.webkitSpeechRecognition;
var recognition = new SpeechRecognition();

var scenario_id =[];
var language_in ;
var language_out ;
var server;

var pause = false;

for ( i =0; i <sessionStorage.getItem("numb_scenarios"); i++){
    scenario_id[i] = sessionStorage.getItem("scenarios"+i);
}

language_in = sessionStorage.getItem("lang_in");
language_out= sessionStorage.getItem("lang_out");
server= sessionStorage.getItem("srv");
console.log(server);

recognition.lang = "fr-FR";
recognition.continuous = true;

recognition.onstart = function() {
    console.log("Started recognition");
}

recognition.onerror = function(event) {
    errors.push("Started recognition");
}

recognition.onend = function() {
    recognition.start();
}

recognition.onresult = function(event) {

    if(event.results && event.results.length > 0 ) {
        
        var uniqueID = (new Date()).getTime();

        var input  = langs[language_in][1];
        console.log(language_in);
        var	output = langs[language_out][1];

		var transcript = event.results[event.results.length-1][0].transcript;
		console.log(event.results[event.results.length-1][0].confidence);
        if (event.results[event.results.length-1][0].confidence > 0.8) {
			ws.send(uniqueID+"|RAW|"+transcript+"|"+input[0]+"|"+output[0]);
            console.log("from "+input+" to "+ output);
            updateField(transcript);
        
        }else{
        	recognition.lang = output[0];
            recognition.stop();
    
            temp = language_in; 
            language_in = language_out;
            language_out = temp;

            input   = langs[language_in][1];
        	output  = langs[language_out][1];

        	console.log("from "+input+" to "+ output)
            if (input[0]=="fr-FR"){
                ws.send(uniqueID+"|RAW|"+"changement de langue"+"|"+input[0]+"|"+output[0]);
            }else{
                ws.send(uniqueID+"|RAW|"+"changement de langue"+"|"+output[0]+"|"+input[0]);
            }

            updateField("<p style='color:green;'> changement de langue");
        }

        
    }    
}

recognition.start();


function updateField(message) {
    
    messages.push("<p>"+message+"</p>");
    var txt = "";
    for( var i = messages.length - 1; i >=0; i--) {
        txt += messages[i];
        
    }
    $("#transcripts").html(txt);
    
}

// set interval for weird bug
// this will stop the recognition, onend event is shot, then recognition is back again
// ¯\_(ツ)_/¯
setInterval(resetVoiceRecog, 10000);
var t = this;
function resetVoiceRecog() {
    t.recognition.stop();
}
/*$('.btn').click(function(){
      $('.collapse').collapse($(this).attr('id'))
    })*/
$('#pause_bt').on('click', function() {
	if(!pause){
		ws.send("NULL|PAUSE|NULL|NULL|NULL");
		pause = true;
		$(this).css("background-image", "url(img/play.png)");
	} 
	else{
		ws.send("NULL|PLAY|NULL|NULL|NULL");
		pause = false;
		$(this).css("background-image", "url(img/pause.png)");

	} 
});

function setScenario(data){
    console.log(data.id);

    
    if(data.id != "sante" && data.id != "yeux"){
        $('#SanteBoutons').hide();
    }else{
        $('#SanteBoutons').show();
    }
    
    i = 0;
    while(scenario_id[i] != data.id && i<scenario_id.length){
        i++;
    }
    ws.send(i+"|SCENARIO_CHANGE|NULL|NULL|NULL");
    
    
}

function createWebSocket(server) {

    if ("WebSocket" in window) {
        try {
            ws = new WebSocket("ws://" + server + ":9092/");
        } catch (err) {
            $(".lead").html("<p style='color:red;'>Error in WebSocket connection.</p>");
        }
        
        ws.onopen = function (error) {
            $(".lead").html("<p style='color:green;'>Up and running</p>");
            if(window.timerID){
                window.clearInterval(window.timerID);
                window.timerID=0;
            }
        };
        
        ws.onclose = function (error) {
            $(".lead").html("<p style='color:red;'>Websocket has been closed.</p>");
            scenario_id[0] = "Auto";
            if(!window.timerID){
                window.timerID = setInterval(
                    function () {
                        createWebSocket(server);
                    }, 5000);
               }
        };
        
        ws.onerror = function (error) {
            $(".lead").html("<p style='color:red;'>Error in WebSocket connection.</p>");
        };
 
        ws.onmessage = function (evt) {
            var received_msg    = evt.data;
            var splitted        = received_msg.split("|");
            var id              = splitted[0];
            var type            = splitted[1];
            var msg             = splitted[2];
            
            if(type == "TRANS")
                updateField("<p style='color:red;'>"+msg+"</p>");
        };
    
    } else {
        alert("Y a même pas WebSocket sur ce navigateur. C'est fini IE6");
    }
}


createWebSocket(server);

