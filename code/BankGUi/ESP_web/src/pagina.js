const char pagina_js[] PROGMEM = R"rawliteral(
function storeMoneyValue(amount) {
    const number = amount;
    localStorage.setItem('storedMoneyValue', number);
    sendMoneyValueToESP();
    window.location.href = 'biljetVraag.html';
}

function storeInputMoneyValue() {
    const number = document.getElementById('numberInput').value;

    if (number % 10 != 0 || number == "") {
        document.getElementById('foutMelding').innerHTML = "Het bedrag moet eindigen in een 0!";
        document.getElementById('numberInput').value = "";
    } 
    else if (parseInt(number) > 600) {
        document.getElementById('foutMelding').innerHTML = "Het bedrag mag maximaal €600 zijn!";
        document.getElementById('numberInput').value = "";
    }
    else {
        localStorage.setItem('storedInputMoneyValue', number);
        sendMoneyValueToESP();
        window.location.href = 'biljetVraag.html';
    }
}

function clearStoredData() {
    localStorage.removeItem('storedMoneyValue');
    localStorage.removeItem('storedInputMoneyValue');
    localStorage.removeItem('biljetResults');
    console.log("Storage cleared");
}

function displayValue() {
    const number = localStorage.getItem('storedMoneyValue') || localStorage.getItem('storedInputMoneyValue');
    const el = document.getElementById('biljetValue');
    if (el) {
        el.textContent = number || '';
    } else {
        console.warn('Element #biljetValue not found');
    }
}

const biljetten = [50, 20, 10];

function calculateAllOptions(bedrag) {
    const optie1 = calculateOptie1(bedrag);
    const optie2 = calculateOptie2(bedrag);
    const optie3 = calculateOptie3(bedrag);
    return { optie1, optie2, optie3 };
}

function formatBiljettenText(name, bedrag, result) {
    let output = `Option: ${name}\nAmount: €${bedrag}\n`;
    for (let b of biljetten) {
        output += `€${b}: ${result[b] || 0}\n`;
    }
    return output.trim();
}

function formatResult(result) {
    return `${result["50"] || 0} x €50, ${result["20"] || 0} x €20, ${result["10"] || 0} x €10`;
}

function updateBiljetFields() {
    fetch('/biljettenResult')
        .then(response => response.json())
        .then(data => {
            localStorage.setItem('biljetResults', JSON.stringify({
                optie1: data.optie1,
                optie2: data.optie2,
                optie3: data.optie3,
            }));
            document.getElementById('Field1').textContent = formatResult(data.optie1);
            document.getElementById('Field2').textContent = formatResult(data.optie2);
            document.getElementById('Field3').textContent = formatResult(data.optie3);
        })
        .catch(err => {
            console.error('Failed to fetch biljetten results:', err);
        });
}

function selectBiljetOption(option) {
    const storedData = JSON.parse(localStorage.getItem('biljetResults'));
    if (!storedData || !storedData[option]) {
        console.error('No stored biljet results found for option:', option);
        return;
    }
    
    sendSelectedOption(option);
}


function sendMoneyValueToESP() {
    const bedrag = parseInt(localStorage.getItem('storedMoneyValue') || localStorage.getItem('storedInputMoneyValue'));
    if (!bedrag) return;

    fetch('/saveAmount', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: `bedrag=${bedrag}`
    })
    .then(response => response.text())
    .then(data => console.log('Server response:', data))
    .catch(err => console.error('Error sending bedrag:', err));
}

function selectBiljetOption(option) {
    const storedData = JSON.parse(localStorage.getItem('biljetResults'));
    if (!storedData || !storedData[option]) {
        console.error('No stored biljet results found for option:', option);
        alert('Geen gegevens gevonden voor deze optie.');
        return;
    }
    
    sendSelectedOption(option);
}

function sendSelectedOption(optionName) {
    const bedrag = parseInt(localStorage.getItem('storedMoneyValue') || localStorage.getItem('storedInputMoneyValue'));
    if (!bedrag) {
        alert('Geen bedrag gevonden. Voer eerst een bedrag in.');
        return;
    }
    
    const results = JSON.parse(localStorage.getItem('biljetResults'));
    if (!results || !results[optionName]) {
        alert('Biljet resultaten ontbreken.');
        return;
    }
    
    const selected = results[optionName];
    const biljetten = [50, 20, 10];
    let plainText = `Option: ${optionName}\nAmount: €${bedrag}\n`;
    for (let b of biljetten) {
        plainText += `€${b}: ${selected[b] || 0}\n`;
    }
    plainText = plainText.trim();
    
    fetch('/print', {
        method: 'POST',
        headers: { 'Content-Type': 'text/plain' },
        body: plainText
    })
    .then(response => {
        if (!response.ok) throw new Error('Failed to send data');
        console.log("Sent:", plainText);
        // Redirect after successful send
        window.location.href = 'bonVraag.html';
    })
    .catch(err => {
        console.error('Error sending selection:', err);
        alert('Fout bij versturen, probeer het opnieuw.');
    });
}

// Show/hide afbreken popup & handle popup buttons
document.addEventListener('DOMContentLoaded', () => {
    displayValue();
    updateBiljetFields();

    // const openAfbreken = document.getElementById('openAfbreken');
    const afbreekPopup = document.getElementById('afbreekPopup');
    const miniNee = document.getElementById('miniNee');
    const miniJa = document.getElementById('miniJa');

    document.querySelectorAll('.openAfbrekenBtn').forEach(btn => {
        btn.addEventListener('click', () => {
            afbreekPopup.style.display = 'flex';
        });
    });

    if (miniNee && afbreekPopup) {
        miniNee.addEventListener('click', () => {
            afbreekPopup.style.display = 'none'; // Hide popup on No
        });
    }

    if (miniJa) {
        miniJa.addEventListener('click', () => {
            clearStoredData();
            window.location.href = 'index.html'; // Redirect on Yes
        });
    }
});

)rawliteral";