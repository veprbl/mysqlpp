BASENAME=userman
DOCFILE=$(BASENAME).xml

STYLESHEETS=/usr/share/sgml/docbook/xsl-stylesheets

all: html/index.html

html/index.html: $(DOCFILE)
	@mkdir -p html
	@#xmllint --xinclude --valid --noout $(DOCFILE)
	xsltproc --xinclude -o html/ file://$(STYLESHEETS)/html/chunk.xsl $(DOCFILE)
