BASENAME=userman
DOCFILE=$(BASENAME).xml

STYLESHEETS=/usr/share/sgml/docbook/xsl-stylesheets

all:
	@xmllint --valid --noout $(DOCFILE)
	$(MAKE) index.html

index.html: $(DOCFILE)
	xsltproc file://$(STYLESHEETS)/html/chunk.xsl $(DOCFILE)
