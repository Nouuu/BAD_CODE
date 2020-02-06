# BAD_CODE

[![time tracker](https://wakatime.com/badge/github/Nouuu/BAD_CODE.svg)](https://wakatime.com/badge/github/Nouuu/BAD_CODE)

## Header files

[https://www.tutorialspoint.com/cprogramming/c_header_files.htm](https://www.tutorialspoint.com/cprogramming/c_header_files.htm)

## GTK

[https://www.gtk.org/download/windows.php]() \
[http://www.msys2.org]()

>**Commandes dans le terminal msys2 :**\
>
>`pacman -Syu`\
>`pacman -Syu`\
>`pacman -S mingw-w64-x86_64-gcc`\
>`pacman -S mingw-w64-x86_64-toolchain base-devel`\
>`pacman -S mingw-w64-x86_64-gtk3`\
>`pacman -S mingw-w64-x86_64-glade`

### GTK & Glade

#### Installer
[https://ftp.gnome.org/pub/GNOME/binaries/win32/glade/3.8/](https://ftp.gnome.org/pub/GNOME/binaries/win32/glade/3.8/)

#### Guides
[https://gerald3d.developpez.com/tutoriels/gtk/glade3/](https://gerald3d.developpez.com/tutoriels/gtk/glade3/)\
[https://openclassrooms.com/fr/courses/1343806-pygtk/1343942-glade](https://openclassrooms.com/fr/courses/1343806-pygtk/1343942-glade)\
[https://www.guillaumeriviere.name/estia/DRI/tp1.php](https://www.guillaumeriviere.name/estia/DRI/tp1.php)

### DLLs & build issue
#### ThreeView
[https://linuxfr.org/users/trim/journaux/utilisation-de-gtktreemodel-gtktreeview-et-consorts](https://linuxfr.org/users/trim/journaux/utilisation-de-gtktreemodel-gtktreeview-et-consorts)
[https://gtk.developpez.com/cours/gtk2/?page=page_23](https://gtk.developpez.com/cours/gtk2/?page=page_23)

### DLLs issue

Lier le dossier **msys2/mingw64/bin** aux variables d'environnement
système **PATH**

Lors du build, si certaines librairies sont introuvable, aller dans le
dossier **msys2/mingw64/include** et les copiers coller en dehors de
leur dossiers de version.\
**Exemple :** copier le dossier gtk et gdk se trouvant dans
**msys2/mingw64/include/gtk-3-0/** directement dans
**msys2/mingw64/include** (respecter les chemins introuvables indiqués
dans les messages d'erreur de compilation).\
Repeter autant de fois que nécéssaire.
Lier le dossier **msys2/mingw64/bin** aux variables d'environnement système **PATH**

## SQLite

[http://zetcode.com/db/sqlitec/](http://zetcode.com/db/sqlitec/)