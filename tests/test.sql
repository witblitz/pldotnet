
SET client_min_messages=DEBUG2;

SELECT pldotnet_load_dll(CONCAT(:'PWD', '/test.dll'));

CREATE FUNCTION pldotnet_test() RETURNS INT AS 'PLMonoTest.Test:test' LANGUAGE pldotnet;


CREATE FUNCTION pldotnet_test_param_1(INT) RETURNS INT AS 'PLMonoTest.Test:test_param_1' LANGUAGE pldotnet;

CREATE FUNCTION pldotnet_test_2_params(INT, INT) RETURNS INT AS 'PLMonoTest.Test:test_2_params' LANGUAGE pldotnet;

CREATE FUNCTION pldotnet_test_string_param(TEXT) RETURNS TEXT AS 'PLMonoTest.Test:test_string_param' LANGUAGE pldotnet;

CREATE FUNCTION pldotnet_multiply_string(TEXT, INT) RETURNS TEXT AS 'PLMonoTest.Test:multiply_string' LANGUAGE pldotnet;


SELECT pldotnet_test();
SELECT pldotnet_test();

SELECT pldotnet_test_param_1(90);
SELECT pldotnet_test_2_params(90, 80);
SELECT pldotnet_test_string_param('HAAAAI');
--SELECT pldotnet_multiply_string('HAAAAI', 100);


