# subprocesser
c++ program that makes some of google colab ! commands into subprocess calls

supported commands are

!curl
!git clone
!rm
!unzip
!mkdir
%cd

Anything that is not one of these commands will be left as it is
Indentation gets preserved for both supported commands and everything else
