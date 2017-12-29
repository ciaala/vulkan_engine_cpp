Free Form Ideas
---------------

If i create the Engine to be multi-threaded I should also analyse the testing of such a configuration.

I should consider to read something about this topic to identify a couple of guidelines.

I am thinking of such a scenario:

 - we can create the operations inside runnables
 - usually we should schedule them in parallel on a thread-pool.
 - we create a custom thread-pool which is single threaded
 - the pool run them also in a random sequence
 - check the end result

Testing
-------

What is the result if we exclude the result