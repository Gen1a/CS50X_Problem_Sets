SELECT m.title Movie, r.rating Rating
FROM ratings r
JOIN movies m
ON m.id = r.movie_id
WHERE m.year = 2010
ORDER BY r.rating DESC, m.title;