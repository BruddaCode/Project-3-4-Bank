const char saldo_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0 , user-scalable=no">
    <link rel="stylesheet" type="text/css" href="opmaak.css" />
    <title>Saldo</title>

</head>




<body>
    <div class="conainer">
        <div class="button-grid">
            <div class="side-buttons left-button">
                <button onclick="" class="button hidden">Button 1</button>
                <button onclick="window.location.href='home';" class="button" id="terug">Terug</button>
                <button onclick="" class="button hidden">button 3</button>
            </div>

            <div id="article2">
                <p>Welkom
                <p>Uw saldo is:
                <br><span id="saldoValue">0</span> Euro</p>
                </p>
            </div>

            <div class="side-buttons right-button">
                <button onclick="" class="button hidden">button 4</button>
                <button class="button openAfbrekenBtn" id="afbreken">Sessie Afbreken</button>
                <button onclick="" class="button hidden">button 6</button>
            </div>

            <div id="afbreekPopup" class="popup">
                <div class="popup-content-afbreken">
                    <h3>Weet u zeker dat u de sessie wilt afbreken?</h3>
                    
                        <button onclick="" class="button hidden">Button 1</button>
                        <button onclick=" window.location.href='saldo';" class="button" id="miniNee">Nee</button>
                        <button onclick="" class="button hidden">button 3</button>
                    
                    
                        <button onclick="" class="button hidden">button 4</button>
                        <button onclick="clearStoredData(); window.location.href='index';" class="button"
                            id="miniJa">Ja</button>
                        <button onclick="" class="button hidden">button 6</button>
                    


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
                if (msg[1] === "4") {
                    document.getElementById('afbreekPopup').style.display = 'flex';
                }
                if (msg[1] === "2") {
                    document.getElementById('terug').style.display = 'flex';
                }
            }

            if (msg[0] === "saldo") {
                document.getElementById('saldoValue').innerHTML = msg[1];                             
            }     
        };

        document.addEventListener('DOMContentLoaded', () => {
            websocket.send("getSaldo");
        });

        document.getElementById('afbreken').addEventListener('click', () => {
            websocket.send("break");
        });
    </script>
</body>

</html>
)rawliteral";