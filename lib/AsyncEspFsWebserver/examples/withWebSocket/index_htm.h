static const char homepage[] PROGMEM = R"EOF(
<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv="Content-type" content="text/html; charset=utf-8">
    <title>WebSocket test page</title>
    
    <style type="text/css" media="screen">
      body {width: 70%; margin: auto; background-color: black; color: #55ff55; font-family: monospace; text-align: center; }
      .box {border: 1px solid white; border-radius: 10px; padding: 5px; margin: 10px; }
      .output {color: white; }
      #log {text-align: left; margin: 20px; max-height: 500px; overflow: auto; }
      #input_div, #input_el {line-height: 13px; color: #AAA;}
      #input_el { width:97%; background-color: rgba(0,0,0,0); border: 0px;}
      #input_el:focus {outline: none; }
      #icon-setup{float: left;margin: 20px}
    </style>
    
    <script type="text/javascript">
      var ws = null;
      function ge(s){ return document.getElementById(s);} // Shorthand for get element
      function ce(s){ return document.createElement(s);}  // Shorthand for create element
      function stb(){ window.scrollTo(0, document.body.scrollHeight || document.documentElement.scrollHeight); } // Scroll to bottom
      
      // This function will add a message to the HTML element with id 'log'
      function addMessage(m){
        var msg = ce("div");
        msg.innerText = m;
        ge("log").appendChild(msg);
        stb();  
      }
      
      // If msg is a obj data tyep, handle it (just timestamp in this example) otherwise add to log console
      function parseMessage(msg) {
        try {
          const obj = JSON.parse(msg);
          if (typeof obj === 'object' && obj !== null) {
            if (obj.esptime !== null) {
              var date = new Date(0); // The 0 sets the date to epoch
              if( date.setUTCSeconds(obj.esptime))
                document.getElementById("esp-time").innerHTML = date;
            }
          }
        } catch {
          addMessage(msg);
        }
      }
      
      // Configure and start WebSocket client
      function startSocket(){
        ws = new WebSocket('ws://'+document.location.host+'/ws',['arduino']);
        ws.binaryType = "arraybuffer";
        ws.onopen = function(e){
          addMessage("WebSocket client connected to " + 'ws://'+document.location.host+'/ws');
        };
        ws.onclose = function(e){
          addMessage("WebSocket client disconnected");
        };
        ws.onerror = function(e){
          console.log("ws error", e);
          addMessage("Error");
        };
        ws.onmessage = function(e){
          parseMessage(e.data)
        };
        // Add event "keydown" listener to input box
        ge("input_el").onkeydown = function(e){
          stb();
          if(e.keyCode == 13 && ge("input_el").value != ""){
            ws.send(ge("input_el").value);
            ge("input_el").value = "";
          }
        }
      }
  
      // When page is fully loaded start connection
      function onBodyLoad(){
        startSocket();
      }
    </script>
  </head>
  <body id="body" onload="onBodyLoad()">
    
    <div class=icon>
      <a id=icon-setup href=/setup>
        <svg width="48" height="48" fill=#55ff55 viewBox="0 0 24 24">
          <path d="M12,8A4,4 0 0,1 16,12A4,4 0 0,1 12,16A4,4 0 0,1 8,12A4,4 0 0,1 12,8M12,10A2,2 0 0,0 10,12A2,2 0 0,0 12,14A2,2 0 0,0 14,12A2,2 0 0,0 12,10M10,22C9.75,22 9.54,21.82 9.5,21.58L9.13,18.93C8.5,18.68 7.96,18.34 7.44,17.94L4.95,18.95C4.73,19.03 4.46,18.95 4.34,18.73L2.34,15.27C2.21,15.05 2.27,14.78 2.46,14.63L4.57,12.97L4.5,12L4.57,11L2.46,9.37C2.27,9.22 2.21,8.95 2.34,8.73L4.34,5.27C4.46,5.05 4.73,4.96 4.95,5.05L7.44,6.05C7.96,5.66 8.5,5.32 9.13,5.07L9.5,2.42C9.54,2.18 9.75,2 10,2H14C14.25,2 14.46,2.18 14.5,2.42L14.87,5.07C15.5,5.32 16.04,5.66 16.56,6.05L19.05,5.05C19.27,4.96 19.54,5.05 19.66,5.27L21.66,8.73C21.79,8.95 21.73,9.22 21.54,9.37L19.43,11L19.5,12L19.43,13L21.54,14.63C21.73,14.78 21.79,15.05 21.66,15.27L19.66,18.73C19.54,18.95 19.27,19.04 19.05,18.95L16.56,17.95C16.04,18.34 15.5,18.68 14.87,18.93L14.5,21.58C14.46,21.82 14.25,22 14,22H10M11.25,4L10.88,6.61C9.68,6.86 8.62,7.5 7.85,8.39L5.44,7.35L4.69,8.65L6.8,10.2C6.4,11.37 6.4,12.64 6.8,13.8L4.68,15.36L5.43,16.66L7.86,15.62C8.63,16.5 9.68,17.14 10.87,17.38L11.24,20H12.76L13.13,17.39C14.32,17.14 15.37,16.5 16.14,15.62L18.57,16.66L19.32,15.36L17.2,13.81C17.6,12.64 17.6,11.37 17.2,10.2L19.31,8.65L18.56,7.35L16.15,8.39C15.38,7.5 14.32,6.86 13.12,6.62L12.75,4H11.25Z" />
        </svg>
      </a>
    </div>

    <div class="box">
      <p>ESP current time, sync with NTP server and sent to this client via <a href="https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API/">WebSocket!</a></p>
        <h4 id='esp-time'>Waiting websocket connection...</h4>
    </div>

    <div class="box">
      $<input type="text" value="" id="input_el">
    </div>
    
    <div class=box>
        <main id=log></main>
    </div>
  </body>
</html>
)EOF";