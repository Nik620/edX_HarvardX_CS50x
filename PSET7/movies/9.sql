/*query to list names of people who starred in
2004 ordered by birth year. list only the
birth year. no order for peoples names
with same birth year. they should appear once
regardless of appearing in movie more than once
in 2004*/
SELECT DISTINCT(name) FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE movies.year = 2004 ORDER BY birth ASC;
