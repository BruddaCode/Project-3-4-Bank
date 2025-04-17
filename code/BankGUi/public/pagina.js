function storeMoneyValue(amount) {
    const number = amount;
    localStorage.setItem('storedMoneyValue', number);
    window.location.href = 'biljetVraag.html';
}

function storeInputMoneyValue() {
    const number = document.getElementById('numberInput').value;
    if(number % 10 != 0 || number == ""){
        document.getElementById('foutMelding').innerHTML = "Het bedrag moet eindigen in een 0!";
        document.getElementById('numberInput').value = "";
    } else{
    localStorage.setItem('storedInputMoneyValue', number);
    window.location.href = 'biljetVraag.html';
    }
}


const removeData = document.getElementById('terug', 'ja', 'nee');
removeData.addEventListener('click', () => {
    localStorage.removeItem('storedMoneyValue');
    localStorage.removeItem('storedInputMoneyValue');
});

function displayValue(){
    const number = localStorage.getItem('storedMoneyValue') || localStorage.getItem('storedInputMoneyValue');
    document.getElementById('biljetValue').textContent = number;
}
window.onload = displayValue;

document.addEventListener('touchmove', function(event){
    if (event.scale !== 1) {
      event.preventDefault();
    }
  }, {passive: false });