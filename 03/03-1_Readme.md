# Notes
This file contains additional notes to the first task for the current seminar.

## Running the program
I have run the program several times and received the following results:

1. Current pid = 18724, current ppid = 16241
2. Current pid = 18725, current ppid = 16241
3. Current pid = 18730, current ppid = 16241

After which I opened a new terminal window and observed the following behavior:

1. Current pid = 19053, current ppid = 18912
2. Current pid = 19057, current ppid = 18912
3. Current pid = 19058, current ppid = 18912

## Conclusions
Therefore, I observed that the pid gives me a new thread each time I run the application, while the ppid (parent) corresponds to the thread of the terminal. Thus making a new terminal changes the ppid.
