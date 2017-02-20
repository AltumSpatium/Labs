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

/* Task 5 */
CREATE TABLE Journal (
  id NUMBER PRIMARY KEY,
  action VARCHAR2(6),
  student_id_old NUMBER,
  student_id_new NUMBER,
  student_name_old VARCHAR2(20),
  student_name_new VARCHAR2(20),
  student_group_old NUMBER,
  student_group_new NUMBER,
  action_date TIMESTAMP);
  
CREATE SEQUENCE Journal_Sequence
  START WITH 1;
  
CREATE OR REPLACE TRIGGER Journal_Trigger
  AFTER INSERT OR UPDATE OR DELETE ON Students
  FOR EACH ROW
BEGIN
  IF INSERTING THEN
    INSERT INTO Journal
      VALUES (Journal_Sequence.NEXTVAL, 'INSERT', NULL, :NEW.id, NULL,
              :NEW.name, NULL, :NEW.group_id, SYSDATE);
  ELSIF UPDATING THEN
    INSERT INTO Journal
      VALUES (Journal_Sequence.NEXTVAL, 'UPDATE', :OLD.id, :NEW.id, :OLD.name,
              :NEW.name, :OLD.group_id, :NEW.group_id, SYSDATE);
  ELSIF DELETING THEN
    INSERT INTO Journal
      VALUES (Journal_Sequence.NEXTVAL, 'DELETE', :OLD.id, NULL, :OLD.name,
              NULL, :OLD.group_id, NULL, SYSDATE);
  END IF;
END;
/

/* Task 6 */
CREATE OR REPLACE PROCEDURE Restore_Info
  (moment IN TIMESTAMP)
IS
CURSOR c IS
  SELECT *
  FROM Journal
  WHERE action_date < moment
  ORDER BY action_date;
BEGIN
  FOR rec IN c LOOP
    IF rec.action = 'INSERT' THEN
      DELETE
      FROM Students
      WHERE id = rec.student_id_new;
    ELSIF rec.action = 'UPDATE' THEN
      UPDATE Students
        SET name = rec.student_name_old, group_id = rec.student_group_old
        WHERE id = rec.student_id_old;
    ELSIF rec.action = 'DELETE' THEN
      INSERT INTO Students
        VALUES (rec.student_id_old, rec.student_name_old, rec.student_group_old);
    END IF;
    
    DELETE
    FROM Journal
    WHERE id = rec.id;
  END LOOP;
END;
/
