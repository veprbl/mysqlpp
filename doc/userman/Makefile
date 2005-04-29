BASENAME=userman
DOCFILE=$(BASENAME).xml
STYLESHEET=$(BASENAME).xsl

all: html/index.html

html/index.html: $(DOCFILE) $(STYLESHEET)
	@mkdir -p html
	@#xmllint --xinclude --valid --noout $(DOCFILE)
	xsltproc --xinclude -o html/ $(STYLESHEET) $(DOCFILE)
