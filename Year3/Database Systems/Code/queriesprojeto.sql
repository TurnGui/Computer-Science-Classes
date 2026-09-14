USE sportorg;

-- 1. Mostrar o número de atletas Masculinos:
SELECT COUNT(*) AS NumeroDeAtletasMasculinos
FROM Atleta
WHERE Genero = 'M'; 

-- 2. Mostrar o número de atletas Femininos:
SELECT COUNT(*) AS NumeroDeAtletasFemininos
FROM Atleta
WHERE Genero = 'F';

-- 3. Listar uma data especifica de um certo evento (exemplo: Volleyball Feminina) :
SELECT Data_Hora 
FROM Evento
WHERE NomeEvento = 'Volleyball Feminina';

-- 4. dado o nome de uma universidade saber quais são os seus atletas (exemplo: Universidade de Cambridge):
SELECT a.IdAtleta, a.NomeAtleta, a.Genero, a.Nacionalidade, a.Email
FROM Universidade u
JOIN Atleta a ON u.IdUniversidade = a.IdUniversidade
WHERE u.NomeUniversidade = 'Universidade de Cambridge';

-- 5. dado o nome de uma modalidade saber quais são os treinadores dessa mesma (exemplo: Atletismo 200m):
SELECT t.IdTreinador, t.NomeTreinador, t.Contacto, t.Email
FROM Modalidade m
JOIN Modalidade_Treinador mt ON m.IdModalidade = mt.IdModalidade
JOIN Treinador t ON mt.IdTreinador = t.IdTreinador
WHERE m.NomeModalidade = 'Atletismo 200m';


-- 6. permitir saber a nacionalidade com mais atletas :
SELECT Nacionalidade, COUNT(*) AS TotalAtletas
FROM Atleta
GROUP BY Nacionalidade
ORDER BY TotalAtletas DESC
LIMIT 1;

-- 7. permitir saber a nacionalidade com menos atletas :
SELECT Nacionalidade, COUNT(*) AS TotalAtletas
FROM Atleta
GROUP BY Nacionalidade
ORDER BY TotalAtletas ASC
LIMIT 1;



-- Algebra Relacional --

-- Dado uma modalidade saber quais são os treinadores
SELECT Treinador.NomeTreinador
FROM Modalidade
JOIN Modalidade_Treinador ON Modalidade.IdModalidade = Modalidade_Treinador.IdModalidade
JOIN Treinador ON Modalidade_Treinador.IdTreinador = Treinador.IdTreinador
WHERE Modalidade.NomeModalidade = 'Futebol';

-- Dado uma universidade saber quais são os atletas.
SELECT Atleta.*
FROM Atleta
JOIN Universidade ON Atleta.IdUniversidade = Universidade.IdUniversidade
WHERE Universidade.NomeUniversidade = 'Universidade do Minho';


-- Indexes
-- Melhora buscas por nome e nacionalidade juntos :
CREATE INDEX idx_atleta_nome ON Atleta(NomeAtleta);
-- DROP INDEX idx_atleta_nome ON Atleta(NomeAtleta);

-- Otimiza buscas por eventos em um local e data específica :
CREATE INDEX idx_evento_nome_data ON Evento(Local, Data_Hora);
-- DROP INDEX idx_evento_nome_data ON Evento(Local, Data_Hora);
