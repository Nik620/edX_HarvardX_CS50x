/*query to list names of people who starred in
Toy story. table with single column for name
of each person*/
/*link people to movies through stars*/
SELECT name FROM people WHERE id IN
(SELECT person_id FROM stars WHERE movie_id IN
(SELECT id FROM movies WHERE title = "Toy Story"));
