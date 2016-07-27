
DROP LANGUAGE IF EXISTS pldotnet CASCADE;
DROP FUNCTION IF EXISTS pldotnet_call_handler() CASCADE;

CREATE FUNCTION pldotnet_call_handler() RETURNS language_handler
    AS :'SOPATH'
    LANGUAGE C;

CREATE LANGUAGE pldotnet
    HANDLER pldotnet_call_handler;

CREATE FUNCTION pldotnet_load_dll(TEXT) RETURNS INT AS 'PLMono._internal:load_dll' LANGUAGE pldotnet;
CREATE FUNCTION pldotnet_raise_elog(INT, TEXT) RETURNS VOID AS 'PLMono._internal:raise_elog' LANGUAGE pldotnet;

