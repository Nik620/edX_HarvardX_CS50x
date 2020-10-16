/*query to determine avg rating of all movies
released in 2012. output table with just avg*/
SELECT AVG(rating) FROM ratings WHERE movie_id
IN (SELECT id FROM movies WHERE year = 2012);
