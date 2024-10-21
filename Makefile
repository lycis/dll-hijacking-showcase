
goodlib:
	clang++ -shared -o mylog.dll mylog.cpp -fuse-ld=lld -Wl,--out-implib,libmylog.a

evillib:
	clang++ -shared -o mylog.dll mylog-evil.cpp -fuse-ld=lld -Wl,--out-implib,libmylog.a

vulnerable00:
	clang++ -o vuln00.exe 00-vulnerable.cpp

vulnerable01: goodlib
	clang++ -o vuln01.exe 01-vulnerable_fullpatch.cpp
	mkdir libs
	mv mylog.dll ./libs/

vulnerable02: md5sum goodlib
	MYLIBSUM=$(./md5sum.exe mylog.dll) && echo "#define MYLOG_CHECKSUM \"${MYLIBSUM}\"" > checksums.h
	clang++ -o vuln02.exe 02-vulnerable_checksum.cpp md5.cpp
	mkdir libs
	mv mylog.dll ./libs/

md5sum:
	clang++ -o md5sum.exe -DSTANDALONE md5.cpp

clean:
	rm -rf ./libs/; rm *.exe; rm *.dll; rm checksums.h; rm *.a
