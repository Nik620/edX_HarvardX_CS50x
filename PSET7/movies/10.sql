/*query to list names of people who directed a
movie that recieved a rating of at least 9.0.
output just name of each person*/
SELECT DISTINCT(name) FROM people
JOIN directors ON people.id = directors.person_id
JOIN ratings ON directors.movie_id = ratings.movie_id
WHERE rating >= 9.0;
