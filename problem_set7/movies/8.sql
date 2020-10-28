-- SELECT name Name
-- FROM people
-- JOIN stars
-- ON people.id = stars.person_id
-- JOIN movies
-- ON movies.id = stars.movie_id
-- WHERE movies.title = "Toy Story";
-- Query above takes 5000ms to execute...

-- CREATE INDEX name_index ON people(name);
-- CREATE INDEX person_index ON stars(person_id);
-- CREATE INDEX movie_index ON movies(id);
-- Tried to speed execution by creating indices first but doesn't seem to work?


SELECT name Name
FROM people
WHERE id IN
	(SELECT person_id
	FROM stars
	WHERE movie_id =
		(SELECT id
		FROM movies
		WHERE title = "Toy Story"));
-- Nested query takes only 390ms to execute