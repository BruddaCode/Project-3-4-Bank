import requests
import json

def WithdrawTest(amount):
    url = "http://145.24.222.28:9000/api/users/withdraw"

    payload = json.dumps({
    "iban": "TD01SYMB0000000005",
    "pin": "7777",
    "pasnummer": "800",
    "amount": amount
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    return response.text

if __name__ == "__main__":
    testValues = [1, 10, 100, 1000, 10000, 100000]
    for value in testValues:
        print(f"Testing withdrawal of {value} euros: {WithdrawTest(value)}")
