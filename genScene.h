#ifndef GENSCENE_H
#define GENSCENE_H
//
#include "ui_genScene.h"

class SoQtExaminerViewer;
class SoSeparator;

#include <QList>

class SoTabBoxManip;
class SoTransformerManip;

//class SoCube;
//class SoSphere;

class genScene : public QDialog, public Ui::genScene
{
Q_OBJECT
public:
	genScene( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
	void on_sphereButton_clicked();
	void on_cubeButton_clicked();
	void on_coneButton_clicked();
	void on_cylButton_clicked();


	void on_saveButton_clicked();
	void on_loadButton_clicked();
private:
	SoQtExaminerViewer	*m_eView;
	SoSeparator			*m_sceneGraph;
	SoSeparator			*m_sphereGp;
	SoSeparator			*m_cubeGp;
	SoSeparator			*m_coneGp;
	SoSeparator			*m_cylGp;
	
	// list of sphere and cube manips ...
	QList<SoTabBoxManip*>			sphereManips;
	QList<SoTransformerManip*>		cubeManips;
	QList<SoTransformerManip*>		cylManips;
	QList<SoTransformerManip*>		coneManips;
	
//	QList<SoCube*>					cubes;
//	QList<SoSphere*>				spheres;	
};
#endif
