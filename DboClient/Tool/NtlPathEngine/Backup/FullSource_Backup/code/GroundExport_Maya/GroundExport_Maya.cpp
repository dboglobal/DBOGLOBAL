// Copyright (C) 1997-2003 Alias|Wavefront, a division of Silicon Graphics Limited.
// 
// The information in this file is provided for the exclusive use of the
// licensees of Alias|Wavefront.  Such users have the right to use, modify,
// and incorporate this code into other products for purposes authorized
// by the Alias|Wavefront license agreement, without fee.
// 
// ALIAS|WAVEFRONT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
// INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
// EVENT SHALL ALIAS|WAVEFRONT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
// CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
// DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

//
//

//polyRawExporter.cpp
#include <maya/MFnPlugin.h>
#include <maya/MDagPath.h>
#include <maya/MIOStream.h>
#include <fstream.h>

#include "polyRawExporter.h"
#include "polyRawWriter.h"

polyRawExporter::~polyRawExporter() 
{ 
//Summary:  destructor method; does nothing
//
}

     
void* polyRawExporter::creator() 
//Summary:  allows Maya to allocate an instance of this object
{
	return new polyRawExporter();
}


MString polyRawExporter::defaultExtension () const 
//Summary:	called when Maya needs to know the preferred extension of this file
//			format.  For example, if the user tries to save a file called 
//			"test" using the Save As dialog, Maya will call this method and 
//			actually save it as "test.xml". Note that the period should *not* 
//			be included in the extension.
{
	return MString("xml");
}


MStatus initializePlugin(MObject obj)
//Summary:	registers the commands, tools, devices, and so on, defined by the 
//			plug-in with Maya
//Returns:	MStatus::kSuccess if the registration was successful;
//			MStatus::kFailure otherwise
{
	MStatus status;
	MFnPlugin plugin(obj, "PathEngine ground mesh exporter", "4.16.0", "Any");

	// Register the translator with the system
	//
	status =  plugin.registerFileTranslator("PathEngine ground mesh",
											"",
											polyRawExporter::creator,
											"vrml2TranslatorOpts",
											"option1=1",
											true);
	if (!status) {
		status.perror("registerFileTranslator");
		return status;
	}

	return status;
}


MStatus uninitializePlugin(MObject obj) 
//Summary:	deregisters the commands, tools, devices, and so on, defined by the 
//			plug-in
//Returns:	MStatus::kSuccess if the deregistration was successful;
//			MStatus::kFailure otherwise
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status =  plugin.deregisterFileTranslator("PathEngine ground mesh");
	if (!status) {
		status.perror("deregisterFileTranslator");
		return status;
	}

	return status;
}


void polyRawExporter::writeHeader(ostream& os) 
//Summary:	outputs legend information before the main data
//Args   :	os - an output stream to write to
{
//	os << "Legend:\n"
//	   << "Delimiter = TAB\n"
//	   << "() = coordinates\n"
//	   << "[] = vector\n"
//	   << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
    os << "hello from the ground mesh exporter\n";
}


polyWriter* polyRawExporter::createPolyWriter(const MDagPath dagPath, MStatus& status) 
//Summary:	creates a polyWriter for the raw export file type
//Args   :	dagPath - the current polygon dag path
//			status - will be set to MStatus::kSuccess if the polyWriter was 
//					 created successfully;  MStatus::kFailure otherwise
//Returns:	pointer to the new polyWriter object
{
	return new polyRawWriter(dagPath, status);
}




