<?xml version="1.0" encoding="utf-8"?>

<!-- XSL stylesheet containing additions to the standard DocBook
     chunked-HTML stylesheet.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
		version="1.0">
	
	<!-- Import the standard DocBook stylesheet that this one is based on.
	     We use a web URL, but the local XML catalog should resolve this to
			 the local copy of the stylesheet, if it exists. -->
	<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/html/chunk.xsl"/>

	<!-- Bring in local changes common to both HTML and FO output -->
	<xsl:include href="common.xsl"/>

	<!-- HTML-specific XSL parameters -->
	<xsl:param name="use.id.as.filename" select="1"/>
	<xsl:param name="chunk.fast" select="1"/>
	<xsl:param name="section.autolabel" select="1"/>
	<xsl:param name="section.autolabel.max.depth" select="2"/>
</xsl:stylesheet>
