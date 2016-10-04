
SET client_min_messages=DEBUG2;

CREATE OR REPLACE FUNCTION test_code (INT, INT) RETURNS TEXT AS $$

	pldotnet.raise("whoop whoop!");


$$ LANGUAGE pldotnet;


SELECT test_code(10, 20);

