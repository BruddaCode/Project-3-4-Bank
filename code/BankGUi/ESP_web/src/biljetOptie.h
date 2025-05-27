const char biljetOptie_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0 , user-scalable=no">
    <link rel="stylesheet" type="text/css" href="opmaak.css" />
    <script src="pagina.js"></script>
    <title>biljet optie</title>

    <style>

    </style>

</head>

<body>
  <div class="container">
    <div class="button-grid">
      <div class="side-buttons left-button">
        <button onclick="selectBiljetOption('optie1');" class="button" id="Field1">Optie 1</button>
        <button onclick="selectBiljetOption('optie3');" class="button" id="Field3">Optie 3</button>
        <button onclick="clearStoredData(); window.location.href='geldKeuze.html';" class="button" id="terug">Terug</button>
      </div>

      <div id="article1">
        <p>
          U heeft €<span id="biljetValue">100</span> ingevoerd.<br />
          Welke biljet combinatie wilt u?
        </p>
      </div>

      <div class="side-buttons right-button">
        <button onclick="selectBiljetOption('optie2');" class="button" id="Field2">Optie 2</button>
        <button class="button hidden" style="visibility:hidden;"></button>
        <button class="button" id="openAfbreken">Sessie Afbreken</button>
      </div>

      <!-- Popup -->
      <div id="afbreekPopup" class="popup">
        <div class="popup-content-afbreken">
          <h3>Weet u zeker dat u de sessie wilt afbreken?</h3>
          <button onclick="afbreekPopup.style.display='none';" class="button" id="miniNee">Nee</button>
          <button onclick="clearStoredData(); window.location.href='index.html';" class="button" id="miniJa">Ja</button>
        </div>
      </div>
    </div>
  </div>
</body>

</html>
)rawliteral";