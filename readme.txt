Entropy

This was tested on an Ubuntu Linux system. 
The code should be transferrable to any environment configured to include OpenFrameworks libraries.

On Linux, please change config.make to point to your OpenFrameworks installation, then run:
make
cd bin
./project


On Windows, please:
Open visual studio
Use openframeworks projectGenerator to generate an app
Replace the /src and /bin folders with mine
Compile and run


 !  Bug  !  
Due to how I (lazily) implemented the keys array, if you are holding down
a key and drag the mouse off the screen or do stuff involving the
keyboard and window at the same time, it may or may not segfault... 
beware...



