


PROJECT BY :

Kaushik Sivakumar  kxs104820@utdallas.edu 2021108266
Sethu Madhav Kollu
Prachi Shende




NOTE:


WE have finished the 100 mark code completely
however, we dint have time to finish the bonus 20% part ,so we have just included a file bonuspoints.c which prints the file 
in the current folder as well subfolders for partial credit in the bonus points     
ALso This code has been designed and made to run suceessfully for 58 files in the folder.
_____________________________________________________________________________________________________________________

INTRODUCTION

The project is done in C.
_____________________________________________________________________________________________________________________

OBJECTIVE

This assignment is an introduction to threads and IPC synchronization mechanisms. Project objective is to use 
pthreads to implement a pipelined grep application.Pipelined grep will search all files in the current directory. 
The user is also able to filter files out of the search based on file size, file user id, and file group id. 
This is a five stage pipeline where there is one thread per stage.There is a buffer between the adjacent stages.

______________________________________________________________________________________________________________________


DELIVERABLES

---------------------------------------------------------------------------
(1) what you got working

The code is working fine and it outputs in the following manner:

{cslinux1:~} ./pipegrep 1000 -1 -1 -1 "searchstring"

./kk (1)searchstring

./kk (2)searchstring

./kk (3)searchstring
********* You have found 3 matches *******
You are done!!!!


What it outputs:
a) kk : The file name in which the search string was found
b) (1),(2),(3)     : The line number in the file where the string was found
c) "searchstring"    : The  string which is searched

-----------------------------------------------------------------------------

(2) a description of your critical sections


This is a five stage pipeline where there is one thread per stage.There is a buffer between the adjacent stages.
Each of these stages where the data/products are produced or consumed are protected by a mutex(pthread_mutex_lock)/(pthread_mutex_unlock), 
so that they do not overlap with each other and cause a race condition. There is also a conditon to check if the data/product from previous stage 
is consumed or else to wait till it is consumed.


RACE CONDITION:
A situation where several processes/threads access and manipulate the same data concurrently and the outcome of the execution depends on the particular
order in which the access takes place is race condition.

So in our project the sequence of all the 5 stages is very important and we need to maintain this sequence for successfull execution of the project.
The threads from various stages must not overlap, a product that's produced in current stage is to be consumed in the next, consumption of a product without
its production in the previous stage is not acceptable as if you try to consume a product when it has not even been generated or is in the process of being
generated, it would cause a wrong output.

-----------------------------------------------------------------------------
(3) The program was tested for 58 files.the performance of the code was tested using clock() and the optimum buffer size was found to be 50

-----------------------------------------------------------------------------

(4) tell me in which stage you would add an additional thread to improve performance and why you chose
that stage
The number of threads in this project would suffice the need of optimal performance, but in order to enhance the
performance we can add a thread to the search stage where the string is searched in the file line by line. Having
an additional thread would speed the process of searching through the multiple occurances of the string in the file
and hence would enhance its performance.

------------------------------------------------------------------------------
(5) Any bugs in your code.
No bugs in the code

_______________________________________________________________________________________________________________________


TO COMPILE

Usage: pipegrep <bufesize> <filesize> <uid> <gid> <word>
1)make
3)Execute ./pipegrep 10 -1 -1 -1 "string to be searched"
