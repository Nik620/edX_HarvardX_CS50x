/*query to list titles and release years of all
Harry Potter movies, in chronological order.
output table with two columns for title and
release year*/
SELECT title, year FROM movies
WHERE title LIKE "Harry Potter%" ORDER BY year;
