/*write query to find movies with IMDB rating of
10.0. output single colmn with movies with 10.0
rating*/
SELECT COUNT(movie_id) FROM ratings
WHERE rating = 10.0;
