create Maze

: AllocateMaze ( pointer n m )
    * cells allot
;

: FillMaze ( pointer n m -- )
    *
    0
    do
	dup ( pointer pointer )
	5 swap  ( pointer 5 pointer)  ( By now, just putting 5's )
	I cells + ! ( pointer )
	1
    +loop
    drop
;

: WriteMaze ( pointer n m -- )
    *
    0
    do
	dup ( pointer pointer )
	I cells + @ . ( pointer )
	1
    +loop
    drop
;
