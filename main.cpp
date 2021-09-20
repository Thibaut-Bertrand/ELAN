/*
 *
 *  This file is part of Elan_v2.
 *
 *  Elan_v2 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Elan_v2 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Elan_v2.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Pour pouvoir lancer des commandes à l'application via la console de commande linux (ou tout autre applications en soit)
    // QCoreApplication::arguments();
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/resources/logo.png"));

    MainWindow w;

    w.show();

    return a.exec();
}
