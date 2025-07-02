import requests
import json

def PincodeTest(pincode):
    url = "http://145.24.222.28:9000/api/users/getinfo"

    payload = json.dumps({
    "iban": "TD01SYMB0000000002",
    "pin": pincode,
    "pasnummer": "553"
    })
    headers = {
    'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    return response.text

if __name__ == "__main__":
    testPincodes = ["1234", "4321", "5678", "8765", "0000"]
    for pincode in testPincodes:
        print(f"Testing pincode {pincode}: {PincodeTest(pincode)}")
