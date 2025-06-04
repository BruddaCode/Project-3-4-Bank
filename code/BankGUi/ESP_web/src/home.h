const char home_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang ="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0 , user-scalable=no">
    <link rel="stylesheet" type="text/css" href="opmaak.css" />
    <title>home</title>

</head>

<body>
    <div class="conainer">
        <div class="button-grid">
            <div class="side-buttons left-button">
                <button class="button hidden"></button>
                <button onclick="window.location.href='saldo';"class="button" id="rekening">Rekening inzien</button>
                <button class="button hidden"></button>
            </div>
        
            <div id="article1">
                <p>Welkom </p>
            </div>

            <div class="side-buttons right-button">
                <button onclick="window.location.href='geldKeuze';" class="button" id="geldOpnemen">Geld opnemen</button>
                <button onclick="storeMoneyValue(70)" "window.location.href='bonVraag';" class="button" id= "snelPin">€70,-</button>
                <button id="afbreken">Sessie Afbreken</button>
            </div>

            <div id="afbreekPopup" class="popup">
                <div class="popup-content-afbreken">
                    <h3>Weet u zeker dat u de sessie wilt afbreken?</h3>

                    <button onclick=" window.location.href='home';" class="button" id="miniNee">Nee</button>
                    <button onclick="clearStoredData(); window.location.href='index';" class="button" id="miniJa">Ja</button>
                </div>
            </div>

            <script> 
                const gateway = `ws://${window.location.hostname}/ws`;
                const websocket = new WebSocket(gateway);

                websocket.onmessage = (event) => {
                    const msg = event.data.split(":");
                    
                    if (msg[0] === "sideBtn") {
                        if (msg[1] === "6") {
                            document.getElementById('afbreekPopup').style.display = 'flex';
                        }
                        if (msg[1] === "2") {
                            document.getElementById('rekening').style.display = 'flex';
                        }
                        if (msg[1] === "4") {
                            document.getElementById('geldOpnemen').style.display = 'flex';
                        }     
                        if (msg[1] === "5") {
                            document.getElementById('snelPin').style.display = 'flex';
                        }                                               
                    }

                    if (msg[0] === "pin") {
                        document.getElementById('pincode').value = msg[1];
                    }        
                };

                document.getElementById('afbreken').addEventListener('click', () => {
                    websocket.send("sideBtn:break");
                });
            </script>
        </div>
    </div>    
</body>
</html>
)rawliteral";