const char biljetOptie_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0 , user-scalable=no">
    <link rel="stylesheet" type="text/css" href="opmaak.css" />
    <title>biljet optie</title>
</head

<body>
  <div class="container">
    <div class="button-grid">
      <div class="side-buttons left-button">
        <button class="button" id="Field1">Optie 1</button>
        <button class="button" id="Field3">Optie 3</button>
        <button onclick="window.location.href='geldKeuze';" class="button" id="terug">Terug</button>
      </div>

      <div id="article1">
        <p>
          U heeft €<span id="biljetValue">0</span> ingevoerd.</br>
          Welke biljet combinatie wilt u?
        </p>
      </div>

      <div class="side-buttons right-button">
        <button class="button" id="Field2">Optie 2</button>
        <button class="button hidden" style="visibility:hidden;"></button>
        <button class="button openAfbrekenBtn" id="afbreken">Sessie Afbreken</button>
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
    const gateway = `ws://${window.location.hostname}/ws`;
    const websocket = new WebSocket(gateway);

    websocket.onmessage = (event) => {
      const msg = event.data.split(":");

      if (msg[0] === "amount") {
        document.getElementById('biljetValue').innerText = msg[1];
      }

      if (msg[0] === "optie1") {
        document.getElementById('Field1').innerText = msg[1];
      }
      if (msg[0] === "optie2") {
        document.getElementById('Field2').innerText = msg[1];
      }
      if (msg[0] === "optie3") {
        document.getElementById('Field3').innerText = msg[1];
      }
                    
      if (msg[0] === "sideBtn") {
        if (msg[1] === "6") {
          document.getElementById('afbreekPopup').style.display = 'flex';
        }
        if (msg[1] === "1") {
          document.getElementById('Field1').style.display = 'flex';
        }
        if (msg[1] === "2") {
          document.getElementById('Field3').style.display = 'flex';
        }     
        if (msg[1] === "3") {
          document.getElementById('terug').style.display = 'flex';
        }    
        if (msg[1] === "4") {
          document.getElementById('Field2').style.display = 'flex';
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

    document.getElementById('Field1').addEventListener('click', () => {
      websocket.send("optie1");
      window.location.href = 'bonVraag';
    });
    document.getElementById('Field2').addEventListener('click', () => {
      websocket.send("optie2");
      window.location.href = 'bonVraag';
    });
    document.getElementById('Field3').addEventListener('click', () => {
      websocket.send("optie3");
      window.location.href = 'bonVraag';
    });
  </script>

</body>

</html>
)rawliteral";