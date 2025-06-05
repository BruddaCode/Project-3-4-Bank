require('dotenv').config({ path: './conf.env' })
const mysql = require('mysql2')
const express = require('express')
const app = express()
const port = 9000

const db = mysql.createConnection({
    host: process.env.DB_HOST,
    port: process.env.DB_PORT,
    user: process.env.DB_USER,
    password: process.env.DB_PASS,
    database: process.env.DB_NAME
})

db.connect(err => {
    if (err) {
        console.error('Connection failed:', err.stack)
        return
    }
    console.log('Connected as id', db.threadId)
})

app.use(express.json())

app.get('/api/noob/health', (req, res) => {
    res.status(200).json({ status: "OK" })  // <-- fixed this line
})

// Common function for account validation
function validateAccount(iban, pin, pasnummer, res, callback) {
    db.query('SELECT * FROM rekeningen WHERE iban = ?', [iban], (err, result) => {
        if (err) return res.status(500).json({ error: err })
        if (!result[0]) return res.status(404).json({ error: "Can't find user" })

        const account = result[0]
        console.log('Account found:', account)
        if (account.actief === 0) return res.status(403).json({ error: "Card is blocked" })
        if (account.pin !== parseInt(pin)) return res.status(403).json({ error: "Wrong pin" })

        callback(account)
    })
}

app.post('/api/noob/users/getinfo', (req, res) => {
    // print request body for debugging
    console.log('Request body:', req.body)
    const requestData = checkJson(req.body, res)
    if (!requestData) return

    const { iban, pin, pasnummer} = requestData

    validateAccount(iban, pin, pasnummer, res, (account) => {
        const { iban, saldo, valuta } = account
        res.json({ iban, saldo, valuta })
    })
})

app.post('/api/noob/users/withdraw', (req, res) => {
    console.log('Request body:', req.body)
    const requestData = checkJson(req.body, res)
    if (!requestData) return

    const { iban, pin, amount } = requestData

    validateAccount(iban, pin, res, (account) => {
        if (amount > account.saldo) {
            return res.status(409).json({ error: "Insufficient funds" })
        }

        const newSaldo = account.saldo - amount
        db.query('UPDATE rekeningen SET saldo = ? WHERE iban = ?', [newSaldo, iban], (err) => {
            if (err) return res.status(500).json({ error: err })
            res.json({ iban, message: "Withdrawal successful" })
        })
    })
})

function checkJson(request, res) {
    for (const [key, value] of Object.entries(request)) {
        if (value === undefined || value === null || value === "") {
            res.status(400).json({ error: `Missing or empty field: ${key}` })
            return false
        }
    }

    for (const [key, value] of Object.entries(request)) {
        if (typeof value === 'string' && value.includes("'")) {
            res.status(400).json({ error: "Potential SQL injection detected" })
            return false
        }
    }

    return request
}

app.listen(port, () => {
    console.log(`Server running on port ${port}`)
})
