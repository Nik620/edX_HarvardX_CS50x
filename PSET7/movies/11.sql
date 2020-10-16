/*query to list titles of five highest rated
movies (in order) that Chadwick Boseman starred
in, starting with highest rated. single column
of movies.*/
SELECT title FROM movies
JOIN ratings ON movies.id = ratings.movie_id
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Chadwick Boseman"
ORDER BY rating DESC LIMIT 5;
