USE sportorg;

INSERT INTO Morada (IdMorada, Rua, NrPorta, CodPostal)
VALUES
	(1,'Rua da Universidade', NULL, '4710-057'),
	(2,'Massachusetts Avenue', NULL, '02138'),
	(3,'The Old Schools, Trinity Lane', NULL, 'CB2 1TN'),
	(4,'7-3-1 Hongo, Bunkyo-ku', NULL, '113-8654'),
	(5,'5 Yiheyuan Road, Haidian District', NULL, '100871'),
    (6, '21 Lower Kent Ridge Road', NULL, '119077'),
    (7, 'Geschwister-Scholl-Platz', 1, '80539'),
    (8, 'Avenida da Liberdade', 45, '1250-096'),
    (9, 'Rua 5 de Outubro', 12, '8000-076'),
    (10, 'Praça Marquês de Pombal', 18, '1250-162'),
    (11, 'Calle de Alcalá', 50, '28014'),
    (12, 'Avenue des Champs-Élysées', 31, '75008'),
    (13, 'Unter den Linden', 21, '10117'),
    (14, 'Via della Conciliazione', 4, '00193'),
    (15, 'Westminster Bridge Road', 31, 'SE1 7PB'),
    (16, 'Broadway', 1633, '10019'),
    (17, 'Yonge Street', 2500, 'M4P 2C9'),
    (18, 'Collins Street', 101, '3000'),
    (19, 'Nanjing Road', 88, '200001'),
    (20, 'Orchard Road', 15, '238841'),
    (21, 'Passeig de Gràcia', 7, '08007'),
    (22, 'Gran Vía', 56, '28013'),
    (23, 'Nathan Road', 320, '00000'),
    (24, 'Abbey Road', 3, 'NW8 9AY'),
    (25, 'Hollywood Boulevard', 6801, '90028'),
    (26, 'Market Street', 425, '94105'),
    (27, 'Rodeo Drive', 248, '90210'),
    (28, 'Fifth Avenue', 727, '10022'),
    (29, 'Wall Street', 11, '10005'),
    (30, 'Park Lane', 22, 'W1K 1BE'),
    (31, 'Oxford Street', 134, 'W1D 1LL'),
    (32, 'Queen Street', 45, '2000'),
    (33, 'Flinders Street', 3, '3000'),
    (34, 'Königsallee', 45, '40212'),
    (35, 'Ginza', 8, '104-0061'),
    (36, 'Rua Augusta', 100, '1100-048'),
    (37, 'Rua Santa Catarina', 500, '4000-447'),
    (38, 'Avenida Paulista', 1578, '01310-200'),
    (39, 'Avenida Atlântica', 2500, '22070-002'),
    (40, 'Rua XV de Novembro', 150, '80020-310'),
    (41, 'Avenida Alvear', 1891, '1129'),
    (42, 'Corrientes Avenue', 3480, 'C1043'),
    (43, 'Rua Oscar Freire', 1100, '01426-001'),
    (44, 'Calle Florida', 300, '1005'),
    (45, 'Avenida 9 de Julio', 1500, 'C1043'),
    (46, 'Praça dos Restauradores', 21, '1250-188'),
    (47, 'Rua Dom Manuel II', 55, '4050-346'),
    (48, 'Rua do Carmo', 78, '1200-092'),
    (49, 'Avenida dos Aliados', 90, '4000-064'),
    (50, 'Avenida Marechal Craveiro Lopes', 500, '1700-284'),
    (51, 'Praça da Figueira', 12, '1100-241'),
    (52, 'Rua das Flores', 32, '4050-262'),
    (53, 'Rua da Imprensa Nacional', 128, '1250-127'),
    (54, 'Travessa do Carmo', 15, '1200-092'),
    (55, 'Largo do Chiado', 5, '1200-108'),
    (56, 'Rua Garrett', 24, '1200-309'),
    (57, 'Rua do Ouro', 10, '1100-380'),
    (58, 'Rua do Alecrim', 42, '1200-014'),
    (59, 'Rua de São Bento', 203, '1200-109'),
    (60, 'Calçada da Estrela', 152, '1200-667');
-- TRUNCATE TABLE Morada;

INSERT INTO Universidade (IdUniversidade, NomeUniversidade, Contacto, Pais, IdMorada)
VALUES 
  (1,'Universidade do Minho',253601109,'Portugal',1),
  (2,'Universidade de Harvard',255601109,'Estados Unidos',2),
  (3,'Universidade de Cambridge',257601109,'Inglaterra',3),
  (4,'Universidade de Tokyo',259601109,'Japão',4),
  (5,'Universidade de Peking',260601109,'China',5),
  (6,'Universidade Nacional de Singapura',263601109,'Singapura',6),
  (7,'Universidade de Munich',258601109,'Alemanha',7);
-- TRUNCATE TABLE Universidade;
  
    
INSERT INTO Evento (IdEvento, NomeEvento, Locall, Data_Hora)
VALUES
(1,'Natação Feminina','Ginásio 5','2024-06-12 15:30:00'),
(2,'Natação Masculina','Ginásio 5','2024-06-13 15:00:00'),
(3,'Volleyball Feminina','Ginásio 7','2024-06-14 09:00:00'),
(4,'Volleyball Masculino','Ginásio 7','2024-06-15 10:30:00'),
(5,'BasketBall Feminino','Ginásio 1','2024-06-16 15:30:00'),
(6,'BasketBall Masculino','Ginásio 2','2024-06-17 15:30:00'),
(7,'Atletismo 100m Masculino','Estádio Olímpico','2024-06-18 10:00:00'),
(8,'Atletismo 200m Masculino','Estádio Olímpico','2024-06-18 11:00:00'),
(9,'Futebol Feminino','Campo Principal','2024-06-19 14:00:00'),
(10,'Futebol Masculino','Campo Principal','2024-06-19 16:00:00'),
(11,'Hóquei Feminino','Ginásio 2','2024-06-20 12:00:00'),
(12,'Hóquei Masculino','Ginásio 2','2024-06-20 14:00:00'),
(13,'Ginástica Artística Feminina','Ginásio 3','2024-06-21 09:00:00'),
(14,'Ginástica Artística Masculina','Ginásio 3','2024-06-21 11:00:00'),
(15,'Ténis de Mesa Feminino','Ginásio 1','2024-06-22 15:00:00'),
(16,'Ténis de Mesa Masculino','Ginásio 1','2024-06-22 17:00:00'),
(17,'Atletismo 100m Feminino','Estádio Olímpico','2024-06-18 15:00:00'),
(18,'Atletismo 200m Feminino','Estádio Olímpico','2024-06-18 16:00:00');
-- TRUNCATE TABLE Evento;
    
    
INSERT INTO Atleta (IdAtleta, NomeAtleta, Email, Contacto, Genero, Data_Nascimento, Nacionalidade, IdUniversidade, IdMorada)
VALUES
(1, 'Ana Maria Costa', 'anamariacosta@gmail.com', '934567892', 'F', '2003-06-02', 'Portugues', 1, 8),
(2, 'João Pedro Silva', 'joaopedrosilva@gmail.com', '932654789', 'M', '1999-11-15', 'Portugues', 1, 9),
(3, 'Emily Johnson', 'emilyjohnson@gmail.com', '931245678', 'F', '2000-04-20', 'Americano', 2, 10),
(4, 'Takeshi Yamamoto', 'takeshiyamamoto@gmail.com', '935876432', 'M', '2002-08-14', 'Japones', 4, 11),
(5, 'Lara Schmidt', 'laraschmidt@gmail.com', '936543219', 'F', '2001-02-10', 'Alemao', 7, 12),
(6, 'Michael Brown', 'michaelbrown@gmail.com', '937654321', 'M', '1998-06-30', 'Ingles', 3, 13),
(7, 'Chun Hei', 'chunhei@gmail.com', '934321678', 'F', '2004-09-25', 'Chines', 5, 14),
(8, 'Sing Wei', 'singwei@gmail.com', '939876543', 'M', '2005-03-12', 'Singapurense', 6, 15),
(9, 'Sofia Martins', 'sofiamartins@gmail.com', '938765432', 'F', '1998-12-05', 'Portugues', 1, 16),
(10, 'David Anderson', 'davidanderson@gmail.com', '930987654', 'M', '2003-07-18', 'Americano', 2, 17),
(11, 'Pedro Almeida', 'pedroalmeida@gmail.com', '930123456', 'M', '2001-05-21', 'Portugues', 1, 18),
(12, 'Maria Fernandes', 'mariafernandes@gmail.com', '931234567', 'F', '2000-03-11', 'Portugues', 1, 19),
(13, 'Kevin Lee', 'kevinlee@gmail.com', '932345678', 'M', '2002-10-09', 'Singapurense', 6,20),
(14, 'Laura Taylor', 'laurataylor@gmail.com', '933456789', 'F', '1998-07-30', 'Ingles', 3, 21),
(15, 'Hiroshi Tanaka', 'hiroshitanaka@gmail.com', '934567890', 'M', '1999-04-15', 'Japones', 4, 22),
(16, 'Emma Müller', 'emmamuller@gmail.com', '935678901', 'F', '2004-02-28', 'Alemao', 7, 23),
(17, 'Lucas Oliveira', 'lucasoliveira@gmail.com', '936789012', 'M', '2003-01-19', 'Portugues', 1, 24),
(18, 'Aiko Nakamura', 'aikonakamura@gmail.com', '937890123', 'F', '2000-11-05', 'Japones', 4, 25),
(19, 'James Smith', 'jamessmith@gmail.com', '938901234', 'M', '2002-08-18', 'Americano', 2, 26),
(20, 'Sophia Wang', 'sophiawang@gmail.com', '939012345', 'F', '2001-03-27', 'Chines', 5, 27),
(21, 'Victor Hugo', 'victorhugo@gmail.com', '930234567', 'M', '2005-09-15', 'Portugues', 1, 28),
(22, 'Carla Nunes', 'carla.nunes@gmail.com', '931345678', 'F', '1999-10-22', 'Portugues', 1, 29),
(23, 'Daniel Cooper', 'danielcooper@gmail.com', '932456789', 'M', '1998-12-11', 'Ingles', 3, 30),
(24, 'Megan White', 'meganwhite@gmail.com', '933567890', 'F', '2000-06-30', 'Americano', 2, 31),
(25, 'Jun Park', 'junpark@gmail.com', '934678901', 'M', '2003-04-14', 'Japones', 4, 32),
(26, 'Lina Santos', 'linasantos@gmail.com', '935789012', 'F', '2001-07-22', 'Portugues', 1, 33),
(27, 'Oliver Davis', 'oliverdavis@gmail.com', '936890123', 'M', '2004-02-10', 'Ingles', 3, 34),
(28, 'Hana Kimura', 'hanakimura@gmail.com', '937901234', 'F', '1998-03-25', 'Japones', 4, 35),
(29, 'Antonio Rocha', 'antoniorocha@gmail.com', '938012345', 'M', '2005-08-19', 'Portugues', 1, 36),
(30, 'Claire Dupont', 'claired@gmail.com', '939123456', 'F', '2000-09-12', 'Alemao', 7, 37),
(31, 'Carlos Mendes', 'carlosmendes@gmail.com', '930345678', 'M', '2001-12-03', 'Portugues', 1, 38),
(32, 'Aline Costa', 'alinecosta@gmail.com', '931456789', 'F', '2004-05-15', 'Portugues', 1, 39),
(33, 'Nathan Thomas', 'nathanthomas@gmail.com', '932567890', 'M', '2002-10-28', 'Americano', 2, 40),
(34, 'Isabella Lewis', 'isabellalewis@gmail.com', '933678901', 'F', '1999-06-17', 'Ingles', 3, 41),
(35, 'Yu Zhang', 'yuzhang@gmail.com', '934789012', 'M', '1998-11-30', 'Chines', 5, 42),
(36, 'Sara Müller', 'saramuller@gmail.com', '935890123', 'F', '2003-01-22', 'Alemao', 7, 43),
(37, 'Tomás Fonseca', 'tomasfonseca@gmail.com', '936901234', 'M', '2005-07-09', 'Portugues', 1, 44),
(38, 'Noah Wilson', 'noahwilson@gmail.com', '937012345', 'M', '2000-03-14', 'Americano', 2, 45),
(39, 'Mia Clarke', 'miaclarke@gmail.com', '938123456', 'F', '2004-11-11', 'Ingles', 3, 46),
(40, 'Satoshi Nakamoto', 'satoshinakamoto@gmail.com', '939234567', 'M', '1999-09-09', 'Japones', 4, 47),
(41, 'Amanda Costa', 'amandacosta@gmail.com', '930456789', 'F', '2002-06-01', 'Portugues', 1, 48),
(42, 'Leonardo Ribeiro', 'leonardoribeiro@gmail.com', '931567890', 'M', '2003-10-10', 'Portugues', 1, 49),
(43, 'Clara Novak', 'claranovak@gmail.com', '932678901', 'F', '2001-08-13', 'Alemao', 7, 50),
(44, 'Robert Walker', 'robertwalker@gmail.com', '933789012', 'M', '2000-12-18', 'Americano', 2, 51),
(45, 'Yuki Saito', 'yukisaito@gmail.com', '934890123', 'F', '1998-01-25', 'Japones', 4, 52),
(46, 'Diego Martinez', 'diegomartinez@gmail.com', '935901234', 'M', '2005-04-16', 'Americano', 2, 53),
(47, 'Emma Watson', 'emmawatson@gmail.com', '936012345', 'F', '2004-08-21', 'Ingles', 3, 54),
(48, 'Sebastian Kurz', 'sebatiankurz@gmail.com', '937123456', 'M', '2002-11-02', 'Alemao', 7, 55),
(49, 'Hua Li', 'huali@gmail.com', '938234567', 'F', '1999-05-20', 'Chines', 5, 56),
(50, 'Alex Carter', 'alexcarter@gmail.com', '939345678', 'M', '1998-07-07', 'Americano', 2, 57);
-- TRUNCATE TABLE Atleta;


INSERT INTO Staff (IdStaff, NomeStaff, Horario, Tipo, Contacto, Email)
VALUES
(1, 'Ricardo Pereira', '8H-16H', 'Segurança', '923567890', 'ricardopereira@gmail.com'),
(2, 'Beatriz Mendes', '9H-18H', 'Recepcionista', '924678901', 'beatrizmendes@gmail.com'),
(3, 'João Carvalho', '7H-15H', 'Técnico', '925789012', 'joaocarvalho@gmail.com'),
(4, 'Patrícia Gomes', '12H-20H', 'Limpeza', '926890123', 'patriciagomes@gmail.com'),
(5, 'Miguel Fonseca', '6H-14H', 'Manutenção', '927901234', 'miguelfonseca@gmail.com'),
(6, 'Carolina Dias', '14H-22H', 'Funcionária', '928012345', 'carolinadias@gmail.com'),
(7, 'Fábio Silva', '10H-18H', 'Auxiliar', '929123456', 'fabiosilva@gmail.com'),
(8, 'Sara Fernandes', '15H-23H', 'Segurança', '930234567', 'sarafernandes@gmail.com'),
(9, 'André Matos', '9H-17H', 'Técnico', '931345678', 'andrematos@gmail.com'),
(10, 'Diana Oliveira', '13H-21H', 'Recepcionista', '932456789', 'dianaoliveira@gmail.com'),
(11, 'Rafael Nogueira', '8H-16H', 'Manutenção', '933567890', 'rafaelnogueira@gmail.com'),
(12, 'Mariana Costa', '9H-18H', 'Limpeza', '934678901', 'marianacosta@gmail.com'),
(13, 'Pedro Lopes', '7H-15H', 'Segurança', '935789012', 'pedrolopes@gmail.com'),
(14, 'Camila Rocha', '12H-20H', 'Auxiliar', '936890123', 'camilarocha@gmail.com'),
(15, 'Bruno Tavares', '10H-18H', 'Funcionário', '937901234', 'brunotavares@gmail.com');
-- TRUNCATE TABLE Staff;


INSERT INTO Modalidade (IdModalidade, NomeModalidade, Descricao, Genero, Regras)
VALUES
(1, 'Natação', 'Prova de velocidade em diferentes estilos de nado.', 'F', 'Cada prova segue as regras da FINA. É permitido apenas um toque na borda nas viradas.'),
(2, 'Natação', 'Prova de velocidade em diferentes estilos de nado.', 'M', 'Cada prova segue as regras da FINA. É permitido apenas um toque na borda nas viradas.'),
(3, 'Volleyball', 'Jogo de equipe onde o objetivo é fazer a bola tocar o chão do adversário.', 'F', 'Cada equipe pode tocar na bola no máximo 3 vezes antes de devolvê-la.'),
(4, 'Volleyball', 'Jogo de equipe onde o objetivo é fazer a bola tocar o chão do adversário.', 'M', 'Cada equipe pode tocar na bola no máximo 3 vezes antes de devolvê-la.'),
(5, 'Basketball', 'Jogo de equipe onde o objetivo é marcar pontos arremessando a bola na cesta adversária.', 'F', 'Segue as regras da FIBA, com 4 períodos de 10 minutos.'),
(6, 'Basketball', 'Jogo de equipe onde o objetivo é marcar pontos arremessando a bola na cesta adversária.', 'M', 'Segue as regras da FIBA, com 4 períodos de 10 minutos.'),
(7, 'Atletismo 100m', 'Prova de velocidade em uma pista de 100 metros.', 'F', 'A largada é feita com bloco e o tempo é cronometrado com precisão de milissegundos.'),
(8, 'Atletismo 100m', 'Prova de velocidade em uma pista de 100 metros.', 'M', 'A largada é feita com bloco e o tempo é cronometrado com precisão de milissegundos.'),
(9, 'Atletismo 200m', 'Prova de velocidade em uma pista de 200 metros.', 'F', 'Os atletas devem permanecer na sua raia durante toda a prova.'),
(10, 'Atletismo 200m', 'Prova de velocidade em uma pista de 200 metros.', 'M', 'Os atletas devem permanecer na sua raia durante toda a prova.'),
(11, 'Futebol', 'Jogo de equipe onde o objetivo é marcar gols no adversário.', 'F', 'Segue as regras da FIFA, com 90 minutos de partida divididos em 2 tempos de 45 minutos.'),
(12, 'Futebol', 'Jogo de equipe onde o objetivo é marcar gols no adversário.', 'M', 'Segue as regras da FIFA, com 90 minutos de partida divididos em 2 tempos de 45 minutos.'),
(13, 'Hóquei', 'Jogo de equipe onde o objetivo é marcar gols com um taco.', 'F', 'Segue as regras da FIH, com 4 períodos de 15 minutos cada.'),
(14, 'Hóquei', 'Jogo de equipe onde o objetivo é marcar gols com um taco.', 'M', 'Segue as regras da FIH, com 4 períodos de 15 minutos cada.'),
(15, 'Ginástica', 'Competição de habilidades artísticas e acrobáticas.', 'F', 'Segue as normas da FIG. Cada atleta é avaliado por dificuldade e execução.'),
(16, 'Ginástica', 'Competição de habilidades artísticas e acrobáticas.', 'M', 'Segue as normas da FIG. Cada atleta é avaliado por dificuldade e execução.'),
(17, 'Ténis de Mesa', 'Jogo individual ou em duplas com raquetes e bola em uma mesa.', 'F', 'Segue as regras da ITTF. A partida é decidida em melhor de 5 sets.'),
(18, 'Ténis de Mesa', 'Jogo individual ou em duplas com raquetes e bola em uma mesa.', 'M', 'Segue as regras da ITTF. A partida é decidida em melhor de 5 sets.');
-- TRUNCATE TABLE Modalidade;

INSERT INTO Treinador (IdTreinador, NomeTreinador, Contacto, Email)
VALUES
(1, 'João Silva', '912345678', 'joaosilva@gmail.com'),
(2, 'Emily Johnson', '932478152', 'emilyjohnson@gmail.com'),
(3, 'David Williams', '954672839', 'davidwilliams@gmail.com'),
(4, 'Hiroshi Tanaka', '961234876', 'hiroshitanaka@gmail.com'),
(5, 'Li Wei', '919876543', 'liwei@gmail.com'),
(6, 'Hans Müller', '930123456', 'hansmuller@gmail.com'),
(7, 'Siti Zainab', '917654321', 'sitzainab@gmail.com'),
(8, 'James Brown', '911234567', 'jamesbrown@gmail.com'),
(9, 'Maria Fernandes', '924567890', 'mariafernandes@gmail.com'),
(10, 'Oliver Smith', '937845210', 'oliversmith@gmail.com'),
(11, 'Takumi Yamamoto', '905612348', 'takumiyamamoto@gmail.com'),
(12, 'Chen Mei', '963852741', 'chenmei@gmail.com'),
(13, 'Rebecca Taylor', '933456789', 'rebeccataylor@gmail.com'),
(14, 'Pedro Costa', '929876543', 'pedrocosta@gmail.com'),
(15, 'Ryuji Suzuki', '922334567', 'ryujisuzuki@gmail.com');
-- TRUNCATE TABLE Treinador;


INSERT INTO Atleta_Evento (IdAtleta, IdEvento)
VALUES
-- Natação Feminina
(1, 1),  
(3, 1),  
(5, 1), 
(9, 1),  
(12, 1), 
(14, 1), 
(16, 1),
(18, 1), 
(20, 1), 

-- Natação Masculina
(2, 2),
(6, 2), 
(10, 2),
(11, 2), 
(13, 2),
(15, 2), 
(17, 2), 
(19, 2),
(21, 2), 

-- Volleyball Feminina
(22, 3), 
(24, 3),  
(26, 3),  
(28, 3),  
(30, 3),
(32, 3), 
(34, 3),
(36, 3), 
(39, 3), 

-- Volleyball Masculina
(23, 4),  
(25, 4),  
(27, 4),  
(29, 4), 
(31, 4),
(33, 4), 
(35, 4), 
(37, 4), 
(38, 4),

-- Basketball Feminino
(41, 5), 
(43, 5),  
(45, 5),  
(47, 5), 
(49, 5),
(1, 5), 

-- Basketball Masculino
(40, 6),  
(42, 6),  
(44, 6),
(46, 6), 
(48, 6), 
(50, 6), 
(2, 6), 

-- Atletismo 100m Masculino
(4, 7), 
(6, 7),  
(8, 7), 
(10, 7), 
(11, 7), 
(13, 7),

-- Atletismo 200m Masculino
(15, 8), 
(17, 8),  
(19, 8),
(21, 8), 
(23, 8), 
(25, 8), 

-- Futebol Feminino
(3, 9), 
(5, 9),  
(7, 9), 
(9, 9), 
(12, 9), 
(14, 9), 

-- Futebol Masculino
(27, 10), 
(29, 10), 
(31, 10), 
(33, 10), 
(35, 10), 
(37, 10), 

-- Hóquei Feminino
(16, 11), 
(18, 11), 
(20, 11), 
(22, 11), 
(24, 11), 
(26, 11), 

-- Hóquei Masculino
(38, 12), 
(40, 12), 
(42, 12), 
(44, 12), 
(46, 12),
(48, 12), 
(50, 12), 

-- Ginástica Artística Feminina
(28, 13), 
(30, 13),
(32, 13), 
(34, 13), 

-- Ginástica Artística Masculina
(2, 14), 
(4, 14), 
(6, 14), 
(8, 14), 

-- Ténis de Mesa Feminino
(36, 15), 
(39, 15), 
(41, 15), 
(43, 15), 

-- Ténis de Mesa Masculino
(10, 16), 
(11, 16), 
(13, 16), 
(15, 16), 

-- Atletismo 100m Feminino
(45, 17), 
(47, 17), 
(49, 17), 
(1, 17), 

-- Atletismo 200m Feminino
(3, 18), 
(5, 18), 
(7, 18), 
(9, 18);
-- TRUNCATE TABLE Atleta_Evento;

INSERT INTO Evento_Staff (IdEvento, IdStaff)
VALUES
-- Natação Feminina
(1, 1),  -- Ricardo Pereira (Segurança)
(1, 2),  -- Beatriz Mendes (Recepcionista)
(1, 3),  -- João Carvalho (Técnico)
(1, 4),  -- Patrícia Gomes (Limpeza)
(1, 5),  -- Miguel Fonseca (Manutenção)
(1, 7),  -- Fábio Silva (Auxiliar)
(1, 8),  -- Sara Fernandes (Segurança)

-- Natação Masculina
(2, 1),  -- Ricardo Pereira (Segurança)
(2, 2),  -- Beatriz Mendes (Recepcionista)
(2, 3),  -- João Carvalho (Técnico)
(2, 4),  -- Patrícia Gomes (Limpeza)
(2, 5),  -- Miguel Fonseca (Manutenção)
(2, 7),  -- Fábio Silva (Auxiliar)
(2, 8),  -- Sara Fernandes (Segurança)

-- Volleyball Feminina
(3, 1),  -- Ricardo Pereira (Segurança)
(3, 2),  -- Beatriz Mendes (Recepcionista)
(3, 3),  -- João Carvalho (Técnico)
(3, 4),  -- Patrícia Gomes (Limpeza)
(3, 5),  -- Miguel Fonseca (Manutenção)
(3, 6),  -- Carolina Dias (Funcionária)
(3, 7),  -- Fábio Silva (Auxiliar)

-- Volleyball Masculino
(4, 1),  -- Ricardo Pereira (Segurança)
(4, 2),  -- Beatriz Mendes (Recepcionista)
(4, 3),  -- João Carvalho (Técnico)
(4, 4),  -- Patrícia Gomes (Limpeza)
(4, 5),  -- Miguel Fonseca (Manutenção)
(4, 6),  -- Carolina Dias (Funcionária)
(4, 7),  -- Fábio Silva (Auxiliar)


-- Basketball Feminino
(5, 1),  -- Ricardo Pereira (Segurança)
(5, 2),  -- Beatriz Mendes (Recepcionista)
(5, 3),  -- João Carvalho (Técnico)
(5, 4),  -- Patrícia Gomes (Limpeza)
(5, 5),  -- Miguel Fonseca (Manutenção)
(5, 7),  -- Fábio Silva (Auxiliar)
(5, 8),  -- Sara Fernandes (Segurança)

-- Basketball Masculino
(6, 1),  -- Ricardo Pereira (Segurança)
(6, 2),  -- Beatriz Mendes (Recepcionista)
(6, 3),  -- João Carvalho (Técnico)
(6, 4),  -- Patrícia Gomes (Limpeza)
(6, 5),  -- Miguel Fonseca (Manutenção)
(6, 7),  -- Fábio Silva (Auxiliar)
(6, 8),  -- Sara Fernandes (Segurança)

-- Atletismo 100m Masculino
(7, 1),  -- Ricardo Pereira (Segurança)
(7, 3),  -- João Carvalho (Técnico)
(7, 4),  -- Patrícia Gomes (Limpeza)
(7, 5),  -- Miguel Fonseca (Manutenção)
(7, 7),  -- Fábio Silva (Auxiliar)

-- Atletismo 200m Masculino
(8, 1),  -- Ricardo Pereira (Segurança)
(8, 3),  -- João Carvalho (Técnico)
(8, 4),  -- Patrícia Gomes (Limpeza)
(8, 5),  -- Miguel Fonseca (Manutenção)
(8, 7),  -- Fábio Silva (Auxiliar)

-- Futebol Feminino
(9, 1),  -- Ricardo Pereira (Segurança)
(9, 2),  -- Beatriz Mendes (Recepcionista)
(9, 3),  -- João Carvalho (Técnico)
(9, 4),  -- Patrícia Gomes (Limpeza)
(9, 5),  -- Miguel Fonseca (Manutenção)
(9, 6),  -- Carolina Dias (Funcionária)
(9, 7),  -- Fábio Silva (Auxiliar)

-- Futebol Masculino
(10, 1),  -- Ricardo Pereira (Segurança)
(10, 2),  -- Beatriz Mendes (Recepcionista)
(10, 3),  -- João Carvalho (Técnico)
(10, 4),  -- Patrícia Gomes (Limpeza)
(10, 5),  -- Miguel Fonseca (Manutenção)
(10, 6),  -- Carolina Dias (Funcionária)
(10, 7),  -- Fábio Silva (Auxiliar)

-- Hóquei Feminino
(11, 1),  -- Ricardo Pereira (Segurança)
(11, 2),  -- Beatriz Mendes (Recepcionista)
(11, 3),  -- João Carvalho (Técnico)
(11, 4),  -- Patrícia Gomes (Limpeza)
(11, 5),  -- Miguel Fonseca (Manutenção)
(11, 7),  -- Fábio Silva (Auxiliar)

-- Hóquei Masculino
(12, 1),  -- Ricardo Pereira (Segurança)
(12, 2),  -- Beatriz Mendes (Recepcionista)
(12, 3),  -- João Carvalho (Técnico)
(12, 4),  -- Patrícia Gomes (Limpeza)
(12, 5),  -- Miguel Fonseca (Manutenção)
(12, 7),  -- Fábio Silva (Auxiliar)

-- Ginástica Artística Feminina
(13, 1),  -- Ricardo Pereira (Segurança)
(13, 2),  -- Beatriz Mendes (Recepcionista)
(13, 3),  -- João Carvalho (Técnico)
(13, 4),  -- Patrícia Gomes (Limpeza)
(13, 5),  -- Miguel Fonseca (Manutenção)
(13, 7),  -- Fábio Silva (Auxiliar)

-- Ginástica Artística Masculina
(14, 1),  -- Ricardo Pereira (Segurança)
(14, 2),  -- Beatriz Mendes (Recepcionista)
(14, 3),  -- João Carvalho (Técnico)
(14, 4),  -- Patrícia Gomes (Limpeza)
(14, 5),  -- Miguel Fonseca (Manutenção)
(14, 7),  -- Fábio Silva (Auxiliar)

-- Ténis de Mesa Feminino
(15, 1),  -- Ricardo Pereira (Segurança)
(15, 2),  -- Beatriz Mendes (Recepcionista)
(15, 3),  -- João Carvalho (Técnico)
(15, 4),  -- Patrícia Gomes (Limpeza)
(15, 5),  -- Miguel Fonseca (Manutenção)
(15, 7),  -- Fábio Silva (Auxiliar)

-- Ténis de Mesa Masculino
(16, 1),  -- Ricardo Pereira (Segurança)
(16, 2),  -- Beatriz Mendes (Recepcionista)
(16, 3),  -- João Carvalho (Técnico)
(16, 4),  -- Patrícia Gomes (Limpeza)
(16, 5),  -- Miguel Fonseca (Manutenção)
(16, 7),  -- Fábio Silva (Auxiliar)

-- Atletismo 100m Feminino
(17, 1),  -- Ricardo Pereira (Segurança)
(17, 2),  -- Beatriz Mendes (Recepcionista)
(17, 3),  -- João Carvalho (Técnico)
(17, 4),  -- Patrícia Gomes (Limpeza)
(17, 5),  -- Miguel Fonseca (Manutenção)
(17, 7),  -- Fábio Silva (Auxiliar)

-- Atletismo 200m Feminino
(18, 1),  -- Ricardo Pereira (Segurança)
(18, 2),  -- Beatriz Mendes (Recepcionista)
(18, 3),  -- João Carvalho (Técnico)
(18, 4),  -- Patrícia Gomes (Limpeza)
(18, 5),  -- Miguel Fonseca (Manutenção)
(18, 7);  -- Fábio Silva (Auxiliar)
-- TRUNCATE TABLE Evento_Staff;


INSERT INTO Atleta_Modalidade (IdAtleta, IdModalidade)
VALUES

-- Natação Feminina
(1, 1),  
(3, 1),  
(5, 1), 
(9, 1),  
(12, 1), 
(14, 1), 
(16, 1),
(18, 1), 
(20, 1), 

-- Natação Masculina
(2, 2),
(6, 2), 
(10, 2),
(11, 2), 
(13, 2),
(15, 2), 
(17, 2), 
(19, 2),
(21, 2), 

-- Volleyball Feminina
(22, 3), 
(24, 3),  
(26, 3),  
(28, 3),  
(30, 3),
(32, 3), 
(34, 3),
(36, 3), 
(39, 3), 

-- Volleyball Masculina
(23, 4),  
(25, 4),  
(27, 4),  
(29, 4), 
(31, 4),
(33, 4), 
(35, 4), 
(37, 4), 
(38, 4),

-- Basketball Feminino
(41, 5), 
(43, 5),  
(45, 5),  
(47, 5), 
(49, 5),
(1, 5), 

-- Basketball Masculino
(40, 6),  
(42, 6),  
(44, 6),
(46, 6), 
(48, 6), 
(50, 6), 
(2, 6), 

-- Atletismo 100m Masculino
(4, 8), 
(6, 8),  
(8, 8), 
(10, 8), 
(11, 8), 
(13, 8),

-- Atletismo 200m Masculino
(15, 10), 
(17, 10),  
(19, 10),
(21, 10), 
(23, 10), 
(25, 10), 

-- Futebol Feminino
(3, 11), 
(5, 11),  
(7, 11), 
(9, 11), 
(12, 11), 
(14, 11), 

-- Futebol Masculino
(27, 12), 
(29, 12), 
(31, 12), 
(33, 12), 
(35, 12), 
(37, 12), 

-- Hóquei Feminino
(16, 13), 
(18, 13), 
(20, 13), 
(22, 13), 
(24, 13), 
(26, 13), 

-- Hóquei Masculino
(38, 14), 
(40, 14), 
(42, 14), 
(44, 14), 
(46, 14),
(48, 14), 
(50, 14), 

-- Ginástica Artística Feminina
(28, 15), 
(30, 15),
(32, 15), 
(34, 15), 

-- Ginástica Artística Masculina
(2, 16), 
(4, 16), 
(6, 16), 
(8, 16), 

-- Ténis de Mesa Feminino
(36, 17), 
(39, 17), 
(41, 17), 
(43, 17), 

-- Ténis de Mesa Masculino
(10, 18), 
(11, 18), 
(13, 18), 
(15, 18), 

-- Atletismo 100m Feminino
(45, 7), 
(47, 7), 
(49, 7), 
(1, 7), 

-- Atletismo 200m Feminino
(3, 9), 
(5, 9), 
(7, 9), 
(9, 9);
-- TRUNCATE TABLE Atleta_Modalidade;

INSERT INTO Modalidade_Treinador (IdModalidade, IdTreinador)
VALUES
(1, 1),  -- João Silva (Natação Feminina)
(2, 2),  -- Emily Johnson (Natação Masculina)
(3, 3),  -- David Williams (Volleyball Feminino)
(4, 4),  -- Hiroshi Tanaka (Volleyball Masculino)
(5, 5),  -- Li Wei (Basketball Feminino)
(6, 6),  -- Hans Müller (Basketball Masculino)
(7, 7),  -- Siti Zainab (Atletismo 100m Feminino)
(8, 8),  -- James Brown (Atletismo 100m Masculino)
(9, 9),  -- Maria Fernandes (Atletismo 200m Feminino)
(10, 10), -- Oliver Smith (Atletismo 200m Masculino)
(11, 11), -- Takumi Yamamoto (Futebol Feminino)
(12, 12), -- Chen Mei (Futebol Masculino)
(13, 13), -- Rebecca Taylor (Hóquei Feminino)
(14, 14), -- Pedro Costa (Hóquei Masculino)
(15, 15), -- Ryuji Suzuki (Ginástica Feminina)
(16, 1), -- Li Wei (Ginástica Masculina)
(17, 2), -- Siti Zainab (Ténis de Mesa Feminino)
(18, 3); -- James Brown (Ténis de Mesa Masculino)
-- TRUNCATE TABLE Modalidade_Treinador;