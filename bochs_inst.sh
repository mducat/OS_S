wget http://bochs.sourceforge.net/svn-snapshot/bochs-20200229.tar.gz
cd bochs*
./configure --enable-debugger --enable-disasm --enable-debugger-gui
make
sudo make install
