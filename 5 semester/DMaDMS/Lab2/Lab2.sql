INSERT INTO `Labs`.`country` (`countryID`, `name`) VALUES ('1', 'Belarus');
INSERT INTO `Labs`.`country` (`countryID`, `name`) VALUES ('2', 'Germany');
INSERT INTO `Labs`.`country` (`countryID`, `name`) VALUES ('3', 'England');
INSERT INTO `Labs`.`country` (`countryID`, `name`) VALUES ('4', 'USA');
INSERT INTO `Labs`.`country` (`countryID`, `name`) VALUES ('5', 'Brazil');

INSERT INTO `Labs`.`organisation` (`organisationID`, `name`, `countryID`) VALUES ('1', 'Belavia', '1');
INSERT INTO `Labs`.`organisation` (`organisationID`, `name`, `countryID`) VALUES ('2', 'Air Staff', '4');
INSERT INTO `Labs`.`organisation` (`organisationID`, `name`, `countryID`) VALUES ('3', 'Germania', '2');
INSERT INTO `Labs`.`organisation` (`organisationID`, `name`, `countryID`) VALUES ('4', 'Azul', '5');
INSERT INTO `Labs`.`organisation` (`organisationID`, `name`, `countryID`) VALUES ('5', 'British Airways', '3');

INSERT INTO `Labs`.`aircraft` (`planeID`, `name`, `organisationID`) VALUES ('1', 'Aerobus', '2');
INSERT INTO `Labs`.`aircraft` (`planeID`, `name`, `organisationID`) VALUES ('2', 'Aerobus 77', '1');	
INSERT INTO `Labs`.`aircraft` (`planeID`, `name`, `organisationID`) VALUES ('3', 'Boeing', '4');
INSERT INTO `Labs`.`aircraft` (`planeID`, `name`, `organisationID`) VALUES ('4', 'Aerobus 747', '5');
INSERT INTO `Labs`.`aircraft` (`planeID`, `name`, `organisationID`) VALUES ('5', 'Boeing 747', '3');
INSERT INTO `Labs`.`aircraft` (`planeID`, `name`, `organisationID`) VALUES ('6', 'Oceanic 815', '4');

INSERT INTO `Labs`.`crew` (`crewID`, `name`, `organisationID`) VALUES ('1', 'Pilots', '2');
INSERT INTO `Labs`.`crew` (`crewID`, `name`, `organisationID`) VALUES ('2', 'Airlovers', '4');
INSERT INTO `Labs`.`crew` (`crewID`, `name`, `organisationID`) VALUES ('3', 'Planners', '3');
INSERT INTO `Labs`.`crew` (`crewID`, `name`, `organisationID`) VALUES ('4', 'FlyingBulba', '1');
INSERT INTO `Labs`.`crew` (`crewID`, `name`, `organisationID`) VALUES ('5', 'Kekus', '5');

INSERT INTO `Labs`.`route` (`routeID`, `from`, `to`) VALUES ('1', 'New York', 'London');
INSERT INTO `Labs`.`route` (`routeID`, `from`, `to`) VALUES ('2', 'Minsk', 'Brazilia');
INSERT INTO `Labs`.`route` (`routeID`, `from`, `to`) VALUES ('3', 'Berlin', 'Boston');
INSERT INTO `Labs`.`route` (`routeID`, `from`, `to`) VALUES ('4', 'Huston', 'Leeds');
INSERT INTO `Labs`.`route` (`routeID`, `from`, `to`) VALUES ('5', 'Munchen', 'Los Angeles');
INSERT INTO `Labs`.`route` (`routeID`, `from`, `to`) VALUES ('6', 'Manchester', 'Minsk');
INSERT INTO `Labs`.`route` (`routeID`, `from`, `to`) VALUES ('7', 'Hamburg', 'Minsk');
INSERT INTO `Labs`.`route` (`routeID`, `from`, `to`) VALUES ('8', 'Minsk', 'London');
INSERT INTO `Labs`.`route` (`routeID`, `from`, `to`) VALUES ('9', 'Brazilia', 'Washington');

INSERT INTO `Labs`.`flight` (`flightID`, `routeID`, `crewID`, `planeID`) VALUES ('1', '3', '3', '5');
INSERT INTO `Labs`.`flight` (`flightID`, `routeID`, `crewID`, `planeID`) VALUES ('2', '7', '3', '5');
INSERT INTO `Labs`.`flight` (`flightID`, `routeID`, `crewID`, `planeID`) VALUES ('3', '8', '2', '4');
INSERT INTO `Labs`.`flight` (`flightID`, `routeID`, `crewID`, `planeID`) VALUES ('4', '1', '1', '1');
INSERT INTO `Labs`.`flight` (`flightID`, `routeID`, `crewID`, `planeID`) VALUES ('5', '2', '3', '2');
INSERT INTO `Labs`.`flight` (`flightID`, `routeID`, `crewID`, `planeID`) VALUES ('6', '9', '3', '2');
INSERT INTO `Labs`.`flight` (`flightID`, `routeID`, `crewID`, `planeID`) VALUES ('7', '4', '5', '3');
INSERT INTO `Labs`.`flight` (`flightID`, `routeID`, `crewID`, `planeID`) VALUES ('8', '6', '4', '5');
INSERT INTO `Labs`.`flight` (`flightID`, `routeID`, `crewID`, `planeID`) VALUES ('9', '5', '5', '3');
