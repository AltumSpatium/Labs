/* Task 1 */
CREATE TABLE Log_Table (
  id NUMBER PRIMARY KEY,
  username VARCHAR2(20),
  log_date TIMESTAMP WITH LOCAL TIME ZONE);
  
CREATE SEQUENCE Log_Sequence
  START WITH 1;
  
CREATE OR REPLACE TRIGGER Logon_Trigger
  AFTER LOGON ON smart.SCHEMA
BEGIN
  INSERT INTO Log_Table
    VALUES (Log_Sequence.NEXTVAL, ORA_LOGIN_USER, SYSDATE);
END;
/

/* Task 2 */
CREATE TABLE Students (
  id NUMBER,
  name VARCHAR2(20),
  group_id NUMBER);
  
CREATE TABLE Groups (
  id NUMBER,
  name VARCHAR2(20),
  c_val NUMBER);
  
/* Task 3 */
CREATE SEQUENCE Students_Sequence
  START WITH 10000;
  
CREATE SEQUENCE Groups_Sequence
  START WITH 1;

-- ID check
CREATE OR REPLACE TRIGGER Check_Id_Students
  BEFORE INSERT ON Students
  FOR EACH ROW
DECLARE
  id_count NUMBER;
BEGIN
  SELECT COUNT(*)
  INTO id_count
  FROM Students
  WHERE id = :NEW.id;
  
  IF id_count > 0 THEN
    RAISE VALUE_ERROR;
  END IF;
END;
/

CREATE OR REPLACE TRIGGER Check_Id_Groups
  BEFORE INSERT ON Groups
  FOR EACH ROW
DECLARE
  id_count NUMBER;
BEGIN
  SELECT COUNT(*)
  INTO id_count
  FROM Groups
  WHERE id = :NEW.id;
  
  IF id_count > 0 THEN
    RAISE VALUE_ERROR;
  END IF;
END;
/

-- Autoincrement
CREATE OR REPLACE TRIGGER Autoincrement_Id_Students
  BEFORE INSERT ON Students
  FOR EACH ROW
BEGIN
  IF :NEW.id IS NULL THEN
    :NEW.id := Students_Sequence.NEXTVAL;
  END IF;
END;
/

CREATE OR REPLACE TRIGGER Autoincrement_Id_Groups
  BEFORE INSERT ON Groups
  FOR EACH ROW
BEGIN
  IF :NEW.id IS NULL THEN
    :NEW.id := Groups_Sequence.NEXTVAL;
  END IF;
END;
/

-- Unique group name
CREATE OR REPLACE TRIGGER Unique_Group_Name
  BEFORE INSERT ON Groups
  FOR EACH ROW
DECLARE
  name_count NUMBER;
BEGIN
  SELECT COUNT(*)
  INTO name_count
  FROM Groups
  WHERE name = :NEW.name;
  
  IF name_count > 0 THEN
    RAISE VALUE_ERROR;
  END IF;
END;
/

/* Task 4 */
CREATE OR REPLACE TRIGGER Foreign_key
  BEFORE DELETE ON Groups
  FOR EACH ROW
DECLARE
  CURSOR c_StudentsOfGroup IS
    SELECT *
    FROM STUDENTS
    WHERE group_id = :OLD.id;
BEGIN
  FOR student IN c_StudentsOfGroup LOOP
    DELETE
    FROM Students
    WHERE id = student.id;
  END LOOP;
END;
/
