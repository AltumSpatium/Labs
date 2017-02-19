/* Task 1 */
CREATE TABLE MyTable (
  id NUMBER PRIMARY KEY,
  val NUMBER);

/* Task 2 */
BEGIN
  FOR v_Counter IN 1..10000 LOOP
    INSERT INTO MyTable
      VALUES (v_Counter, ROUND(DBMS_RANDOM.VALUE(1, 100), 0));
  END LOOP;
END;
/

/* Task 3 */
CREATE OR REPLACE FUNCTION MoreEven
  RETURN VARCHAR2
IS
  v_Result VARCHAR2(5);
  v_EvenCount NUMBER := 0;
  v_OddCount NUMBER := 0;
  CURSOR c_Even IS
    SELECT COUNT(*)
    FROM MyTable
    WHERE MOD(MyTable.val, 2) = 0;
  CURSOR c_Odd IS
    SELECT COUNT(*)
    FROM MyTable
    WHERE MOD(MyTable.val, 2) <> 0;
BEGIN
  OPEN c_Even;
  OPEN c_Odd;
  
  FETCH c_Even INTO v_EvenCount;
  FETCH c_Odd INTO v_OddCount;
  
  IF c_Even%NOTFOUND THEN
    v_Result := 'FALSE';
  END IF;
  
  IF c_Odd%NOTFOUND THEN
    v_Result := 'TRUE';
  END IF;
  
  IF v_EvenCount > v_OddCount THEN
    v_Result := 'TRUE';
  ELSIF v_EvenCount = v_OddCount THEN
    v_Result := 'EQUAL';
  ELSE
    v_Result := 'FALSE';
  END IF;
  
  DBMS_OUTPUT.PUT_LINE(v_Result);
  RETURN v_Result;
END;
/

/* Task 4 */
CREATE OR REPLACE FUNCTION InsertText
  (id IN NUMBER)
  RETURN VARCHAR2
IS
  v_Result VARCHAR2(100);
BEGIN
  v_Result := 'INSERT INTO MyTable VALUES (' || id || ', ROUND(DBMS_RANDOM.VALUE(1, 100), 0);';
  
  DBMS_OUTPUT.PUT_LINE(v_Result);
  RETURN v_Result;
END;
/

/* Task 5 */
CREATE OR REPLACE PROCEDURE MyTableInsert
  (id IN NUMBER, val IN NUMBER) 
IS
BEGIN
  INSERT INTO MyTable
    VALUES (id, val);
END;
/

CREATE OR REPLACE PROCEDURE MyTableUpdate
  (id IN NUMBER, newVal IN NUMBER)
IS
BEGIN
  UPDATE MyTable
    SET val = newVal
    WHERE MyTable.id = id;
END;
/

CREATE OR REPLACE PROCEDURE MyTableDelete
  (id IN NUMBER)
IS
BEGIN
  DELETE FROM MyTable
  WHERE MyTable.id = id;
END;
/
