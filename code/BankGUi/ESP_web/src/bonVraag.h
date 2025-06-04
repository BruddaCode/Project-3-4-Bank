const char bonVraag_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0 , user-scalable=no">
  <link rel="stylesheet" type="text/css" href="opmaak.css" />
  <title>Bon Vraag</title>

  <style>
    button.hidden{
        visibility: hidden;
    }
  </style>
</head>

<body>
    <div class="container">
        <div class="button-grid">
            <div class="side-buttons left-button">
                <button onclick="" class="button hidden">Terug</button>
                <button window.location.href='/';" class="button" id="nee">Nee </button>
                <button onclick="" class="button hidden">button 3</button>
            </div>
        
            <div id="article1">
                <p>Processing... </p>

                <p>Wilt u de bon?</p>
                
            </div>

            <div class="side-buttons right-button">
                <button onclick="" class="button hidden">button 4</button>
                <button window.location.href='/';" class="button" id="ja">Ja</button>
                <button onclick="" class="button hidden">button 5</button>
            </div>
        </div>
    </div>   
    
    
    <script>
        const gateway = `ws://${window.location.hostname}/ws`;
        const websocket = new WebSocket(gateway);

        websocket.onmessage = (event) => {
            const msg = event.data.split(":");
                    
            if (msg[0] === "sideBtn") {
                if (msg[1] === "2") {
                    document.getElementById('nee').style.display = 'flex';
                }
                if (msg[1] === "5") {
                    document.getElementById('ja').style.display = 'flex';
                }
            }     
        };

        document.getElementById('afbreken').addEventListener('click', () => {
                    document.getElementById('afbreekPopup').style.display = 'flex';
                });

        document.getElementById('bevestig_afbreken').addEventListener('click', () => {
            websocket.send("break");
            window.location.href = '/';
        });
    </script>    
</body>
</html>
)rawliteral";