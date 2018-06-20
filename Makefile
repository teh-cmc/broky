.PHONY: toc

toc:
	docker run --rm -it -v ${PWD}:/usr/src jorgeandrada/doctoc --github

init:
	cmake -H. -Bbuilds/Debug -DCMAKE_BUILD_TYPE=Debug
	# cmake -H. -Bbuilds/Release -DCMAKE_BUILD_TYPE=Release

build:
	cmake --build builds/Debug
	# cmake --build builds/Release

test: build
	CTEST_OUTPUT_ON_FAILURE=1 \
		cmake --build builds/Debug/ --target test
