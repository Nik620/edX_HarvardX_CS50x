/*query to lsit tiles of movies releast post 2018,
in alphabetical order. single column for title
of each movie. also include movies with release
dates in the future*/
SELECT title FROM movies
WHERE year >= 2018 ORDER BY title;
/*count with ', COUNT(title)'*/
