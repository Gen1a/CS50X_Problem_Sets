SELECT name Name
FROM people
WHERE name != "Kevin Bacon" AND id IN(
	SELECT person_id
	FROM stars
	WHERE movie_id IN(
		SELECT movie_id
		FROM stars
		WHERE person_id IN(
			SELECT id
			FROM people
			WHERE name = "Kevin Bacon" AND birth = 1958)));
-- Execution time = 689ms

-- SELECT DISTINCT name Name
-- FROM people JOIN stars ON people.id = stars.person_id
-- WHERE name != "Kevin Bacon" AND movie_id IN(
-- 	SELECT movie_id
-- 	FROM people JOIN stars ON people.id = stars.person_id
-- 	WHERE name = "Kevin Bacon" AND birth = 1958);
-- Same output put execution time = 21309ms
