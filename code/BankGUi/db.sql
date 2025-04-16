CREATE DATABASE IF NOT EXISTS SympleDB;
USE SympleDB;

CREATE TABLE klanten (
    id INT PRIMARY KEY AUTO_INCREMENT,
    voornaam VARCHAR(30) NOT NULL,
    achternaam VARCHAR(50) NOT NULL,
    email VARCHAR(100) NOT NULL,
    postcode VARCHAR(6) NOT NULL,
    huisnummer VARCHAR(10) NOT NULL
);

CREATE TABLE rekeningen (
    iban VARCHAR(18) PRIMARY KEY NOT NULL,
    klantid int,
    pin int NOT NULL,
    saldo FLOAT NOT NULL,
    valuta VARCHAR(4) NOT NULL,
    FOREIGN KEY (klantid) REFERENCES klanten(id)
);

CREATE TABLE transacties (
    id int PRIMARY KEY AUTO_INCREMENT,
    iban VARCHAR(18),
    bedrag float NOT NULL,
    valuta VARCHAR(4) NOT NULL,
    datum DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (iban) REFERENCES rekeningen(iban)
)