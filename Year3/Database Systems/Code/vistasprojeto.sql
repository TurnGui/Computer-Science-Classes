USE sportorg;

-- 1. View para organizar os atletas pelo numero de eventos que participam:
CREATE VIEW AtletasPorNumeroEventos AS
SELECT 
    a.NomeAtleta,
    COUNT(ae.IdEvento) AS NumeroEventos
FROM 
    atleta_evento AS ae
JOIN 
    atleta AS a ON ae.IdAtleta = a.IdAtleta
GROUP BY 
    a.NomeAtleta
ORDER BY 
    NumeroEventos DESC;
    
SELECT * FROM AtletasPorNumeroEventos;
-- DROP VIEW AtletasPorNumeroEventos;

-- 2. View para visualizar o numero de participantes e de staffs em cada evento:
CREATE VIEW DetalhesEventos AS
SELECT 
    E.IdEvento,
    E.NomeEvento,
    E.Locall,
    E.Data_Hora,
    COUNT(DISTINCT AE.IdAtleta) AS TotalParticipantes,
    COUNT(DISTINCT ES.IdStaff) AS TotalStaff
FROM 
    Evento E
LEFT JOIN 
    Atleta_Evento AE
ON 
    E.IdEvento = AE.IdEvento
LEFT JOIN 
    Evento_Staff ES
ON 
    E.IdEvento = ES.IdEvento
GROUP BY 
    E.IdEvento, E.NomeEvento, E.Locall, E.Data_Hora;
    
SELECT * FROM DetalhesEventos;
-- DROP VIEW DetalhesEventos;

-- 3. View que lista os funcionários associados a cada evento, incluindo as suas funções e horários:
CREATE VIEW StaffEventos AS
SELECT 
    E.IdEvento,
    E.NomeEvento,
    S.IdStaff,
    S.NomeStaff,
    S.Tipo,
    S.Horario
FROM 
    Evento E
JOIN 
    Evento_Staff ES
ON 
    E.IdEvento = ES.IdEvento
JOIN 
    Staff S
ON 
    ES.IdStaff = S.IdStaff;
    
SELECT * FROM StaffEventos;
-- DROP VIEW StaffEventos;

-- 4. View que mostra quais universidades têm atletas a participar no maior número de modalidades:
CREATE VIEW UniversidadesMaisModalidades AS
SELECT 
    U.NomeUniversidade,
    COUNT(DISTINCT am.IdModalidade) AS TotalModalidades
FROM 
    universidade U
JOIN 
    Atleta A ON U.IdUniversidade = A.IdUniversidade
JOIN 
    Atleta_Modalidade AM ON A.IdAtleta = AM.IdAtleta
GROUP BY 
    U.NomeUniversidade
ORDER BY 
    TotalModalidades DESC;

SELECT * FROM UniversidadesMaisModalidades;
-- DROP VIEW UniversidadesMaisModalidades;