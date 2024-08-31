-- create table test("ID" serial primary key,  val integer);

-- insert into test(val) values (11), (22);

-- select val from test limit 1;

create table if not exists "Users"("ID" serial primary key, "login" varchar(20) unique, "password" varchar(50));
-- 
-- DO $$ DECLARE
-- 	r RECORD;
-- BEGIN
-- 	FOR r IN (SELECT tablename FROM pg_tables WHERE schemaname = 'public') LOOP
-- 		EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident(r.tablename) || ' CASCADE';
-- 	END LOOP;
-- END $$;

CREATE PROCEDURE add_new_user(
    IN l VARCHAR,
    IN pw VARCHAR
)
LANGUAGE plpgsql
AS $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM "Users" WHERE "Users"."login" = l AND "Users"."password" = pw) THEN
        INSERT INTO "Users" ("login", "password")
        VALUES (l, pw);
    END IF;
END;
$$;

call add_new_user('user1', '1234');

select * from "Users";
