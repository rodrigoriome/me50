SELECT title
FROM movies
WHERE id IN
(
    SELECT movie_id
    FROM stars
    JOIN people ON people.id = stars.person_id
    WHERE name = "Johnny Depp"
)
AND id IN
(
    SELECT movie_id
    FROM stars
    JOIN people ON people.id = stars.person_id
    WHERE name = "Helena Bonham Carter"
);
