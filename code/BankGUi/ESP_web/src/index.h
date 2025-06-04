const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0, user-scalable=no">
  <link rel="stylesheet" type="text/css" href="opmaak.css" />
  <title>Home</title>
</head>
<style>
  input[type='text'] {
    font-family: Verdana;

  }
  input::-ms-reveal,
  input::-ms-clear {
    display: none;
  }
</style>
<body>
  <div class="container">
    <div id="article1">
      <p>Welkom bij</p>
      <p>SYMPLE</p>
      <p>Scan uw pas om verder te gaan</p>
      
      <div id="popup" class="popup">
        <div class="popup-Pin">
          <h3>Voer uw pincode in:</h3>
          <p id="foutMelding"></p>
          <input type="password" id="pincode" style="-webkit-text-security: circle; font-size: 32px;" maxlength="4" minlength="4" required/>
          <p id="verder">Druk op de vink om door te gaan</p>
        </div>
      </div>

    </div>
    
    <script>
    //popup pincode
      const gateway = `ws://${window.location.hostname}/ws`;
      const websocket = new WebSocket(gateway);

      websocket.onmessage = (event) => {
        const msg = event.data.split(":");
        
        if (msg[0] === "card") {
          document.getElementById('popup').style.display = 'flex';
          document.getElementById('pincode').focus();
        }

        if (msg[0] === "pin") {
          document.getElementById('pincode').value = msg[1];
        }

        if (msg[0] === "error") {
          document.getElementById('foutMelding').innerText = msg[1];
        }

        if (msg[0] === "home") {
          window.location.href = 'home';
        }
      };
    </script>
</body>
</html>
)rawliteral";