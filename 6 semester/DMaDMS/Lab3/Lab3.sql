-- Needed tables
CREATE TABLE Metrics (
  id NUMBER PRIMARY KEY,
  name VARCHAR(50),
  danger_value NUMBER,
  critical_value NUMBER);
  
CREATE TABLE Log_XML (
  id NUMBER PRIMARY KEY,
  xml CLOB);
  
CREATE SEQUENCE Metrics_Sequence
  START with 1;
  
CREATE SEQUENCE Log_XML_Sequence
  START WITH 1;
  
-- Metrics DML procedures
CREATE OR REPLACE PROCEDURE AddMetric
  (name IN VARCHAR2, danger_value IN NUMBER, critical_value IN NUMBER)
IS
BEGIN
  INSERT INTO Metrics
    VALUES (Metrics_Sequence.NEXTVAL, name, danger_value, critical_value);
END;
/

CREATE OR REPLACE PROCEDURE DeleteMetric
  (name IN VARCHAR2)
IS
BEGIN
  DELETE
  FROM Metrics
  WHERE Metrics.name = name;
END;
/

-- Checking task
CREATE OR REPLACE PROCEDURE Check_Metrics
IS
  v_Value NUMBER := 0;
  CURSOR c_Metrics IS
    SELECT *
    FROM Metrics;
    
  PROCEDURE Write_Log
    (log_msg IN CLOB)
  IS
  BEGIN
    INSERT INTO Log_XML
      VALUES (Log_XML_Sequence.NEXTVAL, log_msg);
    COMMIT;
  END;
  
  PROCEDURE Log_Session
  IS
  BEGIN
    Write_Log(DBMS_XMLGEN.GETXML('SELECT * FROM V$SESSION'));
  END;
    
  PROCEDURE Send_Email
    (msg IN VARCHAR2)
  IS
  BEGIN
    APEX_MAIL.SEND(p_to => 'sinner.in.me11@gmail.com', p_from => 'sinner.in.me11@gmail.com',
                   p_subj => 'Alert!', p_body => msg);
    APEX_MAIL.PUSH_QUEUE;
  EXCEPTION
    WHEN OTHERS THEN
      Write_Log('Error sending email');
      NULL;
  END;
BEGIN
  Write_Log('Calling Job');
  
  FOR metric IN c_Metrics LOOP
    IF metric.name = 'sysstat' THEN
      SELECT (a.value / b.value)
      INTO v_Value
      FROM V$SYSSTAT a, V$SYSSTAT b
      WHERE a.name = 'parse time cpu' AND b.name = 'CPU used by this session';
    ELSIF metric.name = 'pgastat' THEN
      SELECT value
      INTO v_Value
      FROM V$PGASTAT
      WHERE name = 'total PGA allocated';
    END IF;
    
    IF v_Value >= metric.critical_value THEN
      Write_Log('Critical value');
      Log_Session;
      Send_Email('Critical value');
    ELSIF v_Value >= metric.danger_value THEN
      Write_Log('Danger value');
      Send_Email('Critical value');
    END IF;
  END LOOP;
END;
/

-- Job creating
BEGIN
  DBMS_SCHEDULER.CREATE_JOB(job_name => 'job_metrics',
                            job_type => 'STORED_PROCEDURE',
                            job_action => 'Check_Metrics',
                            start_date => SYSTIMESTAMP,
                            end_date => SYSTIMESTAMP + 1,
                            repeat_interval => 'FREQ=MINUTELY;INTERVAL=1',
                            enabled => TRUE,
                            auto_drop => FALSE,
                            comments => 'No comments.');
END;
/

-- Adding some metrics
BEGIN
  ADDMETRIC('sysstat', 1, 1);
  ADDMETRIC('pgastat', 0, 0);
  --SYS.DBMS_SCHEDULER.STOP_JOB('metrics_job, sys.dw_jobs');
END;
/

-- Stopping job
BEGIN
  DBMS_SCHEDULER.STOP_JOB('job_metrics, sys.dw_jobs');
END;
/
