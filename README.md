# tinyshell

Fork of Memfault [tinny shell](https://github.com/memfault/interrupt/tree/master/example/firmware-shell) 
For description please have a look at [Memfault website](https://interrupt.memfault.com/blog/firmware-shell)

Two small changes compare to original version:
1. As I couldn't get it to work with Putty (it sends CR when Enter Key is pressed), which I am using as a serial terminal, I had to add CR as the shell command line ending. It can be configured by *substitute_rn* variable in *shell_boot*.
```c
void shell_boot(const sShellImpl *impl, int substitute_rn)  
```
  

2. Sample program is run under linux.

First you have to create two connected virtual serial ports
```shell
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```
  

And use one of the ports in the sample app and second in Putty.