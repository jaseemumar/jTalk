all:compile
.PHONY:compile clean
compile:
	@cd src; \
	javac ./cs296JTalk2/*.java;\
	javac jtalkG20.java;\
	jar -cmf ../MANIFEST.MF ../jtalkG20.jar *.class  cs296JTalk2
clean:
	@rm -f ./src/cs296JTalk2/*.class;
	@rm -f ./src/jtalkG20.class;
	@rm -f *.txt
	@rm -f *.jar
	
