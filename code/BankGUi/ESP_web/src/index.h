const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1.0 , maximum-scale=1.0, user-scalable=no">
  <link rel="stylesheet" type="text/css" href="opmaak.css" />
  <script src="pagina.js"></script>
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
      <p>
        Scan uw pas om verder te gaan
      </p>
      <div id="popup" class="popup">
        <div class="popup-Pin">
          <h3>Voer uw pincode in:</h3>
          <p id="foutMelding"></p>
          <form action="home.html" method="post">
            <input type="password" id="pincode" style="-webkit-text-security: circle; font-size: 32px;" maxlength="4"
              minlength="4" required />
         </form>
          <p id="verder">Druk op de vink om door te gaan</p>
        </div>
      </div>
    </div>
    <script>
    //popup pincode
      const openPopup = document.getElementById('article1');
      const okPopup = document.getElementById('okPopup');
      const popup = document.getElementById('popup');
      document.addEventListener('click', function () {
        const popup = document.getElementById('popup');
        popup.style.display = 'flex';
        document.getElementById('pincode').focus();
      });
      okPopup.addEventListener('click', () => {
        document.getElementById('pincode').focus();
        popup.style.display = 'none';
      });
      window.addEventListener('click', (event) => {
        if (event.target === popup) {
          popupPin.style.display = 'none';
        }
      });
      document.getElementById('pincode').addEventListener('keypress', function (event) {
          if (event.key === 'Enter') { // Check if the Enter key is pressed
            window.location.href = 'home.html'; // Redirect to the next page
          }
        });
      document
        .getElementById("pincode")
        .addEventListener("keypress", function (evt) {
          if (evt.which < 48 || evt.which > 57) {
            evt.preventDefault();
          }
        });
    </script>
</body>
</html>
)rawliteral";