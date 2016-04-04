gb = go build -o dist/$(1)/tester$(2)

all:
	env GOOS=windows GOARCH=386 $(call gb,windows,.exe)
	env GOOS=linux GOARCH=386 $(call gb,linux)
	env GOOS=darwin GOARCH=amd64 $(call gb,osx)
	cd dist; for dir in * ; do \
		cp ../minunit.h $$dir/; \
		zip -r $$dir.zip $$dir; \
	done

clean:
	rm -rf dist
