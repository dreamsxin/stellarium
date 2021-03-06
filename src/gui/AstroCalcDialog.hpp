/*
 * Stellarium
 * 
 * Copyright (C) 2015 Alexander Wolf
 * Copyright (C) 2016 Nick Fedoseev (visualization of ephemeris)
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
*/

#ifndef _ASTROCALCDIALOG_HPP_
#define _ASTROCALCDIALOG_HPP_

#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QVector>
#include <QTimer>

#include "StelDialog.hpp"
#include "StelCore.hpp"
#include "Planet.hpp"
#include "SolarSystem.hpp"
#include "Nebula.hpp"
#include "NebulaMgr.hpp"
#include "StarMgr.hpp"

class Ui_astroCalcDialogForm;
class QListWidgetItem;

class AstroCalcDialog : public StelDialog
{
	Q_OBJECT

public:
	//! Defines the number and the order of the columns in the table that lists planetary positions
	//! @enum PlanetaryPositionsColumns
	enum PlanetaryPositionsColumns {
		ColumnName,		//! name of object
		ColumnRA,		//! right ascension
		ColumnDec,		//! declination
		ColumnMagnitude,	//! magnitude
		ColumnType,		//! type of object
		ColumnCount		//! total number of columns
	};

	//! Defines the number and the order of the columns in the ephemeris table
	//! @enum EphemerisColumns
	enum EphemerisColumns {
		EphemerisDate,		//! date and time of ephemeris
		EphemerisJD,		//! JD
		EphemerisRA,		//! right ascension
		EphemerisDec,		//! declination
		EphemerisMagnitude,	//! magnitude
		EphemerisCount		//! total number of columns
	};

	//! Defines the number and the order of the columns in the phenomena table
	//! @enum PhenomenaColumns
	enum PhenomenaColumns {
		PhenomenaType,		//! type of phenomena
		PhenomenaDate,		//! date and time of ephemeris
		PhenomenaObject1,	//! first object
		PhenomenaObject2,	//! second object
		PhenomenaSeparation,	//! angular separation
		PhenomenaCount		//! total number of columns
	};

	AstroCalcDialog(QObject* parent);
	virtual ~AstroCalcDialog();

	//! Notify that the application style changed
	void styleChanged();

	static QVector<Vec3d> EphemerisListJ2000;
	static QVector<QString> EphemerisListDates;
	static int DisplayedPositionIndex;

public slots:
        void retranslate();

protected:
        //! Initialize the dialog widgets and connect the signals/slots.
        virtual void createDialogContent();
        Ui_astroCalcDialogForm *ui;

private slots:
	//! Search planetary positions and fill the list.
	void currentPlanetaryPositions();
	void selectCurrentPlanetaryPosition(const QModelIndex &modelIndex);
	void onChangedEphemerisPosition(const QModelIndex &modelIndex);

	//! Calculate ephemeris for selected celestial body and fill the list.
	void generateEphemeris();
	void cleanupEphemeris();
	void selectCurrentEphemeride(const QModelIndex &modelIndex);
	void saveEphemeris();

	//! Calculate phenomena for selected celestial body and fill the list.
	void calculatePhenomena();
	void selectCurrentPhenomen(const QModelIndex &modelIndex);
	void savePhenomena();

	void drawAltVsTimeDiagram();
	void drawCurrentTimeDiagram();
	void mouseOverLine(QMouseEvent *event);

	void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
	class StelCore* core;
	class SolarSystem* solarSystem;
	class NebulaMgr* dsoMgr;
	class StarMgr* starMgr;
	class StelObjectMgr* objectMgr;
	QTimer *currentTimeLine;

	//! Update header names for planetary positions table
	void setPlanetaryPositionsHeaderNames();
	//! Update header names for ephemeris table
	void setEphemerisHeaderNames();
	//! Update header names for phenomena table
	void setPhenomenaHeaderNames();

	//! Init header and list of planetary positions
	void initListPlanetaryPositions();
	//! Init header and list of ephemeris
	void initListEphemeris();
	//! Init header and list of phenomena
	void initListPhenomena();

	//! Populates the drop-down list of celestial bodies.
	//! The displayed names are localized in the current interface language.
	//! The original names are kept in the user data field of each QComboBox
	//! item.
	void populateCelestialBodyList();
	//! Populates the drop-down list of time steps.
	void populateEphemerisTimeStepsList();
	//! Populates the drop-down list of major planets.
	void populateMajorPlanetList();
	//! Populates the drop-down list of groups of celestial bodies.
	void populateGroupCelestialBodyList();	
	//! Prepare graph settings
	void prepareAxesAndGraph();

	//! Calculation conjunctions and oppositions.
	//! @note Ported from KStars, should be improved, because this feature calculate
	//! angular separation ("conjunction" defined as equality of right ascension
	//! of two body) and current solution is not accurate and slow.
	QMap<double, double> findClosestApproach(PlanetP& object1, PlanetP& object2, double startJD, double stopJD, double maxSeparation, bool opposition);
	double findDistance(double JD, PlanetP object1, PlanetP object2, bool opposition);
	bool findPrecise(QPair<double, double>* out, PlanetP object1, PlanetP object2, double JD, double step, int prevSign, bool opposition);
	void fillPhenomenaTable(const QMap<double, double> list, const PlanetP object1, const PlanetP object2, bool opposition);

	QMap<double, double> findClosestApproach(PlanetP& object1, NebulaP& object2, double startJD, double stopJD, double maxSeparation);
	double findDistance(double JD, PlanetP object1, NebulaP object2);
	bool findPrecise(QPair<double, double>* out, PlanetP object1, NebulaP object2, double JD, double step, int prevSign);
	void fillPhenomenaTable(const QMap<double, double> list, const PlanetP object1, const NebulaP object2);

	QMap<double, double> findClosestApproach(PlanetP& object1, StelObjectP& object2, double startJD, double stopJD, double maxSeparation);
	double findDistance(double JD, PlanetP object1, StelObjectP object2);
	bool findPrecise(QPair<double, double>* out, PlanetP object1, StelObjectP object2, double JD, double step, int prevSign);
	void fillPhenomenaTable(const QMap<double, double> list, const PlanetP object1, const StelObjectP object2);

	QString delimiter, acEndl;
	QStringList ephemerisHeader, phenomenaHeader, planetaryPositionsHeader;
	static float brightLimit;
	static float minY, maxY;

	//! Make sure that no tabs icons are outside of the viewport.
	//! @todo Limit the width to the width of the screen *available to the window*.
	void updateTabBarListWidgetWidth();
};

// Reimplements the QTreeWidgetItem class to fix the sorting bug
class ACTreeWidgetItem : public QTreeWidgetItem
{
public:
	ACTreeWidgetItem(QTreeWidget* parent)
		: QTreeWidgetItem(parent)
	{
	}

private:
	bool operator < (const QTreeWidgetItem &other) const
	{
		int column = treeWidget()->sortColumn();

		if (column == AstroCalcDialog::ColumnMagnitude)
		{
			return text(column).toFloat() < other.text(column).toFloat();
		}
		else
		{
			return text(column).toLower() < other.text(column).toLower();
		}
	}
};

#endif // _ASTROCALCDIALOG_HPP_
