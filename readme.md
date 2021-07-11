## RAIF - Redundent Array of Inexpensive Files


### Building from source

Run "./build.sh" in a bash shell to build, then "./build.sh install" to install.

If you don't want to install in /usr/bin, then run "./build.sh install \</path/to/install/directory\>" with the
desired install location. Make sure the path argument is a full path, and not a relative one.

### Usage

"\<shell command\> | raif\_write \<block size\> \<file full path 1\> \<file full path 2\> ... \<parity file full path\>"
will write data strips to the files and the last file is the parity file which holds the bit sums.


"raif\_gen\_missing \<missing index\> \<file full path 1\> \<file full path 2\> ... \<parity file full path\>"
will generate a missing file by calculating from other present files.


"raif\_read \<block size\> \<file full path 1\> \<file full path 2\> ... \<parity file full path\> \<| command [or] > file\>"
will read (asynchronously) from the files to stdout.
