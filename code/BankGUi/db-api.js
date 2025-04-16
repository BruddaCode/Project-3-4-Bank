require('dotenv').config({path:'./conf.env'});
const mysql = require('mysql2');
const express = require('express');
const app = express();
const port = 9000;

const db = mysql.createConnection({
    host: process.env.DB_HOST,
    port: process.env.DB_PORT,
    user: process.env.DB_USER,
    password: process.env.DB_PASS,
    database: process.env.DB_NAME
});

db.connect(err => {
    if (err) {
        console.error('Connection failed:', err.stack);
        return;
    }
    console.log('Connected as id', db.threadId);
});

app.use(express.json());

app.get('/api/noob/health', (req, res) => {
    res.json({status: "OK"});
});

// --- READ ONE ---
app.post('/api/noob/users/getinfo', (req, res) => {
    try {
        var iban = req.body.iban;
        var pin = req.body.pin;
    }
    catch {
        res.json({error: "bad request"});
    }

    // check for sql-injection
    // get data from db
    // send back json
    

    
    // res.json({data: req.body});
});

// --- UPDATE ---
app.put('/users/:id', (req, res) => {
    const { name, email } = req.body;
    db.query('UPDATE users SET name = ?, email = ? WHERE id = ?', [name, email, req.params.id], (err) => {
        if (err) return res.status(500).json({ error: err });
        res.json({ message: 'User updated' });
    });
});

app.listen(port, () => {
    console.log(`server op poort: ${port}`);
});