#ifndef GROUND_EXPORT_MAYA_INCLUDED
#define GROUND_EXPORT_MAYA_INCLUDED

#include "polyExporter.h"

class polyRawExporter : public polyExporter {

	public:
								polyRawExporter(){}
		virtual					~polyRawExporter();

		static	void*			creator();
				MString			defaultExtension () const;
//				MStatus			initializePlugin(MObject obj);
//				MStatus			uninitializePlugin(MObject obj);


	private:	
				polyWriter*		createPolyWriter(const MDagPath dagPath, MStatus& status);
				void			writeHeader(ostream& os);
};

#endif /*__POLYEXPORTER_H*/
