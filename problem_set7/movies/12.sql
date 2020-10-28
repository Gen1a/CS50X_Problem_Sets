SELECT title Title
FROM movies
WHERE id IN
	(SELECT movie_id
	FROM stars
	WHERE person_id =
		(SELECT id
		FROM people
		WHERE name = "Johnny Depp"))
AND id IN
	(SELECT movie_id
	FROM stars
	WHERE person_id =
		(SELECT id
		FROM people
		WHERE name = "Helena Bonham Carter"));

-- SELECT title Title
-- FROM movies
-- JOIN stars ON movies.id = stars.movie_id
-- JOIN people ON stars.person_id = people.id
-- WHERE name = "Johnny Depp"
-- AND movie_id IN
--     (SELECT movie_id
--     FROM stars
--     JOIN people ON stars.person_id = people.id
--     WHERE name = "Helena Bonham Carter");
-- Shorter version with same execution time