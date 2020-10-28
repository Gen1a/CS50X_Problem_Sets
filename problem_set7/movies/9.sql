SELECT name Name
FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON stars.movie_id = movies.id
WHERE movies.year = 2004
GROUP BY people.name
ORDER BY people.birth;

-- SELECT DISTINCT name Name
-- FROM people
-- JOIN stars ON stars.person_id = people.id
-- JOIN movies ON stars.movie_id = movies.id
-- WHERE movies.year = 2004
-- ORDER BY birth;
-- Alternative with DISTINCT instead of GROUP BY (+ 10.000ms execution time)