const char bonVraag_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0 , user-scalable=no">
  <link rel="stylesheet" type="text/css" href="opmaak.css" />
  <script src="pagina.js"></script>
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
                <button onclick="clearStoredData(); window.location.href='index.html';" class="button" id="nee">Nee </button>
                <button onclick="" class="button hidden">button 3</button>
            </div>
        
            <div id="article1">
                <p>Processing... </p>

                <p>Wilt u de bon?</p>
                
            </div>

            <div class="side-buttons right-button">
                <button onclick="" class="button hidden">button 4</button>
                <button onclick="clearStoredData(); window.location.href='index.html';" class="button" id="ja">Ja</button>
                <button onclick="" class="button hidden">button 5</button>
            </div>
        </div>
    </div>    
</body>
</html>
)rawliteral";