-- Criação da Base de Dados "Sport Org" -- 

CREATE SCHEMA IF NOT EXISTS sportorg;
USE sportorg;

-- Apagar a base de dados
-- DROP SCHEMA sportorg; 


-- Criação da tabela "Morada"
CREATE TABLE IF NOT EXISTS Morada(
	IdMorada INT AUTO_INCREMENT,
    Rua VARCHAR(45) NOT NULL,
    NrPorta INT,
    CodPostal VARCHAR(45) NOT NULL,
    PRIMARY KEY(IdMorada)
);
-- Remover a tabela "Morada"
-- DROP TABLE Morada;


-- Criação da tabela "Universidade"
CREATE TABLE IF NOT EXISTS Universidade(
    IdUniversidade INT AUTO_INCREMENT,
    NomeUniversidade VARCHAR(45) NOT NULL,
    Contacto INT NOT NULL,
    Pais VARCHAR(50) NOT NULL,
    IdMorada INT NOT NULL,
    PRIMARY KEY(IdUniversidade),
    FOREIGN KEY (IdMorada) REFERENCES Morada(IdMorada)
);
-- Remover a tabela "Universidade"
-- DROP TABLE Universidade;

-- Criação da tabela "Atleta"
CREATE TABLE IF NOT EXISTS Atleta(
	IdAtleta INT AUTO_INCREMENT,
    NomeAtleta VARCHAR(75) NOT NULL,
	Email VARCHAR(75) NOT NULL ,
	Contacto INT NOT NULL ,
    Genero VARCHAR(20) NOT NULL,
	Data_Nascimento DATE NOT NULL,
    Nacionalidade VARCHAR(50) NOT NULL,
    IdUniversidade INT NOT NULL,
    IdMorada INT NOT NULL,
    PRIMARY KEY(IdAtleta),
    FOREIGN KEY (IdUniversidade) REFERENCES Universidade(IdUniversidade),
    FOREIGN KEY (IdMorada) REFERENCES Morada(IdMorada)
);
-- Remover a tabela "Atleta"
-- DROP TABLE Atleta;

-- Criação da tabela "Evento"
CREATE TABLE IF NOT EXISTS Evento(
    IdEvento INT AUTO_INCREMENT,
    NomeEvento VARCHAR(45) NOT NULL,
    Locall VARCHAR(60) NOT NULL,
    Data_Hora DATETIME NOT NULL,
    PRIMARY KEY(IdEvento)
);
-- Remover a tabela "Evento"
-- DROP TABLE Evento;

-- Criação da tabela "Staff"
CREATE TABLE IF NOT EXISTS Staff(
    IdStaff INT AUTO_INCREMENT,
    NomeStaff VARCHAR(45) NOT NULL,
    Horario VARCHAR(60) NOT NULL,
    Tipo VARCHAR(20) NOT NULL,
    Contacto INT NOT NULL,
    Email VARCHAR(75),
    PRIMARY KEY(IdStaff)
);
-- Remover a tabela "Staff"
-- DROP TABLE Staff;

-- Criação da tabela "Modalidade"
CREATE TABLE IF NOT EXISTS Modalidade(
    IdModalidade INT AUTO_INCREMENT,
    NomeModalidade VARCHAR(45) NOT NULL,
    Descricao VARCHAR(100) NOT NULL,
    Genero VARCHAR(20) NOT NULL,
    Regras VARCHAR(100) NOT NULL,
    PRIMARY KEY(IdModalidade)
);
-- Remover a tabela "Modalidade"
-- DROP TABLE Modalidade;

-- Criação da tabela "Treinador"
CREATE TABLE IF NOT EXISTS Treinador(
    IdTreinador INT AUTO_INCREMENT,
    NomeTreinador VARCHAR(45) NOT NULL,
    Contacto INT NOT NULL,
    Email VARCHAR(75) NOT NULL,
    PRIMARY KEY(IdTreinador)
);
-- Remover a tabela "Treinador"
-- DROP TABLE Treinador;

-- Criação da tabela do relacionamento "Atleta - Evento" 
CREATE TABLE IF NOT EXISTS Atleta_Evento (
    IdAtleta INT,
    IdEvento INT,
    PRIMARY KEY (IdAtleta, IdEvento),
    FOREIGN KEY (IdAtleta) REFERENCES Atleta(IdAtleta),
    FOREIGN KEY (IdEvento) REFERENCES Evento(IdEvento)
);
-- Remover a tabela "Atleta - Evento
-- DROP TABLE Atleta_Evento;


-- Criação da tabela do relacionamento "Evento - Staff"
CREATE TABLE IF NOT EXISTS Evento_Staff(
    IdEvento INT,
    IdStaff INT,
    PRIMARY KEY (IdEvento, IdStaff),
    FOREIGN KEY (IdEvento) REFERENCES Evento(IdEvento),
    FOREIGN KEY (IdStaff) REFERENCES Staff(IdStaff)
);
-- Remover a tabela "Evento - Staff"
-- DROP TABLE Evento_Staff;

-- Criação da tabela do relacionamento "Atleta - Modalidade"
CREATE TABLE IF NOT EXISTS Atleta_Modalidade(
    IdAtleta INT,
    IdModalidade INT,
    PRIMARY KEY (IdAtleta, IdModalidade),
    FOREIGN KEY (IdAtleta) REFERENCES Atleta(IdAtleta),
    FOREIGN KEY (IdModalidade) REFERENCES Modalidade(IdModalidade)
);
-- Remover a tabela "Atleta - Modalidade"
-- DROP TABLE Atleta_Modalidade;

-- Criação da tabela do relacionamento "Modalidade - Treinador"
CREATE TABLE IF NOT EXISTS Modalidade_Treinador(
	IdModalidade INT,
    IdTreinador INT,
    PRIMARY KEY (IdModalidade, IdTreinador),
    FOREIGN KEY (IdModalidade) REFERENCES Modalidade(IdModalidade),
    FOREIGN KEY (IdTreinador) REFERENCES Treinador(IdTreinador)
);
-- Remover a tabela "Modalidade - Treinador"
-- DROP TABLE Modalidade_Treinador;