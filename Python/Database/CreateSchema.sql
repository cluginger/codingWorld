-- Cleanup existing tables and data
DROP TABLE IF EXISTS Countries, Institutions, Persons,
  Theses, Conferences, Journals, Papers, AuthPapers;

-- Create all tables, incl constraints
CREATE TABLE Countries(
  CoKey SERIAL PRIMARY KEY,
  Name VARCHAR(256) NOT NULL
);

CREATE TABLE Institutions(
  IKey SERIAL PRIMARY KEY,
  Name VARCHAR(256) NOT NULL,
  CoKey INT REFERENCES Countries
);

CREATE TABLE Persons(
  AKey INT PRIMARY KEY,
  Name VARCHAR(128) UNIQUE NOT NULL,
  Website VARCHAR(256),
  IKey INT REFERENCES Institutions
);

CREATE TABLE Theses(
  TKey INT PRIMARY KEY,
  Title VARCHAR(512) NOT NULL,
  Year INT NOT NULL,
  NPages INT,
  Type CHAR(3),
  ISBN VARCHAR(17), --13+4
  AKey INT REFERENCES Persons NOT NULL,
  IKey INT REFERENCES Institutions
);

CREATE TABLE Conferences(
  CKey INT PRIMARY KEY,
  SName VARCHAR(32) NOT NULL,
  Title VARCHAR(256) NOT NULL,
  City VARCHAR(128) NOT NULL,
  CoKey INT REFERENCES Countries NOT NULL,
  Year INT NOT NULL,
  ISBN VARCHAR(17), --13+4
  UNIQUE(SName,Year)
);

CREATE TABLE Journals(
  JKey INT PRIMARY KEY,
  SName VARCHAR(32) NOT NULL,
  Title VARCHAR(256) NOT NULL,
  Volume INT NOT NULL,
  Issue INT,
  Year INT NOT NULL,
  UNIQUE(SName,Volume,Issue)
);

CREATE TABLE Papers(
  PKey INT PRIMARY KEY,
  Title VARCHAR(512) NOT NULL,
  Pages VARCHAR(64),
  CKey INT REFERENCES Conferences,
  JKey INT REFERENCES Journals,
  CHECK((CKey IS NOT NULL AND JKey IS NULL)
    OR (CKey IS NULL AND JKey IS NOT NULL))
);

CREATE TABLE AuthPapers(
  PKey INT REFERENCES Papers,
  AKey INT REFERENCES Persons,
  Rank INT NOT NULL,
  PRIMARY KEY(PKey, AKey),
  CHECK(Rank >= 1)
);
