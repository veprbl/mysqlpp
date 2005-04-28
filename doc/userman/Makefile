BASENAME=userman
DOCFILE=$(BASENAME).xml

STYLESHEETS=/usr/share/sgml/docbook/xsl-stylesheets

all:
	@xmllint --valid --noout $(DOCFILE)
	$(MAKE) index.html

index.html: $(DOCFILE)
	@mkdir -p html
	xsltproc -o html/ file://$(STYLESHEETS)/html/chunk.xsl $(DOCFILE)
