--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table employees --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`employees` (`employeeID`, `name`, `position`, `phone`) VALUES ('1', 'Alfredo E. Blanton', 'Palliative care nurse', '989-513-1893');
INSERT INTO `labs`.`employees` (`employeeID`, `name`, `position`, `phone`) VALUES ('2', 'Derrick N. Phillips', 'Mechanic', '785-454-9977');
INSERT INTO `labs`.`employees` (`employeeID`, `name`, `position`, `phone`) VALUES ('3', 'Sam P. Burton', 'Operating engineer', '708-350-2111');
INSERT INTO `labs`.`employees` (`employeeID`, `name`, `position`, `phone`) VALUES ('4', 'Boyce M. Messer', 'Arborist', '928-543-3783');
INSERT INTO `labs`.`employees` (`employeeID`, `name`, `position`, `phone`) VALUES ('5', 'Florence S. Sharkey', 'Educational counselor', '662-531-2189');
INSERT INTO `labs`.`employees` (`employeeID`, `name`, `position`, `phone`) VALUES ('6', 'Laura T. Gray', 'Biostatistician', '417-994-8801');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table order_status --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`order_status` (`statusID`, `status`) VALUES ('1', 'processed');
INSERT INTO `labs`.`order_status` (`statusID`, `status`) VALUES ('2', 'paid');
INSERT INTO `labs`.`order_status` (`statusID`, `status`) VALUES ('3', 'shipped');
INSERT INTO `labs`.`order_status` (`statusID`, `status`) VALUES ('4', 'cancelled');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table products --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('1', 'Juice');
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('2', 'Soap');
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('3', 'Monitor');
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('4', 'Phone');
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('5', 'Apples');
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('6', 'TV');
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('7', 'Milk');
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('8', 'Porks');
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('9', 'Bag');
INSERT INTO `labs`.`products` (`productID`, `name`) VALUES ('10', 'Car');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table stock --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`stock` (`stockID`, `productPlace`) VALUES ('1', 'Portland');
INSERT INTO `labs`.`stock` (`stockID`, `productPlace`) VALUES ('2', 'Los Angeles');
INSERT INTO `labs`.`stock` (`stockID`, `productPlace`) VALUES ('3', 'Seattle');
INSERT INTO `labs`.`stock` (`stockID`, `productPlace`) VALUES ('4', 'Houston');
INSERT INTO `labs`.`stock` (`stockID`, `productPlace`) VALUES ('5', 'Atlanta');
INSERT INTO `labs`.`stock` (`stockID`, `productPlace`) VALUES ('6', 'Houston');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table stock_info --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('1', '2', '-', '3', '30');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('1', '3', '-', '50', '20');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('1', '4', '-', '250', '30');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('2', '1', '-', '2', '50');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('2', '3', '-', '25', '100');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('3', '1', '-', '2', '40');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('3', '4', '-', '250', '10');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('3', '7', '-', '1', '23');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('3', '5', '-', '2', '53');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('4', '6', '-', '150', '34');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('5', '9', '-', '7', '75');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('5', '10', '-', '10000', '86');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('6', '7', '-', '1', '34');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('6', '8', '-', '5', '32');
INSERT INTO `labs`.`stock_info` (`stockID`, `productID`, `specification`, `productPrice`, `productsLeft`) VALUES ('6', '6', '-', '150', '56');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table organizations --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`organizations` (`organizationID`, `taxNumber`, `name`, `boss`, `phone`, `adress`)
VALUES ('1', '64738', 'JSC Crystal', 'John Davis', '214-267-5403', 'Dallas');
INSERT INTO `labs`.`organizations` (`organizationID`, `taxNumber`, `name`, `boss`, `phone`, `adress`)
VALUES ('2', '76483', 'JSC Garden Care', 'Sabrina B. Elliott', '909-864-4776', 'Highland');
INSERT INTO `labs`.`organizations` (`organizationID`, `taxNumber`, `name`, `boss`, `phone`, `adress`)
VALUES ('3', '27349', 'Ltd. Aaronson Furniture', 'Frankie G. Caffey', '401-216-1713', 'Providence');
INSERT INTO `labs`.`organizations` (`organizationID`, `taxNumber`, `name`, `boss`, `phone`, `adress`)
VALUES ('4', '85940', 'Ltd. Handy Dan', 'Jill C. Sheedy', '617-826-8034', 'Boston');
INSERT INTO `labs`.`organizations` (`organizationID`, `taxNumber`, `name`, `boss`, `phone`, `adress`)
VALUES ('5', '85733', 'JSC', 'Maurice E. Hill', '701-325-2778', 'Linton');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table clients --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`clients` (`clientManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('1', 'Ed D. Carter', '612-312-8542', '-', '1');
INSERT INTO `labs`.`clients` (`clientManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('2', 'Bobby M. Dick', '404-341-9391', '-', '2');
INSERT INTO `labs`.`clients` (`clientManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('3', 'Jack T. Parker', '504-823-3385', '-', '3');
INSERT INTO `labs`.`clients` (`clientManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('4', 'James R. Rosa', '786-200-2788', '-', '4');
INSERT INTO `labs`.`clients` (`clientManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('5', 'Phil McBride', '703-818-5885', '-', '5');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table suppliers --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`suppliers` (`supplierManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('1', 'Leslie Park', '806-667-7252', '-', '1');
INSERT INTO `labs`.`suppliers` (`supplierManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('2', 'Yui Edwards', '254-761-0122', '-', '2');
INSERT INTO `labs`.`suppliers` (`supplierManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('3', 'Nick Israel', '954-596-8938', '-', '3');
INSERT INTO `labs`.`suppliers` (`supplierManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('4', 'Jo Scott', '312-969-8937', '-', '4');
INSERT INTO `labs`.`suppliers` (`supplierManagerID`, `manager`, `phone`, `fax`, `organizationID`) VALUES ('5', 'Stue Lokken', '843-729-5581', '-', '5');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table orders --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('1', '47576', '2010-10-19', '100', '-', '1', '1', '2');
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('2', '37467', '2010-10-19', '200', '-', '3', '2', '2');
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `shipmentDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('3', '64934', '2010-10-20', '2010-10-28', '300', '-', '2', '4', '3');
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('4', '83984', '2010-10-21', '200', '-', '4', '3', '1');
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('5', '95833', '2010-11-03', '1500', '-', '5', '5', '4');
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('6', '76374', '2010-11-11', '5000', '-', '3', '4', '4');
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('7', '22562', '2010-11-14', '3500', '-', '2', '5', '2');
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `shipmentDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('8', '57843', '2010-11-25', '2010-12-04', '10000', '-', '1', '6', '3');
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `shipmentDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('9', '26574', '2010-12-02', '2010-12-06', '2500', '-', '2', '3', '3');
INSERT INTO `labs`.`orders` (`orderID`, `invoiceNumber`, `orderDate`, `totalAmount`, `comments`, `clientManagerID`, `employeeID`, `statusID`)
VALUES ('10', '96843', '2010-12-05', '50', '-', '4', '2', '1');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table arrive_stock --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`arrive_stock` (`stockID`, `arriveDate`, `supplierID`, `invoiceNumber`, `amount`, `price`, `inspector`)
VALUES ('1', '2010-09-10', '1', '64376', '3', '500', 'Juan E. Iliff');
INSERT INTO `labs`.`arrive_stock` (`stockID`, `arriveDate`, `supplierID`, `invoiceNumber`, `amount`, `price`, `inspector`)
VALUES ('2', '2010-09-10', '2', '36347', '123', '748', 'Alan T. Carr');
INSERT INTO `labs`.`arrive_stock` (`stockID`, `arriveDate`, `supplierID`, `invoiceNumber`, `amount`, `price`, `inspector`)
VALUES ('3', '2010-09-15', '3', '34673', '32', '100', 'Diego D. Sales');
INSERT INTO `labs`.`arrive_stock` (`stockID`, `arriveDate`, `supplierID`, `invoiceNumber`, `amount`, `price`, `inspector`)
VALUES ('4', '2010-09-15', '4', '37838', '64', '5005', 'Leroy T. Sayre');
INSERT INTO `labs`.`arrive_stock` (`stockID`, `arriveDate`, `supplierID`, `invoiceNumber`, `amount`, `price`, `inspector`)
VALUES ('5', '2010-10-01', '5', '67545', '575', '9000', 'Gary F. Rule');
INSERT INTO `labs`.`arrive_stock` (`stockID`, `arriveDate`, `supplierID`, `invoiceNumber`, `amount`, `price`, `inspector`)
VALUES ('6', '2010-10-10', '1', '87289', '545', '4392', 'Earl M. Morrissey');
--------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Table ordered_products --
--------------------------------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('1', '1', '5', '100');
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('2', '2', '6', '200');
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('3', '3', '7', '300');
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('4', '4', '5', '400');
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('5', '5', '6', '500');
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('6', '6', '7', '600');
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('7', '1', '5', '700');
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('8', '2', '6', '800');
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('9', '3', '7', '900');
INSERT INTO `labs`.`ordered_products` (`orderID`, `stockID`, `amount`, `sellingPrice`) VALUES ('10', '4', '8', '1000');
--------------------------------------------------------------------------------------------------------------------------------------------------------------
