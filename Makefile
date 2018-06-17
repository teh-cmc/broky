.PHONY: toc

toc:
	docker run --rm -it -v ${PWD}:/usr/src jorgeandrada/doctoc --github

cmake:
	cmake -H. -Bbuilds/Debug -DCMAKE_BUILD_TYPE=Debug
	cmake -H. -Bbuilds/Release -DCMAKE_BUILD_TYPE=Release
