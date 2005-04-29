BASENAME=userman
DOCFILE=$(BASENAME).xml
HTML_SS=html.xsl
FO_SS=fo.xsl
SOURCES=$(DOCFILE) $(STYLESHEET) lgpl.xml

all: html/index.html

pdf:
	xsltproc --xinclude $(FO_SS) $(DOCFILE) > $(BASENAME).fo
	pdfxmltex $(BASENAME).fo 2>&1 > /dev/null
	mv $(BASENAME).pdf ../mysqlpp-$(BASENAME).pdf

html/index.html: $(SOURCES)
	@mkdir -p html
	@#xmllint --xinclude --valid --noout $(DOCFILE)
	xsltproc --xinclude -o html/ $(HTML_SS) $(DOCFILE)
