import requests
import json

def NoobTest(payload):
    url = f"https://noob.datalabrotterdam.nl/api/noob/users/getinfo?target={payload['iban']}"

    payload = json.dumps({
        "iban": payload["iban"],
        "pin": payload["pin"],
        "pasnummer": payload["pasnummer"]
    })
    
    headers = {
        'NOOB-TOKEN': 'baf10db5-0db9-462b-91e6-d446da5d4c6d',
        'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    return response.text

if __name__ == "__main__":
    testPayloads = [
        {"iban": "TD01SYMB0000000002", "pin": "4321", "pasnummer": "553"},
        {"iban": "TD01SYMB0000000003", "pin": "1987", "pasnummer": "880"},
        {"iban": "TD01SYMB0000000004", "pin": "5678", "pasnummer": "889"},
        {"iban": "TD01SYMB0000000005", "pin": "8765", "pasnummer": "800"},
        {"iban": "TD01SYMB0000000006", "pin": "1234", "pasnummer": "420"}
    ]
    for payload in testPayloads:
        print(f"Testing noob for iban {payload}: {NoobTest(payload)}")