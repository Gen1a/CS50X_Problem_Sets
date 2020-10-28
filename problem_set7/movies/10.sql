SELECT name Name
FROM people
WHERE id IN
	(SELECT person_id
	FROM directors
	WHERE movie_id IN
		(SELECT movie_id
		FROM ratings
		WHERE rating >= 9.0));
-- Seems safer to use DISTINCT or GROUP BY but produces the same results