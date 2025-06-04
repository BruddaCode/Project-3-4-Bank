const char geldKeuze_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0 , user-scalable=no">
    <link rel="stylesheet" type="text/css" href="opmaak.css" />
    <title>GeldKeuze</title>
</head>

<body>
    <div class="container">
        <div class="button-grid">
            <div class="side-buttons left-button">
                <button class="button" id="20Pin">€20,-</button>
                <button class="button" id="50Pin">€50,-</button>
                <button onclick="window.location.href='home';" class="button" id="terug">Terug</button>
            </div>

            <div id="article1">
                <p>Voer uw bedrag in:</p>
            </div>

            <div class="side-buttons right-button">
                <button class="button" id= "100Pin">€100,-</button>
                <button id="openPopup" class="button" id="eigenKeuze">Eigen keuze</button>
                <button class="button openAfbrekenBtn" id="afbreken">Sessie Afbreken</button>
            </div>


            <div id="popup" class="popup">
                <div class="popup-content">
                    <h3>Voer het bedrag in:</h3>

                    <input type="number" id="numberInput" placeholder="Voer een tiental in" style="font-size: 32px;" />

                    <p id="foutMelding"></p>
                    <p id="verder">Druk op de vink om door te gaan</p>
                </div>
            </div>

                    <!-- Popup -->
            <div id="afbreekPopup" class="popup">
                <div class="popup-content-afbreken">
                <h3>Weet u zeker dat u de sessie wilt afbreken?</h3>
                <button onclick="afbreekPopup.style.display='none';" class="button" id="miniNee">Nee</button>
                <button id="bevestig_afbreken" class="button" id="miniJa">Ja</button>
                </div>
            </div>
        </div>
    </div>


    <script>
        const openPopup = document.getElementById('openPopup');
        const okPopup = document.getElementById('okPopup');
        const popup = document.getElementById('popup');
        const numberInput = document.getElementById('numberInput');



        openPopup.addEventListener('click', () => {
            popup.style.display = 'flex';
            numberInput.focus();
        });


        numberInput.addEventListener('keydown', function (event) {
            if (event.key === 'Enter') {
                storeInputMoneyValue();
            }
        });

        window.addEventListener('click', (event) => {
            if (event.target === popup) {
                popup.style.display = 'none';
            }
        });

        
        const gateway = `ws://${window.location.hostname}/ws`;
        const websocket = new WebSocket(gateway);

        websocket.onmessage = (event) => {
            const msg = event.data.split(":");
                
            if (msg[0] === "sideBtn") {
                if (msg[1] === "6") {
                    document.getElementById('afbreekPopup').style.display = 'flex';
                }
                if (msg[1] === "1") {
                    document.getElementById('20Pin').style.display = 'flex';
                }   
                if (msg[1] === "2") {
                    document.getElementById('50Pin').style.display = 'flex';
                }
                if (msg[1] === "3") {
                    document.getElementById('terug').style.display = 'flex';
                }  
                if (msg[1] === "4") {
                    document.getElementById('100Pin').style.display = 'flex';
                }
                if (msg[1] === "5") {
                    document.getElementById('eigenKeuze').style.display = 'flex';
                }              
            }     
        };

        document.getElementById('20Pin').addEventListener('click', () => {
            websocket.send("pin20");
            window.location.href = 'biljetVraag';
        });
        document.getElementById('50Pin').addEventListener('click', () => {
            websocket.send("pin20");
            window.location.href = 'biljetVraag';
        });
        document.getElementById('100Pin').addEventListener('click', () => {
            websocket.send("pin20");
            window.location.href = 'biljetVraag';
        });

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