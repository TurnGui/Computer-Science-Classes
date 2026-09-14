USE sportorg;
-- 1. Procedure de adicionar um Atleta novo
-- DROP PROCEDURE AdicionarAtleta;
DELIMITER $$

CREATE PROCEDURE AdicionarAtleta(
    IN p_NomeAtleta VARCHAR(75),
    IN p_Nacionalidade VARCHAR(50),
    IN p_DataNascimento DATE,
    IN p_Email VARCHAR(100),
    IN p_Contacto VARCHAR(20),
    IN p_IdUniversidade INT,
    IN p_IdMorada INT,
    IN p_Genero VARCHAR(20)
)
BEGIN
    INSERT INTO Atleta (NomeAtleta, Nacionalidade, Data_Nascimento, Email, Contacto, IdUniversidade, IdMorada, Genero)
    VALUES (p_NomeAtleta, p_Nacionalidade, p_DataNascimento, p_Email, p_Contacto, p_IdUniversidade, p_IdMorada, p_Genero);
END$$

DELIMITER ;

CALL AdicionarAtleta(
    'João Silva',
    'Portugues',
    '1999-06-12',
    'joaosilva@email.com',
    '987654321',
    1,
    1,
    'M'
);

SELECT * FROM Atleta;

-- 2. Remove um atleta da base de dados, incluindo os registos associados:
-- DROP PROCEDURE RemoverAtleta;
DELIMITER $$

CREATE PROCEDURE RemoverAtleta(
    IN p_IdAtleta INT
)
BEGIN
    -- Remove as associações com eventos
    DELETE FROM Atleta_Evento WHERE IdAtleta = p_IdAtleta;

    -- Remove associações com modalidades
    DELETE FROM Atleta_Modalidade WHERE IdAtleta = p_IdAtleta;

    -- Remove o próprio atleta
    DELETE FROM Atleta WHERE IdAtleta = p_IdAtleta;
END$$

DELIMITER ;

CALL RemoverAtleta (
	51
);

SELECT * FROM Atleta;


-- 3. Estabelece a relação entre um treinador e uma modalidade:
-- DROP PROCEDURE AssociarTreinadorAModalidade;
DELIMITER $$

CREATE PROCEDURE AssociarTreinadorAModalidade(
    IN p_IdTreinador INT,
    IN p_IdModalidade INT
)
BEGIN
    INSERT INTO Modalidade_Treinador (IdTreinador, IdModalidade)
    VALUES (p_IdTreinador, p_IdModalidade);
END$$

DELIMITER ;
CALL AssociarTreinadorAModalidade(
10,
3
);

SELECT * FROM Modalidade_Treinador;


-- Perfis de utilização
-- NOTA: As passwords abaixo são apenas placeholders para fins de demonstração
-- (ambiente local de desenvolvimento). Nunca usar passwords em texto simples
-- num ambiente de produção — usar variáveis de ambiente/gestão de segredos.

CREATE USER 'admin'@'localhost' IDENTIFIED BY 'CHANGE_ME_admin';
CREATE USER 'gestor'@'localhost' IDENTIFIED BY 'CHANGE_ME_gestor';
CREATE USER 'treinador'@'localhost' IDENTIFIED BY 'CHANGE_ME_treinador';
CREATE USER 'staff'@'localhost' IDENTIFIED BY 'CHANGE_ME_staff';
CREATE USER 'aluno'@'localhost' IDENTIFIED BY 'CHANGE_ME_aluno';

GRANT ALL PRIVILEGES ON *.* TO 'admin'@'localhost';

-- Gestor pode ver e alterar
GRANT SELECT, UPDATE ON sportorg.Atleta TO 'gestor'@'localhost';
GRANT SELECT, UPDATE ON sportorg.Evento TO 'gestor'@'localhost';
GRANT SELECT, UPDATE ON sportorg.Modalidade TO 'gestor'@'localhost';
GRANT SELECT, UPDATE ON sportorg.Universidade TO 'gestor'@'localhost';

-- Staff pode apenas ver
GRANT SELECT ON sportorg.Evento TO 'staff'@'localhost';
GRANT SELECT ON sportorg.Staff TO 'staff'@'localhost';

-- Treinador pode apenas ver
GRANT SELECT ON sportorg.Atleta TO 'treinador'@'localhost'; 
GRANT SELECT ON sportorg.Modalidade TO 'treinador'@'localhost';


-- Se um aluno deve acessar apenas seus próprios dados, você pode usar views
CREATE VIEW aluno_view AS 
SELECT * 
FROM Atleta 
WHERE IdAtleta = (SUBSTRING_INDEX(USER(), '@', 1)); 
-- SUBSTRING_INDEX Obtém o nome do usuário logado (por exemplo, aluno1) e filtra os dados correspondentes ao ID do aluno.

GRANT SELECT ON aluno_view TO 'aluno'@'localhost';

-- Exemplo de como revogar permissões (não é executado em sequência com os GRANTs
-- acima, é apenas uma demonstração da sintaxe de REVOKE):
-- REVOKE SELECT, UPDATE ON sportorg.Atleta FROM 'gestor'@'localhost';

-- Após criar ou alterar permissões, é necessário atualizar os privilégios no servidor
FLUSH PRIVILEGES; 

-- Para o backup manual, abrir cmd como admin
-- ir até a pasta aonde está o mysql bin
-- codigo: cd C:\Program Files\MySQL\MySQL Server 9.0
-- cd bin 
-- mysqldump -u admin -p sportorg > backup_sportorg.sql
-- (introduzir a password do utilizador admin quando solicitado)
-- e cria o ficheiro backup.

-- -p: Solicitará a senha do usuário.
-- sportorg: Nome da base de dados.
-- backup_sportorg.sql: Nome do arquivo de saída