import requests
import json

def SaldoTest(payload):
    url = "http://145.24.222.28:9000/api/users/getinfo"

    payload = json.dumps({
    "iban": payload["iban"],
    "pin": payload["pin"],
    "pasnummer": payload["pasnummer"]
    })
    
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    return response.text

if __name__ == "__main__":
    testPayloads = [{"iban": "TD01SYMB0000000002","pin": "4321","pasnummer": "553"},
                    {"iban": "TD01SYMB0000000003","pin": "1987","pasnummer": "880"},
                    {"iban": "TD01SYMB0000000004","pin": "5643","pasnummer": "889"},
                    {"iban": "TD01SYMB0000000005","pin": "7777","pasnummer": "800"},
                    {"iban": "TD01SYMB0000000006","pin": "1234","pasnummer": "420"}]
    for payload in testPayloads:
        print(f"Testing saldo for iban {payload}: {SaldoTest(payload)}")