/* Task 1 */
CREATE TABLE MyTable (
  id NUMBER PRIMARY KEY,
  val NUMBER);
 
CREATE SEQUENCE MyTable_Sequence
  START WITH 1;
  
/* Task 2 */
BEGIN
  LOOP
    INSERT INTO MyTable
      VALUES (MyTable_Sequence.NEXTVAL, ROUND(DBMS_RANDOM.VALUE(1, 100), 0));
    EXIT WHEN MyTable_Sequence.CURRVAL > 10000;
  END LOOP;
END;
/

/* Task 3 */
CREATE OR REPLACE TYPE queue_element_t AS OBJECT (
  val NUMBER);
  
BEGIN
  DBMS_AQ.CREATE_QUEUE_TABLE(
    queue_table => 'my_queue_table',
    queue_payload_type => 'queue_element_t');
END;
/

BEGIN
  DBMS_AQ.CREATE_QUEUE(
    queue_name => 'my_queue',
    queue_table => 'my_queue_table');
  DBMS_AQ.START_QUEUE(
    queue_name => 'my_queue');
END;
/
  
DECLARE
  message_properties DBMS_AQ.message_properties_t;
  queue_options DBMS_SQ.enqueue_options_t;
  message_id RAW(16);
  CURSOR c IS
    SELECT *
    FROM MyTable;
BEGIN
  FOR elem IN c LOOP
    DBMS_AQ.ENQUEUE('my_queue', queue_options, message_properties,
      queue_element_t(elem.val), message_id);
  END LOOP;
  COMMIT;
END;
/

/* Task 4 */
CREATE TABLE ResultTable (
  id NUMBER PRIMARY KEY,
  start_dt TIMESTAMP,
  finish_dt TIMESTAMP,
  id_myTable NUMBER,
  id_subscriber NUMBER);
  
CREATE SEQUENCE ResultTable_Sequence
  START with 1;
  
/* Task 5 */
CREATE OR REPLACE PROCEDURE Subscriber
  (context IN RAW, RegInfo IN SYS.AQ$_reg_info, 
   desc IN SYS.AQ$_descriptor, payload RAW, payload_l NUMBER)
IS
  l_tr_obj queue_element_t;
  l_msg_props DBMS_AQ.message_properties_t;
  l_queue_opts  DBMS_AQ.dequeue_options_t;
  l_msg_id RAW(16);
  start_dt TIMESTAMP;
  finish_dt TIMESTAMP;
  c_MT_id NUMBER := 0;
BEGIN
  l_queue_opts.consumer_name := desc.consumer_name;
  l_queue_opts.msgid := desc.msg_id;
  DBMS_AQ.DEQUEUE(desc.queue_name, l_queue_opts, l_msg_props, l_tr_obj, l_msg_id);
    
  start_dt := SYSTIMESTAMP;
  DBMS_LOCK.Sleep(l_tr_obj.val);
  finish_dt := SYSTIMESTAMP;
  
  SELECT id
  INTO c_MT_id
  FROM MyTable
  WHERE val = l_tr_obj.val;
  
  INSERT INTO ResultTable
    VALUES (ResultTable_Sequence.NEXTVAL, start_dt, finish_dt, c_MT_id, 0);
    commit;
  exception
    when others then 
        null;
  end;
  /

DECLARE
  subsc_t SYS.AQ$_AGENT;
  subsc_addr VARCHAR2(1024) := 'AQADMIN.MY_QUEUE';
BEGIN
  subsc_t := SYS.AQ$_AGENT('s1', subsc_addr, 0);
  DBMS_AQADM.add_subscriber(
    queue_name => 'my_queue',
    subscriber => subsc_t);
  DBMS_AQ.register(SYS.AQ$_reg_info_list(
    SYS.AQ$_reg_info('my_queue:s1', DBMS_AQ.namespace_aq, 'subscriber', hextoraw('FF'))), 1);
  
  subsc_t := SYS.AQ$_AGENT('s2', subsc_addr, 0);
  DBMS_AQADM.add_subscriber(
    queue_name => 'my_queue',
    subscriber => subsc_t);
  DBMS_AQ.register(SYS.AQ$_reg_info_list(
    SYS.AQ$_reg_info('my_queue:s2', DBMS_AQ.namespace_aq, 'subscriber', hextoraw('FF'))), 1);
    
  subsc_t := SYS.AQ$_AGENT('s3', subsc_addr, 0);
  DBMS_AQADM.add_subscriber(
    queue_name => 'my_queue',
    subscriber => subsc_t);
  DBMS_AQ.register(SYS.AQ$_reg_info_list(
    SYS.AQ$_reg_info('my_queue:s3', DBMS_AQ.namespace_aq, 'subscriber', hextoraw('FF'))), 1);
END;
/
