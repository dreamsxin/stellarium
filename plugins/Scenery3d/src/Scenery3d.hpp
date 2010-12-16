/*
 * Stellarium-Collada Plug-in
 * 
 * Copyright (C) 2010 Simon Parzer, Gustav Oberwandling, Peter Neubauer
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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _SCENERY3D_HPP_
#define _SCENERY3D_HPP_

#include "StelGui.hpp"
#include "StelModule.hpp"
#include "OBJ.hpp"

#include <QString>

//! Main class of the plug-in
class Scenery3d
{
public:
    Scenery3d();
    virtual ~Scenery3d();

    void load(const QSettings& scenery3dIni, const QString& scenery3dID);
	
    void update(double deltaTime);
    void draw(StelCore* core);

    QString getName() const { return name; }
    QString getAuthorName() const { return authorName; }
    QString getDescription() const { return description; }
    QString getLandscapeName() const { return landscapeName; }
	
private:
    void drawCubeTestScene(StelCore* core);

    Mat4f projectionMatrix;
    float rotation;

    OBJ* objModel;

    Vec4d* vertices; // model vertices
    Vec4d* verticesP; // projected (on-screen) vertices
    Vec4f* texcoords;
    Vec4f* normals;

    QString id;
    QString name;
    QString authorName;
    QString description;
    QString landscapeName;
    QString modelSceneryFile;
    QString modelGroundFile;
};

#endif
