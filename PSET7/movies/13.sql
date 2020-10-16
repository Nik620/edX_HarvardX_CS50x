/*query to list names of all people who starred
in movie in which Kevin Bacon also starred.
Use the Kevin Bacon born in 1958, and should not
be included in the list. Output single column for
name of each person*/
SELECT DISTINCT(name) from people
JOIN stars ON people.id = stars.person_id
WHERE stars.movie_id IN
(SELECT movie_id from stars
JOIN people ON stars.person_id = people.id
WHERE people.name = "Kevin Bacon"
AND people.birth = 1958)
AND name != "Kevin Bacon";
