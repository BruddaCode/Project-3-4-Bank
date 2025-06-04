const char biljetVraag_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0 , user-scalable=no">
    <link rel="stylesheet" type="text/css" href="opmaak.css" />
    <script src="pagina.js"></script>
    <title>Biljet Keuze</title>

</head>

<body>
    <div class="conainer">
        <div class="button-grid">
            <div class="side-buttons left-button">
                <button onclick="" class="button hidden">Button 1</button>
                <button onclick="window.location.href='bonVraag.html'; " class="button" id="biljetNee">Nee</button>
                <button onclick="window.location.href='geldKeuze.html';" class="button" id="terug">terug</button>
            </div>

            <div id="article1">
                <p>Wilt u kiezen welke biljetten u krijgt? </p>

            </div>

            <div class="side-buttons right-button">
                <button onclick="" class="button hidden">button 4</button>
                <button onclick="window.location.href='biljetOptie.html';" class="button" id="biljetJa">Ja</button>
                <button class="button openAfbrekenBtn" id="afbreken">Sessie Afbreken</button>
            </div>

            <div id="afbreekPopup" class="popup">
                <div class="popup-content-afbreken">
                    <h3>Weet u zeker dat u de sessie wilt afbreken?</h3>

                    <button onclick=" window.location.href='biljetVraag.html';" class="button" id="miniNee">Nee
                    </button>
                    <button onclick="clearStoredData(); window.location.href='index.html';" class="button"
                        id="miniJa">Ja</button>

                </div>
            </div>
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
                    document.getElementById('biljetNee').style.display = 'flex';
                }
                if (msg[1] === "3") {
                    document.getElementById('terug').style.display = 'flex';
                }     
                if (msg[1] === "5") {
                    document.getElementById('biljetJa').style.display = 'flex';
                }                                               
            }     
        };

        document.getElementById('afbreken').addEventListener('click', () => {
            websocket.send("sideBtn:break");
        });
    </script>




</body>

</html>
)rawliteral";