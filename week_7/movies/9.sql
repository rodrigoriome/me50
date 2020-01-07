SELECT name
FROM people
WHERE id in
(
    SELECT DISTINCT(person_id)
    FROM people
    JOIN stars ON people.id = stars.person_id
    JOIN movies ON movies.id = stars.movie_id
    WHERE year = 2004
)
ORDER BY birth;
