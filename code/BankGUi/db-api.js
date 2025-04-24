require('dotenv').config({path:'./conf.env'})
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
    res.statusCode(200).json({status: "OK"})
})

// --- getinfo ---
app.post('/api/noob/users/getinfo', (req, res) => {
    // check if given json is correct
    const responseData = checkJson(req.body, res)
    if (!responseData) return
    const {iban, pin} = responseData

    db.query('SELECT * FROM rekeningen WHERE iban = ?', [iban], (err, result) => {
        if (err) {
            return res.status(500).json({ error: err })
        }
        if (result[0] == null) {
            return res.status(500).json({ error: "cant find user" })
        }
        if (result[0].actief == 0){
            return res.status(409).json({ error: "card is blocked" })
        }
        if (pin != result[0].pin) {
            return res.status(409).json({ error: "wrong pin" })
        }
        const {iban, saldo, valuta} = result[0]
        res.json({iban, saldo, valuta})
    })
})

// --- withdraw ---
app.post('/api/noob/users/withdraw', (req, res) => {
    const responseData = checkJson(req.body, res)
    if (!responseData) return
    const {iban, pin, amount} = responseData

    db.query('SELECT * FROM rekeningen WHERE iban = ?', [iban], (err, result) => {
        if (err) {
            return res.status(500).json({ error: err })
        }
        if (result[0] == null) {
            return res.status(500).json({ error: "cant find user" })
        }
        if (result[0].actief == 0){
            return res.status(409).json({ error: "card is blocked" })
        }
        if (pin != result[0].pin) {
            return res.status(409).json({ error: "wrong pin" })
        }
        if (amount > result[0].saldo) {
            return res.status(409).json({ error: "Insufficient Funds" })
        }
        var saldo = result[0].saldo - amount

        db.query('UPDATE rekeningen SET saldo = ? WHERE iban = ?', [saldo, iban], (err) => {
            if (err) {
                return res.status(500).json({ error: err })
            }
            res.json({iban: iban, message: "Withdrawal successful"})
        })
    })
})

app.listen(port, () => {
    console.log(`server op poort: ${port}`)
})

function checkJson(request, res) {
    // check if keys are empty
    for (const [key, value] of Object.entries(request)) {
        if (!value) {
            res.status(400).json({ error: `Missing or empty field: ${key}` })
            return false 
        }
    }
    
    // check for sql-injection
    for (const [key, value] of Object.entries(request)) {
        if (Number.isInteger(value)) {
            continue
        }
        if (value.includes("'")) {
            res.status(500).json({error: "nice SQL"})
            return false 
        }
    }

    return request
}