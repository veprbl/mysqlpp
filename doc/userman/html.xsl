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

	<!-- Handle ulinks with type=mysqlapi specially: generate hyperlinks
	     to MySQL reference manual, in the proper form, given only the
			 function name with dashes instead of underscores in the URL. -->
	<xsl:template match="ulink" name="mysqlapiulink">
		<xsl:choose>
			<xsl:when test="@type = 'mysqlapi'">
				<tt>
					<a>
						<xsl:variable name="fn_dash" select="@url"/>
						<xsl:variable name="fn_name"
							select="translate($fn_dash, '-', '_')"/>
						<xsl:attribute name="href">
							<xsl:text>http://dev.mysql.com/doc/mysql/en/</xsl:text>
							<xsl:value-of select="$fn_dash"/>
							<xsl:text>.html</xsl:text>
						</xsl:attribute>
						<xsl:value-of select="$fn_name"/>
						<xsl:text>()</xsl:text>
					</a>
				</tt>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="ulink"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

</xsl:stylesheet>
