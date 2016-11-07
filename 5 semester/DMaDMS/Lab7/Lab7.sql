-----------------------------------------------------------------------------
-- Вывести реквизиты организаций (название, УНП и юр.адрес), --
-- которые являются поставщиками. --
-----------------------------------------------------------------------------
SELECT o.name, o.taxNumber, o.adress
FROM organizations o, suppliers s
WHERE s.organizationID IN (
    SELECT o.organizationID
    FROM organizations);
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- Вывести коды и наименования товаров, которые еще не поставлялись. --
-----------------------------------------------------------------------------
SELECT name
FROM products
WHERE productID NOT IN (
    SELECT productID
    FROM stock_info);
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- Вывести наименование товаров на складах, остатки которых 100 и меньше. --
-----------------------------------------------------------------------------
SELECT name
FROM products
WHERE productID IN (
    SELECT productID
    FROM stock_info
    GROUP BY productID
    HAVING SUM(productsLeft) <= 100);
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- Вывести заказы (код, номер накладной и дату), в которые включены --
-- более 2-х товаров. --
-----------------------------------------------------------------------------
SELECT o.orderID, o.invoiceNumber, o.orderDate
FROM orders o, ordered_products op
WHERE o.orderID = op.orderID
AND o.orderID IN (
    SELECT op.orderID
    FROM stock_info si
    WHERE op.stockID = si.stockID
    GROUP BY si.stockID
    HAVING COUNT(DISTINCT si.productID) > 2);
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- Определить наименование организаций, сделавших более 1 заказа. --
-----------------------------------------------------------------------------
SELECT org.name
FROM organizations org
WHERE org.organizationID IN (
    SELECT c.organizationID
    FROM clients c, orders o
    WHERE c.clientManagerID = o.clientManagerID
    GROUP BY o.clientManagerID
    HAVING COUNT(o.orderID) > 1);
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- Определить организации, менеджеры которых сделали заказы на товар --
-- с кодом склада 2.--
-----------------------------------------------------------------------------
SELECT org.name
FROM organizations org
WHERE org.organizationID IN (
    SELECT c.organizationID
    FROM clients c, orders o, ordered_products op
    WHERE c.clientManagerID = o.clientManagerID
    AND op.orderID = o.orderID
    AND op.stockID = 2);
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- Получить спецификацию товара, имеющегося  в наличии на --
-- складе, цена которого минимальна. --
-----------------------------------------------------------------------------
SELECT DISTINCT p.name
FROM products p, stock_info si
WHERE p.productID = si.productID
AND si.productPrice = (
    SELECT MIN(productPrice)
    FROM stock_info);
-----------------------------------------------------------------------------
