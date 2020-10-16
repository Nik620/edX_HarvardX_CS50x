/*query to list all movies released in 2010 and
their ratings, descending by ratings. For movies
with same rating, order alphabetically. Movies
with no ratings are not included.*/
/*CREATE TABLE tenratings (id INT, title TEXT, year NUMERIC, rating NUMERIC);
SELECT title, rating FROM tenratings WHERE*/
SELECT title, rating FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE year = 2010
ORDER BY rating DESC, title ASC;
