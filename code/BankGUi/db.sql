CREATE DATABASE IF NOT EXISTS SympleDB;
USE SympleDB;

CREATE Table klanten (
    id INT PRIMARY KEY AUTO_INCREMENT,
    voornaam VARCHAR(30) NOT NULL,
    achternaam VARCHAR(50) NOT NULL,
    email VARCHAR(100) NOT NULL,
    postcode VARCHAR(6) NOT NULL,
    huisnummer VARCHAR(10) NOT NULL
);

CREATE Table rekeningen (
    iban VARCHAR(18) PRIMARY KEY NOT NULL,
    klantid int FOREIGN KEY REFERENCES klanten(id),
    pin int NOT NULL,
    saldo FLOAT NOT NULL,
    valuta VARCHAR(4) NOT NULL
);