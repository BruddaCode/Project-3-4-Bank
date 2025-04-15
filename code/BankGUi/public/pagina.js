function storeMoneyValue(amount) {
    const number = amount;
    localStorage.setItem('storedMoneyValue', number);
    window.location.href = 'biljetOptie.html';
}

function storeInputMoneyValue() {
    const number = document.getElementById('numberInput').value;
    localStorage.setItem('storedInputMoneyValue', number);
    window.location.href = 'biljetOptie.html';
}
