/*
example4-1.c
Modifying an existing XML document: create a new node.
*/

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <libxml/xpathInternals.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>



xmlDocPtr
getdoc (char *docname) 
{
	xmlDocPtr doc;
	
	
	doc = xmlParseFile(docname);
	
	if (doc == NULL ) 
	{
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}

	return doc;
}

xmlXPathObjectPtr
getnodeset (xmlDocPtr doc, xmlChar *xpath, xmlChar *ns_prefix[], 
            xmlChar *ns_uri[], int ns_count) 
{
	int i;
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;


	context = xmlXPathNewContext(doc);
	
	if (context == NULL) 
	{
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}
	
	for (i = 0; i < ns_count; i += 1)
	{
	    xmlXPathRegisterNs(context, ns_prefix[i], ns_uri[i]);   
	}
	
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	
	if (result == NULL) 
	{
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	
	if(xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
        printf("No result\n");
		return NULL;
	}
	
	return result;
}

int
main (int argc, char **argv)
{
    xmlXPathObjectPtr result;
    xmlNodePtr room, carpet;
    char *docin, *docout;
    xmlChar *xpath;
    xmlDocPtr doc;
    
    
    docin = "../../data/simple.xml";
    docout = "../../data/out4-1c.xml";
    
    // Parse and show the original document
	doc = getdoc(docin);
	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	
	// Create a new node under 'room' where 'id' = 'bedroom'
	xpath = BAD_CAST "//room[@id=\"bedroom\"]";
	result = getnodeset(doc, xpath, NULL, NULL, 0);
    
    if (result)
    {     
        room = result->nodesetval->nodeTab[0];
        carpet = xmlNewChild(room, NULL, BAD_CAST "carpet", NULL);
        xmlNodeAddContent(carpet, BAD_CAST "a carpet appeared");
    
        xmlXPathFreeObject(result);
    }
	
	// Show and save the modified document
	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	xmlSaveFormatFileEnc(docout, doc, "UTF-8", 1);
	
    xmlFreeDoc(doc);
    xmlCleanupParser();
    
    return(0);
}
