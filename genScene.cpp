#include "genScene.h"

#include <QtGui>
#include <QFileDialog> 

#include <fstream>

// Inventor Includes
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/manips/SoTabBoxManip.h>
#include <Inventor/manips/SoTransformerManip.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoPickStyle.h>

//
genScene::genScene( QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);
	SoQt::init(mainFrame);
	
	// Now lets add the examiner viewer ... 
	m_eView = new SoQtExaminerViewer (mainFrame); 

	m_eView->setBackgroundColor(SbColor(0.176f, 0.234f, 0.477f));
	m_eView->setDecoration(false);
	m_eView->setFeedbackVisibility(true);
	// m_eView->setDrawStyle(SoQtViewer::STILL, SoQtViewer::VIEW_WIREFRAME_OVERLAY);
	// m_eView->setDrawStyle(SoQtViewer::INTERACTIVE, SoQtViewer::VIEW_LOW_COMPLEXITY);
	m_eView->setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
	// m_eView->setStereoOffset(200.0f);

	SoBaseColor * col1 = new SoBaseColor;
	col1->rgb = SbColor(1, 1, 0);
	m_cubeGp = new SoSeparator;
	m_cubeGp->addChild(col1);
	m_cubeGp->ref();
	
	SoBaseColor * col2 = new SoBaseColor;
	col2->rgb = SbColor(1, 0, 0);
	m_sphereGp = new SoSeparator;
	m_sphereGp->addChild(col2);	
	m_sphereGp->ref();

	SoBaseColor * col4 = new SoBaseColor;
	col4->rgb = SbColor(0, 0, 1);
	m_cylGp = new SoSeparator;
	m_cylGp->addChild(col4);	
	m_cylGp->ref();

	SoBaseColor * col5 = new SoBaseColor;
	col5->rgb = SbColor(1, 0, 1);
	m_coneGp = new SoSeparator;
	m_coneGp->addChild(col5);	
	m_coneGp->ref();

	m_sceneGraph = new SoSeparator;
	m_sceneGraph->ref();
	m_sceneGraph->addChild(m_cubeGp);
	m_sceneGraph->addChild(m_sphereGp);
	m_sceneGraph->addChild(m_coneGp);
	m_sceneGraph->addChild(m_cylGp);

	SoSeparator *sep = new SoSeparator;
	SoBaseColor * col3 = new SoBaseColor;
	col3->rgb = SbColor(1, 1, 1);
	sep->addChild(col3);
	
	SoDrawStyle *style = new SoDrawStyle; // *)SoNode::getByName("octStyle");
    style->style.setValue(SoDrawStyle::LINES);
	sep->addChild(style);

	SoPickStyle *pickStyle = new SoPickStyle;
	pickStyle->style = SoPickStyle::UNPICKABLE;
	sep->addChild(pickStyle);

	sep->addChild(new SoCube);
	m_sceneGraph->addChild(sep);
	
	m_eView->setSceneGraph(m_sceneGraph);
	m_eView->show();
}
//
void genScene::on_sphereButton_clicked()
{
	SoSeparator *sep = new SoSeparator;
	SoTabBoxManip *manip = new SoTabBoxManip;
	sep->addChild(manip);
	SoSphere *sph = new SoSphere;
	sep->addChild(sph);
	m_sphereGp->addChild(sep);
    sphereManips.append(manip); 
//    spheres.append(sph);
     	
    m_eView->viewAll();
    m_eView->saveHomePosition();

}

void genScene::on_coneButton_clicked() {
	SoSeparator *sep = new SoSeparator;
	SoTransformerManip *manip = new SoTransformerManip;
	sep->addChild(manip);
	SoCone *cone = new SoCone;
	sep->addChild(cone);
	m_coneGp->addChild(sep);
	
	coneManips.append(manip);
	m_eView->viewAll();
    m_eView->saveHomePosition();
}

void genScene::on_cylButton_clicked() {
	SoSeparator *sep = new SoSeparator;
	SoTransformerManip *manip = new SoTransformerManip;
	sep->addChild(manip);
	SoCylinder *cyl = new SoCylinder;
	sep->addChild(cyl);
	m_cylGp->addChild(sep);
	
	cylManips.append(manip);
	m_eView->viewAll();
    m_eView->saveHomePosition();
}

//
void genScene::on_cubeButton_clicked()
{
	SoSeparator *sep = new SoSeparator;
	SoTransformerManip *manip = new SoTransformerManip;
	sep->addChild(manip);
	SoCube *cube = new SoCube;
	sep->addChild(cube);
	m_cubeGp->addChild(sep);
	
	cubeManips.append(manip);
//	cubes.append(cube);
	m_eView->viewAll();
    m_eView->saveHomePosition();
}
//
void genScene::on_saveButton_clicked()
{
	// most important ... write out the file ...
	QString fname = QFileDialog::getSaveFileName ( this, "Select file to save to", QDir::homePath());
	
	std::ofstream out(fname.toLatin1());
	
	unsigned int n = sphereManips.size() + cubeManips.size() + cylManips.size() + coneManips.size();
	out << n << std::endl;		
			
	// write out all spheres ...
	for (int i=0; i<sphereManips.size(); i++) {
		out << "Sphere ";
	  	
	  	SbVec3f translation = sphereManips[i]->translation.getValue();
  		SbRotation rotation = sphereManips[i]->rotation.getValue();
  		SbVec3f scalevector = sphereManips[i]->scaleFactor.getValue();
  		SbRotation scaleorientation = sphereManips[i]->scaleOrientation.getValue();
	  	 	
  		 SbMatrix transformation;
  		 transformation.setTransform(translation, rotation, scalevector, scaleorientation);
  		 
		 SbMatrix invTrans = transformation.inverse();

  		 float *buf = (float *)invTrans;
  		 for (int j=0; j<16; j++)
  		 	out << buf[j] << " ";
  		 out << std::endl;
 	}
	// now all cubes ...
	for (int i=0; i<cubeManips.size(); i++) {
		out << "Cube ";
	  	
	  	SbVec3f translation = cubeManips[i]->translation.getValue();
  		SbRotation rotation = cubeManips[i]->rotation.getValue();
  		SbVec3f scalevector = cubeManips[i]->scaleFactor.getValue();
  		SbRotation scaleorientation = cubeManips[i]->scaleOrientation.getValue();
	  	 	
  		 SbMatrix transformation;
  		 transformation.setTransform(translation, rotation, scalevector, scaleorientation);

  		 SbMatrix invTrans = transformation.inverse();

  		 float *buf = (float *)invTrans;
  		 for (int j=0; j<16; j++)
  		 	out << buf[j] << " ";
  		 out << std::endl;
 	} // for (int i=0; i<cubeManips.size(); i++)
	// Now cones ..
	for (int i=0; i<coneManips.size(); i++) {
		out << "Cone ";
	  	
	  	SbVec3f translation = coneManips[i]->translation.getValue();
  		SbRotation rotation = coneManips[i]->rotation.getValue();
  		SbVec3f scalevector = coneManips[i]->scaleFactor.getValue();
  		SbRotation scaleorientation = coneManips[i]->scaleOrientation.getValue();
	  	 	
  		 SbMatrix transformation;
  		 transformation.setTransform(translation, rotation, scalevector, scaleorientation);
  		 
		 SbMatrix invTrans = transformation.inverse();

  		 float *buf = (float *)invTrans;
  		 for (int j=0; j<16; j++)
  		 	out << buf[j] << " ";
  		 out << std::endl;
 	} // for (int i=0; i<coneManips.size(); i++)
	// finally the cylinders
	for (int i=0; i<cylManips.size(); i++) {
		out << "Cylinder ";
	  	
	  	SbVec3f translation = cylManips[i]->translation.getValue();
  		SbRotation rotation = cylManips[i]->rotation.getValue();
  		SbVec3f scalevector = cylManips[i]->scaleFactor.getValue();
  		SbRotation scaleorientation = cylManips[i]->scaleOrientation.getValue();
	  	 	
  		 SbMatrix transformation;
  		 transformation.setTransform(translation, rotation, scalevector, scaleorientation);
  		 
		 SbMatrix invTrans = transformation.inverse();

  		 float *buf = (float *)invTrans;
  		 for (int j=0; j<16; j++)
  		 	out << buf[j] << " ";
  		 out << std::endl;
 	}
}

void genScene::on_loadButton_clicked() {
  QString fname = QFileDialog::getOpenFileName ( this, "Select file to load", QDir::homePath());

  std::ifstream in (fname.toLatin1());

  int n;
  in >> n;

  char str[256];

  double *mat = new double [16];

  for (int i=0; i<n; i++) {
    in >> str;
    for (int j=0; j<16; j++)
      in >> mat[j];

	if (!strcmp(str, "Cube")) {
		SoSeparator *sep = new SoSeparator;
		SoTransformerManip *manip = new SoTransformerManip;
		SbMatrix invMat(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);
		manip->setMatrix(invMat.inverse());
		sep->addChild(manip);
		SoCube *cube = new SoCube;
		sep->addChild(cube);
		m_cubeGp->addChild(sep);
		cubeManips.append(manip);
	} else if (!strcmp(str, "Sphere")) {
		SoSeparator *sep = new SoSeparator;
		SoTabBoxManip *manip = new SoTabBoxManip;
		SbMatrix invMat(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);
		manip->setMatrix(invMat.inverse());
		sep->addChild(manip);
		SoSphere *sph = new SoSphere;
		sep->addChild(sph);
		m_sphereGp->addChild(sep);
		sphereManips.append(manip); 
	} else if (!strcmp(str, "Cone")) {
		SoSeparator *sep = new SoSeparator;
		SoTransformerManip *manip = new SoTransformerManip;
		SbMatrix invMat(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);
		manip->setMatrix(invMat.inverse());
		sep->addChild(manip);
		SoCone *cone = new SoCone;
		sep->addChild(cone);
		m_coneGp->addChild(sep);
		coneManips.append(manip);
	} else if (!strcmp(str, "Cylinder")) {
		SoSeparator *sep = new SoSeparator;
		SoTransformerManip *manip = new SoTransformerManip;
		SbMatrix invMat(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);
		manip->setMatrix(invMat.inverse());
		sep->addChild(manip);
		SoCylinder *cyl = new SoCylinder;
		sep->addChild(cyl);
		m_cylGp->addChild(sep);
		cylManips.append(manip);
	}

  }
  
  m_eView->viewAll();
  m_eView->saveHomePosition();

}
