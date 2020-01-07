SELECT DISTINCT(name)
FROM people
JOIN stars ON stars.person_id = people.id
WHERE movie_id IN
(
    SELECT movies.id
    FROM movies
    INNER JOIN stars ON stars.movie_id = movies.id
    JOIN people ON people.id = stars.person_id
    WHERE name = "Kevin Bacon"
    AND birth = 1958
)
AND name != "Kevin Bacon";
