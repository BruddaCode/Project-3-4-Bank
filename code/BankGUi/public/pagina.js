
function storeMoneyValue(amount) {
    const number = amount; localStorage.setItem('storedMoneyValue', number);
    window.location.href = 'biljetVraag.html';
}
function storeInputMoneyValue() {
    const number = document.getElementById('numberInput').value;
    if (number % 10 != 0 || number == "") {
        document.getElementById('foutMelding').innerHTML = "Het bedrag moet eindigen in een 0!";
        document.getElementById('numberInput').value = "";
    } else {
        localStorage.setItem('storedInputMoneyValue', number);
        window.location.href = 'biljetVraag.html';
    }
}
// const removeData = document.getElementById('terug', 'ja', 'nee');
// removeData.addEventListener('click', () => {
//     localStorage.removeItem('storedMoneyValue');
//     localStorage.removeItem('storedInputMoneyValue');
// });

function clearStoredData() {
    localStorage.removeItem('storedMoneyValue');
    localStorage.removeItem('storedInputMoneyValue');
    localStorage.removeItem('biljetResults'); // Clear calculated results
}

function displayValue() {
    const number = localStorage.getItem('storedMoneyValue') || localStorage.getItem('storedInputMoneyValue');
    document.getElementById('biljetValue').textContent = number;
}

// Calculate Biljetten Combinations
function calculateBiljetten() {
    const bedrag = parseInt(localStorage.getItem('storedMoneyValue') || localStorage.getItem('storedInputMoneyValue'));
    const biljetten = [50, 20, 10]; // Already sorted in descending order

    // Option 1: Largest denominations first
    const optie1 = calculateOptie(bedrag, biljetten);
    // Option 2: Smallest denominations first
    const optie2 = calculateOptie(bedrag, [...biljetten].reverse());
    // Option 3: Random order
    const optie3 = calculateRandomOptie(bedrag, biljetten);

    // Create JSON object
    const results = {
        amountEntered: bedrag,
        optie1: optie1,
        optie2: optie2,
        optie3: optie3,
    };

    // Save JSON data to a file

}

function calculateOptie(bedrag, biljetten) {
    let result = {};
    for (let waarde of biljetten) {
        result[waarde] = Math.floor(bedrag / waarde);
        bedrag %= waarde;
    }
    return result;
}


function calculateRandomOptie(bedrag, biljetten) {
    let result = {};

    while (bedrag > 0) {
        // Filter out bills that are greater than the remaining amount
        const validBiljetten = biljetten.filter(biljet => biljet <= bedrag);

        // Select a random bill from the valid options
        const randomWaarde = validBiljetten[Math.floor(Math.random() * validBiljetten.length)];

        // Increment the count for the selected bill
        result[randomWaarde] = (result[randomWaarde] || 0) + 1;

        // Subtract the value of the selected bill from the remaining amount
        bedrag -= randomWaarde;
    }

    // Ensure all denominations are displayed (even if unused)
    biljetten.forEach(biljet => {
        if (!result[biljet]) {
            result[biljet] = 0; // Set count to 0 for unused bills
        }
    });

    return result;
}

function formatResult(result) {
    const biljettenOrder = [50, 20, 10]; // Define display order
    return biljettenOrder
        .map(waarde => `${result[waarde] || 0} x €${waarde}`) // Use predefined order
        .join(', ');
}


function recalculateBiljetten() {
    const bedrag = parseInt(localStorage.getItem('storedMoneyValue') || localStorage.getItem('storedInputMoneyValue'));
    const biljetten = [50, 20, 10];

    // Recalculate Options
    const optie1 = calculateOptie(bedrag, biljetten);
    const optie2 = calculateOptie(bedrag, [...biljetten].reverse());
    const optie3 = calculateRandomOptie(bedrag, biljetten);

    // Save results in localStorage
    const results = {
        optie1: optie1,
        optie2: optie2,
        optie3: optie3,
    };
    localStorage.setItem('biljetResults', JSON.stringify(results));

    // Update Display
    displayBiljettenResults();
}

// Display Results in Biljet Optie
function displayBiljettenResults() {
    const results = JSON.parse(localStorage.getItem('biljetResults'));
    if (results) {
        document.getElementById('Field1').textContent = formatResult(results.optie1); // €50 first
        document.getElementById('Field2').textContent = formatResult(results.optie2); // €10 first
        document.getElementById('Field3').textContent = formatResult(results.optie3); // Random order
    }
}

function selectBiljetOption(option) {
    const storedData = JSON.parse(localStorage.getItem('biljetResults'));
    const bedrag = parseInt(localStorage.getItem('storedMoneyValue') || localStorage.getItem('storedInputMoneyValue'));

    const selectedOption = storedData[option]; // e.g., 'optie1', 'optie2', or 'optie3'
    const jsonResult = {
        amountEntered: bedrag,
        chosenBills: selectedOption,
    };

    window.location.href = 'bonVraag.html';
}

window.onload = () => {
    displayValue();
    recalculateBiljetten();
};




//popup sessie afbreken
const openAfbreken = document.getElementById('openAfbreken');
const afbreekPopup = document.getElementById('afbreekPopup');

openAfbreken.addEventListener('click', () => {
    afbreekPopup.style.display = 'flex';
    numberInput.focus();
});