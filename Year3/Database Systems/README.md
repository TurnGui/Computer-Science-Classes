# SportOrg — Relational Database for a Multi-Modality Sports Competition

[![Database CI](https://github.com/TurnGui/sportorg-database/actions/workflows/ci.yml/badge.svg)](https://github.com/TurnGui/sportorg-database/actions/workflows/ci.yml)
[![MySQL](https://img.shields.io/badge/MySQL-8.0-4479A1?logo=mysql&logoColor=white)](https://www.mysql.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A full relational database system for managing athletes, universities, coaches, staff, sports
("modalities") and events for an Olympics-style multi-sport university competition, built with
MySQL.

Built as the practical project for the **Database Systems** course, BSc in Computer Science,
University of Minho (2024/2025). Group project — this repository contains the database design
and implementation work.

## Overview

The system models the full lifecycle of a multi-sport competition: universities enrol athletes,
athletes practice one or more sports and take part in events, coaches are assigned to sports,
and staff are assigned to events. The project covers the complete database development
lifecycle — requirements analysis, conceptual (ER) modeling, logical modeling and
normalization, physical implementation in MySQL, and operational tooling (queries, views,
stored procedures, indexing, and access control).

## Entity-Relationship Diagram

![ER Diagram](Conceptual%20Model/er-diagram.png)

Core entities: `Atleta` (Athlete), `Universidade` (University), `Modalidade` (Sport), `Evento`
(Event), `Treinador` (Coach), `Staff`, and `Morada` (Address), connected through
many-to-many relationships (athlete↔event, athlete↔sport, sport↔coach, event↔staff).

The full conceptual/logical modeling, requirements analysis, and normalization proof (the
schema is normalized to 3NF) are documented in [`Report/`](Report).

## Repository structure

```
.
├── Code/
│   ├── projetosql.sql          # Schema: tables, keys, constraints
│   ├── povoamentoprojeto.sql   # Sample data (universities, athletes, events, ...)
│   ├── queriesprojeto.sql      # Analytical queries + indexes
│   ├── vistasprojeto.sql       # Views
│   └── proceduresprojeto.sql   # Stored procedures + user roles/privileges
├── Conceptual Model/
│   ├── er-diagram.png          # Entity-Relationship diagram
│   └── projetobd.brM3          # Editable ER model source file
├── Report/                     # Full written report + presentation slides
├── Project-Assignment.pdf      # Original course assignment brief
└── LICENSE
```

## Features

- **Schema**: 10 tables (7 entities + 3 many-to-many relationship tables), with primary/foreign
  key constraints throughout.
- **Sample data**: realistic seed data — dozens of universities worldwide, athletes, events,
  sports, coaches and staff.
- **Queries**: aggregate and join queries answering management questions (athlete counts by
  gender, athletes by university, coaches by sport, most/least represented nationality, etc.).
- **Views**: `AtletasPorNumeroEventos`, `DetalhesEventos`, `StaffEventos`,
  `UniversidadesMaisModalidades` — precomputed reporting views over the schema.
- **Stored procedures**: add/remove an athlete (with cascading cleanup of related records),
  associate a coach with a sport.
- **Indexing**: indexes on frequently filtered columns (athlete name; event location + date).
- **Access control**: role-based MySQL users (`admin`, `gestor`, `treinador`, `staff`, `aluno`)
  with least-privilege `GRANT`s per role, plus a row-level view example restricting each
  student to their own record.

## Getting started

Requires MySQL 8+ (or MariaDB). Verified to run cleanly end-to-end (schema, seed data, views,
procedures and queries, with zero errors) on MySQL 8.0 on Linux. Run the scripts in order:

```bash
mysql -u root -p < "Code/projetosql.sql"        # 1. create schema + tables
mysql -u root -p < "Code/povoamentoprojeto.sql"  # 2. load sample data
mysql -u root -p < "Code/vistasprojeto.sql"      # 3. create views
mysql -u root -p < "Code/proceduresprojeto.sql"  # 4. create procedures + demo users
mysql -u root -p < "Code/queriesprojeto.sql"     # 5. run example queries + indexes
```

> The `CREATE USER` statements in `proceduresprojeto.sql` use placeholder passwords for local
> demonstration purposes only — replace them before using this schema anywhere beyond a local
> sandbox.

A GitHub Actions workflow ([`ci.yml`](.github/workflows/ci.yml)) runs this exact sequence
against MySQL 8 on every push, so the badge above reflects whether the scripts actually execute
cleanly end-to-end.

## Example output

```
mysql> SELECT COUNT(*) AS NumeroDeAtletasMasculinos FROM Atleta WHERE Genero = 'M';
+---------------------------+
| NumeroDeAtletasMasculinos |
+---------------------------+
|                        26 |
+---------------------------+

mysql> SELECT Nacionalidade, COUNT(*) AS TotalAtletas FROM Atleta
    -> GROUP BY Nacionalidade ORDER BY TotalAtletas DESC LIMIT 1;
+---------------+--------------+
| Nacionalidade | TotalAtletas |
+---------------+--------------+
| Portugues     |           15 |
+---------------+--------------+

mysql> SELECT * FROM UniversidadesMaisModalidades LIMIT 5;
+---------------------------+------------------+
| NomeUniversidade          | TotalModalidades |
+---------------------------+------------------+
| Universidade do Minho     |               18 |
| Universidade de Tokyo     |               14 |
| Universidade de Cambridge |               13 |
| Universidade de Harvard   |               13 |
| Universidade de Munich    |               10 |
+---------------------------+------------------+

mysql> SELECT * FROM DetalhesEventos LIMIT 5;
+----------+----------------------+-----------+---------------------+---------------------+------------+
| IdEvento | NomeEvento           | Local     | Data_Hora           | TotalParticipantes  | TotalStaff |
+----------+----------------------+-----------+---------------------+---------------------+------------+
|        1 | Natação Feminina     | Ginásio 5 | 2024-06-12 15:30:00 |                   9 |          7 |
|        2 | Natação Masculina    | Ginásio 5 | 2024-06-13 15:00:00 |                   9 |          7 |
|        3 | Volleyball Feminina  | Ginásio 7 | 2024-06-14 09:00:00 |                   9 |          7 |
|        4 | Volleyball Masculino | Ginásio 7 | 2024-06-15 10:30:00 |                   9 |          7 |
|        5 | BasketBall Feminino  | Ginásio 1 | 2024-06-16 15:30:00 |                   6 |          7 |
+----------+----------------------+-----------+---------------------+---------------------+------------+
```

## Design notes

- The schema was validated against 1NF/2NF/3NF and is fully normalized (see `Report/`, section
  4.2).
- Storage growth was estimated based on projected yearly data volume (see `Report/`, section
  5.2).
- A backup/recovery plan (`mysqldump`-based) is documented in `Code/proceduresprojeto.sql` and
  `Report/`, section 5.9.

## Tech stack

MySQL · SQL (DDL/DML, stored procedures, views, indexing, access control) · GitHub Actions (CI)

## License

Released under the [MIT License](LICENSE).
