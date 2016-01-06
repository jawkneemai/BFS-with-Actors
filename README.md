# BFS-with-Actors
Given a certain text file with movies and its actors (the .list files are an example), this will construct a graph that connects actors to other actors in the same movie, with the edges labeled with that movie. In other words, every actor is their own node and will have edges to other actors if they share a common movie. That edge will be labeled the movie they both appear in. The breadth-first search will find the shortest connection between any two actors (through movies and other actors) in the text file and display its path, similar to the "six degrees of separation" concept. (i.e. Bob was in X who had Joe who was in Y who had Leonardo.)

The text file must list the actors and the movies they are in, in the following format:
```
Marc Abbink
|Gone Girl (2014)
|The Dark Knight Rises (2012)
```

