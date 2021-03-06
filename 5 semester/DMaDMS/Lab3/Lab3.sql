CREATE SCHEMA IF NOT EXISTS Labs;

DROP TABLE IF EXISTS employees;
DROP TABLE IF EXISTS order_status;
DROP TABLE IF EXISTS products;
DROP TABLE IF EXISTS stock;
DROP TABLE IF EXISTS organizations;
DROP TABLE IF EXISTS clients;
DROP TABLE IF EXISTS suppliers;
DROP TABLE IF EXISTS orders;
DROP TABLE IF EXISTS arrive_stock;
DROP TABLE IF EXISTS ordered_products;

----------------------------------------------------------------------------
-- Таблица «Сотрудники» хранит данные о работниках торговой фирмы, --
-- которые выполняют оформление и ведение заказов, принимают на склад --
-- товар и несут за него ответственность. --
----------------------------------------------------------------------------
-- employeeID - ID сотрудника --
-- name - имя сотрудника --
-- position - должность сотрудника --
-- phone - тел. номер сотрудника --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`employees` (
	`employeeID` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(30) NOT NULL,
	`position` VARCHAR(30) DEFAULT NULL,
	`phone` VARCHAR(15) DEFAULT NULL,
	PRIMARY KEY (`employeeID`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «СостояниеЗаказа» является справочником и хранит описание --
-- возможных состояний заказа (оформлен, оплачен, отгружен и т.д.). --
----------------------------------------------------------------------------
-- statusID - ID статуса --
-- status - статус заказа: --
-- "обработан", "оплачен", "доставляется", "отменён" --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`order_status` (
	`statusID` INT NOT NULL AUTO_INCREMENT,
	`status` VARCHAR(15) NOT NULL,
	PRIMARY KEY (`statusID`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «КаталогТоваров» является справочником товаров и хранит --
-- описание наименований товаров. --
----------------------------------------------------------------------------
-- productID - ID товара --
-- name - наименование товара --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`products` (
	`productID` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(30) NOT NULL,
	PRIMARY KEY (`productID`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «Склад» хранит данные о складах товаров. --
----------------------------------------------------------------------------
-- stockID - ID склада --
-- productPlace - месторасположение товаров --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`stock` (
	`stockID` INT NOT NULL AUTO_INCREMENT,
	`productPlace` VARCHAR(30) NOT NULL,
	PRIMARY KEY (`stockID`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «Информация о складе» хранит данные о товарах на складе. --
----------------------------------------------------------------------------
-- stockID - ID склада --
-- productID - ID товара --
-- specification - спецификация товаров склада --
-- productPrice - цена товаров --
-- productsLeft - осталось товаров --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`stock_info` (
	`stockID` INT NOT NULL,
	`productID` INT NOT NULL,
	`specification` VARCHAR(30) NULL,
	`productPrice` FLOAT NOT NULL,
	`productsLeft` INT NOT NULL);
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «Организации» используется для хранения общих данных обо --
-- всех организациях, с которыми осуществляется взаимодействие --
-- торговой фирмы. --
----------------------------------------------------------------------------
-- organizationID - ID организации --
-- taxNumber - уникальный номер плательщика (УНП) --
-- name - название организации --
-- boss - руководитель организации --
-- phone - тел. номер организации --
-- adress - адрес организации --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`organizations` (
	`organizationID` INT NOT NULL AUTO_INCREMENT,
	`taxNumber` INT NOT NULL,
	`name` VARCHAR(45) NOT NULL,
	`boss` VARCHAR(45) NOT NULL,
	`phone` VARCHAR(15) DEFAULT NULL,
	`adress` VARCHAR(20) NOT NULL,
	PRIMARY KEY (`organizationID`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «Клиенты» используется для хранения данных о менеджерах, --
-- которые от имени организации покупают товары у торговой фирмы. --
----------------------------------------------------------------------------
-- clientManagerID - ID менеджера клиентов --
-- manager - имя менеджера --
-- phone - тел. номер менеджера --
-- fax - факс менеджера --
-- organizationID - ID организации, которую представляет менеджер --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`clients` (
	`clientManagerID` INT NOT NULL AUTO_INCREMENT,
	`manager` VARCHAR(45) NOT NULL,
	`phone` VARCHAR(15) DEFAULT NULL,
	`fax` VARCHAR(15) DEFAULT NULL,
	`organizationID` INT NOT NULL,
	PRIMARY KEY (`clientManagerID`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «Поставщики» используется для хранения данных о менеджерах, --
-- которые от имени организации поставляли товары на склад торговой фирмы. --
----------------------------------------------------------------------------
-- supplierManagerID - ID менеджера поставщиков --
-- manager - имя менеджера --
-- phone - тел. номер менеджера --
-- fax - факс менеджера --
-- organizationID - ID организации, которую представляет менеджер --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`suppliers` (
	`supplierManagerID` INT NOT NULL AUTO_INCREMENT,
	`manager` VARCHAR(45) NOT NULL,
	`phone` VARCHAR(15) DEFAULT NULL,
	`fax` VARCHAR(15) DEFAULT NULL,
	`organizationID` INT NOT NULL,
	PRIMARY KEY (`supplierManagerID`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «Заказы» хранит данные о заказах сформированных для клиентов --
-- и их состоянии (оформлен, оплачен, отгружен и т.д.). --
----------------------------------------------------------------------------
-- orderID - ID заказа --
-- invoiceNumber - номер накладной --
-- orderDate - дата заказа --
-- shipmentDate - дата отгрузки --
-- totalAmount - общая сумма заказа --
-- comments - примечания к заказу --
-- clientManagerID - ID менеджера клиентов --
-- employeeID - ID сотрудника, обслуживающего заказ --
-- statusID - ID текущего статуса заказа --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`orders` (
	`orderID` INT NOT NULL AUTO_INCREMENT,
	`invoiceNumber` INT NOT NULL,
	`orderDate` DATE NOT NULL,
	`shipmentDate` DATE DEFAULT NULL,
	`totalAmount` FLOAT NOT NULL,
	`comments` VARCHAR(45) DEFAULT NULL,
	`clientManagerID` INT NOT NULL,
	`employeeID` INT NOT NULL,
	`statusID` INT NOT NULL,
	PRIMARY KEY (`orderID`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «ПриходСклад» хранит данные о поступления товаров на склад --
-- торговой фирмы, в соответствии с их кодами на складе. --
----------------------------------------------------------------------------
-- stockID - ID склада --
-- arriveDate - дата поставки --
-- supplierID - ID менеджера поставщиков --
-- invoiceNumber - номер накладной --
-- amount - количество товаров --
-- price - цена поставки --
-- inspector - приёмщик --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`arrive_stock` (
	`stockID` INT NOT NULL,
	`arriveDate` DATE NOT NULL,
	`supplierID` INT NOT NULL,
	`invoiceNumber` INT NOT NULL,
	`amount` INT NOT NULL,
	`price` FLOAT NOT NULL,
	`inspector` VARCHAR(25) NOT NULL,
	PRIMARY KEY (`stockID`, `arriveDate`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
-- Таблица «Заказано товаров» описывает включение товаров, поступивших на --
-- склад в заказы, указывая их количество и цену. --
----------------------------------------------------------------------------
-- orderID - ID заказа --
-- stockID - ID скалада --
-- amount - количество товаров --
-- sellingPrice - цена продажи --
----------------------------------------------------------------------------
CREATE TABLE `labs`.`ordered_products` (
	`orderID` INT NOT NULL,
	`stockID` INT NOT NULL,
	`amount` INT NOT NULL,
	`sellingPrice` FLOAT NOT NULL,
	PRIMARY KEY (`orderID`, `stockID`));
----------------------------------------------------------------------------

----------------------------------------------------------------------------
ALTER TABLE stock_info
ADD CONSTRAINT has_product FOREIGN KEY (productID)
REFERENCES products (productID) ON UPDATE CASCADE;
----------------------------------------------------------------------------

----------------------------------------------------------------------------
ALTER TABLE clients
ADD CONSTRAINT client_from_organization FOREIGN KEY (organizationID)
REFERENCES organizations (organizationID) ON UPDATE CASCADE;
----------------------------------------------------------------------------

----------------------------------------------------------------------------
ALTER TABLE suppliers
ADD CONSTRAINT supplier_from_organization FOREIGN KEY (organizationID)
REFERENCES organizations (organizationID) ON UPDATE CASCADE;
----------------------------------------------------------------------------

----------------------------------------------------------------------------
ALTER TABLE orders
ADD CONSTRAINT client_for_order FOREIGN KEY (clientManagerID)
REFERENCES clients (clientManagerID) ON UPDATE CASCADE,
ADD CONSTRAINT employee_for_order FOREIGN KEY (employeeID)
REFERENCES employees (employeeID) ON UPDATE CASCADE,
ADD CONSTRAINT order_status FOREIGN KEY (statusID)
REFERENCES order_status (statusID) ON UPDATE CASCADE;
----------------------------------------------------------------------------

----------------------------------------------------------------------------
ALTER TABLE arrive_stock
ADD CONSTRAINT arrive_to_stock FOREIGN KEY (stockID)
REFERENCES stock (stockID) ON UPDATE CASCADE,
ADD CONSTRAINT arrive_from_supplier FOREIGN KEY (supplierID)
REFERENCES suppliers (supplierManagerID) ON UPDATE CASCADE;
----------------------------------------------------------------------------

----------------------------------------------------------------------------
ALTER TABLE ordered_products
ADD CONSTRAINT products_from_order FOREIGN KEY (orderID)
REFERENCES orders (orderID) ON UPDATE CASCADE,
ADD CONSTRAINT ordered_from_stock FOREIGN KEY (stockID)
REFERENCES stock (stockID) ON UPDATE CASCADE;
----------------------------------------------------------------------------
