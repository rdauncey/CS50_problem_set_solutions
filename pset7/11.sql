SELECT title FROM
movies JOIN stars ON movies.id = stars.movie_id JOIN
ratings ON stars.movie_id = ratings.movie_id
WHERE person_id IN (SELECT id FROM people WHERE name = "Chadwick Boseman")
ORDER BY rating DESC LIMIT 5;