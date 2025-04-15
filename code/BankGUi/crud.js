require('dotenv').config({path:'./conf.env'});
const mysql = require('mysql2');


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

// --- CREATE ---
app.post('/users', (req, res) => {
    const { name, email } = req.body;
    db.query('INSERT INTO users (name, email) VALUES (?, ?)', [name, email], (err, result) => {
      if (err) return res.status(500).json({ error: err });
      res.status(201).json({ id: result.insertId, name, email });
    });
  });
  
  // --- READ ALL ---
  app.get('/users', (req, res) => {
    db.query('SELECT * FROM users', (err, results) => {
      if (err) return res.status(500).json({ error: err });
      res.json(results);
    });
  });
  
  // --- READ ONE ---
  app.get('/users/:id', (req, res) => {
    db.query('SELECT * FROM users WHERE id = ?', [req.params.id], (err, results) => {
      if (err) return res.status(500).json({ error: err });
      res.json(results[0]);
    });
  });
  
  // --- UPDATE ---
  app.put('/users/:id', (req, res) => {
    const { name, email } = req.body;
    db.query('UPDATE users SET name = ?, email = ? WHERE id = ?', [name, email, req.params.id], (err) => {
      if (err) return res.status(500).json({ error: err });
      res.json({ message: 'User updated' });
    });
  });
  
  // --- DELETE ---
  app.delete('/users/:id', (req, res) => {
    db.query('DELETE FROM users WHERE id = ?', [req.params.id], (err) => {
      if (err) return res.status(500).json({ error: err });
      res.json({ message: 'User deleted' });
    });
  });