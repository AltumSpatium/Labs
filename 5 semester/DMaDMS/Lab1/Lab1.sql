CREATE SCHEMA Labs;

DROP TABLE IF EXISTS country;
DROP TABLE IF EXISTS organisation;
DROP TABLE IF EXISTS aircraft;
DROP TABLE IF EXISTS crew;
DROP TABLE IF EXISTS route;
DROP TABLE IF EXISTS flight;

CREATE TABLE `Labs`.`country` (
	`countryID` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(45) NOT NULL,
	PRIMARY KEY (`countryID`));

CREATE TABLE `Labs`.`organisation` (
	`organisationID` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(45) NOT NULL,
	PRIMARY KEY (`organisationID`));

CREATE TABLE `Labs`.`aircraft` (
	`planeID` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(45) NOT NULL,
	PRIMARY KEY (`planeID`));

CREATE TABLE `Labs`.`crew` (
	`crewID` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(45) NOT NULL,
	PRIMARY KEY (`crewID`));

CREATE TABLE `Labs`.`route` (
	`routeID` INT NOT NULL AUTO_INCREMENT,
	`from` VARCHAR(45) NOT NULL,
	`to` VARCHAR(45) NOT NULL,
	PRIMARY KEY (`routeID`));

CREATE TABLE `Labs`.`flight` (
	`flightID` INT NOT NULL AUTO_INCREMENT,
	PRIMARY KEY (`flightID`));

ALTER TABLE organisation
ADD countryID INT DEFAULT NULL,
ADD FOREIGN KEY (countryID) REFERENCES country (countryID) ON UPDATE CASCADE;

ALTER TABLE aircraft
ADD organisationID INT DEFAULT NULL,
ADD FOREIGN KEY (organisationID) REFERENCES organisation (organisationID) ON UPDATE CASCADE;

ALTER TABLE crew
ADD organisationID INT DEFAULT NULL,
ADD FOREIGN KEY (organisationID) REFERENCES organisation (organisationID) ON UPDATE CASCADE;

ALTER TABLE flight
ADD routeID INT DEFAULT NULL,
ADD FOREIGN KEY (routeID) REFERENCES route (routeID) ON UPDATE CASCADE,
ADD crewID INT DEFAULT NULL,
ADD FOREIGN KEY (crewID) REFERENCES crew (crewID) ON UPDATE CASCADE,
ADD planeID INT DEFAULT NULL,
ADD FOREIGN KEY (planeID) REFERENCES aircraft (planeID) ON UPDATE CASCADE;
