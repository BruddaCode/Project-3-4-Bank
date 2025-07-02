import random

class BiljetResult:
    def __init__(self):
        self.b10 = 0
        self.b20 = 0
        self.b50 = 0

def calculate_optie1(bedrag):
    optie1 = BiljetResult()
    optie1.b50 = bedrag // 50
    bedrag %= 50

    optie1.b20 = bedrag // 20
    bedrag %= 20

    optie1.b10 = bedrag // 10

    calculated = f"50x{optie1.b50}|20x{optie1.b20}|10x{optie1.b10}"

    return calculated

def fallback_greedy(remaining, result, aantal_tientjes, max_tientjes):
    while remaining >= 50:
        result.b50 += 1
        remaining -= 50
    while remaining >= 20:
        result.b20 += 1
        remaining -= 20
    while remaining >= 10 and aantal_tientjes < max_tientjes:
        result.b10 += 1
        aantal_tientjes += 1
        remaining -= 10

    calculated = f"50x{result.b50}|20x{result.b20}|10x{result.b10}"

    return calculated

def calculate_optie2(bedrag):
    optie2 = BiljetResult()
    aantal_tientjes = 0
    remaining = bedrag
    max_tientjes = 6

    while remaining > 0:
        valid = []
        if remaining >= 50:
            valid.append(50)
        if remaining >= 20:
            valid.append(20)
        if remaining >= 10 and aantal_tientjes < max_tientjes:
            valid.append(10)

        if not valid:
            return fallback_greedy(remaining, optie2, aantal_tientjes, max_tientjes)

        keuze = random.choice(valid)

        if keuze == 50:
            optie2.b50 += 1
        elif keuze == 20:
            optie2.b20 += 1
        elif keuze == 10:
            optie2.b10 += 1
            aantal_tientjes += 1

        remaining -= keuze

    calculated = f"50x{optie2.b50}|20x{optie2.b20}|10x{optie2.b10}"

    return calculated

def calculate_optie3(bedrag):
    optie3 = BiljetResult()
    remaining = bedrag

    while remaining > 0:
        valid = []
        if remaining >= 50:
            valid.append(50)
        if remaining >= 20:
            valid.append(20)
        if remaining >= 10:
            valid.append(10)

        keuze = random.choice(valid)

        if keuze == 50:
            optie3.b50 += 1
        elif keuze == 20:
            optie3.b20 += 1
        elif keuze == 10:
            optie3.b10 += 1

        remaining -= keuze

    calculated = f"50x{optie3.b50}|20x{optie3.b20}|10x{optie3.b10}"

    return calculated

def BerekenTest(bedrag):
    print(f"Bedrag: {bedrag} euro")
    optie1 = calculate_optie1(bedrag)
    optie2 = calculate_optie2(bedrag)
    optie3 = calculate_optie3(bedrag)
    
    return {
        "optie1": optie1,
        "optie2": optie2,
        "optie3": optie3
    }

if __name__ == "__main__":
    testBedragen = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
    for bedrag in testBedragen:
        result = BerekenTest(bedrag)
        print(f"Resultaten voor {bedrag} euro: {result}")
        print("-" * 40)