var langs = JSON.parse(data);
var mom         = this;
var errors      = [];
var scenario_id =[];
var serveur="localhost";

//commencere la consultation
$('#start').on('click', function() {
    sessionStorage.setItem("numb_scenarios",scenario_id.length);
    for ( i =0; i <scenario_id.length; i++){
    	sessionStorage.setItem("scenarios"+i,scenario_id[i]);
    }

    sessionStorage.setItem("lang_in",language_in);
    sessionStorage.setItem("lang_out",language_out);
    sessionStorage.setItem("srv",serveur)
})

//choisir la langue 1
$('#select_language').on('change', function() {
    language_in = select_language.selectedIndex;
    console.log(scenario_id);

})

//choisir la langue 2
$('#translate_language').on('change', function() {
    language_out = translate_language.selectedIndex;
})


function createWebSocket(server) {

    /*for (i = scenario_id.options.length; i > 0; i--) {
        scenario_id.options[i] = null;
    }*/
    scenario_id[0] = "Auto";
    //scenario_id.selectedIndex = 0;
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
            //$("#scenario_id").empty();
            scenario_id[0] = "Auto";
            if(!window.timerID){
                window.timerID = setInterval(
                    function () {
                        createWebSocket(
                            document.getElementById("ip_selection").value
                        );
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
            if (type == "SCENARIO_DATA")
                scenario_id[parseInt(id) + 1] = msg;
        };
    
    } else {
        alert("Y a même pas WebSocket sur ce navigateur. C'est fini IE6");
    }
}

$('#ip_selection').on('change', function() {
    createWebSocket(this.value);
    serveur=this.value;
});

$('#refresh_bt').on('click', function() {
    createWebSocket(
        document.getElementById("ip_selection").value
    );
    serveur = document.getElementById("ip_selection").value;
});

createWebSocket("localhost");

if (typeof select_language !== 'undefined') {
    for (var i = 0; i < langs.length; i++) {
        select_language.options[i] = new Option(langs[i][0], i);
    }

    for (var i = 0; i < langs.length; i++) {
        translate_language.options[i] = new Option(langs[i][0], i);
    }
    select_language.selectedIndex       = 14;
    translate_language.selectedIndex    = 11;

    var language_in = select_language.selectedIndex;
    var language_out = translate_language.selectedIndex;
}

