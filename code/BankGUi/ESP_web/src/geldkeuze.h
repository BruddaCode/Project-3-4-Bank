const char geldKeuze_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0 , user-scalable=no">
    <link rel="stylesheet" type="text/css" href="opmaak.css" />
    <script src="pagina.js"></script>
    <title>GeldKeuze</title>
</head>

<body>
    <div class="container">
        <div class="button-grid">
            <div class="side-buttons left-button">
                <button onclick="storeMoneyValue(20)" class="button">€20,-</button>
                <button onclick="storeMoneyValue(50)" class="button">€50,-</button>
                <button onclick="window.location.href='home.html';" class="button">Terug</button>
            </div>

            <div id="article1">
                <p>Voer uw bedrag in:</p>
            </div>

            <div class="side-buttons right-button">
                <button onclick="storeMoneyValue(100)" class="button">€100,-</button>
                <button id="openPopup" class="button">Eigen keuze</button>
                <button class="button openAfbrekenBtn">Sessie Afbreken</button>
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
                <button type="button" class="button" id="miniJa">Ja</button>
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


    </script>

</body>

</html>
)rawliteral";