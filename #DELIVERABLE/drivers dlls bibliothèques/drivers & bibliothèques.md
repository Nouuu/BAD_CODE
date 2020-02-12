# Informations :

- Plateforme : Windows 10
- Langage : C
- Librairies utilisés :
```
<stdio.h> <string.h> <libgen.h> <stdlib.h> <dirent.h> <sqlite3.h> 
<gdk-pixbuf/gdk-pixbuf.h> <gtk-3.0/gtk/gtk.h> <math.h> <windows.h>
```

## Installations des composants nécéssaires :
### 1 - GTK & GTK

#### GTK

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

#### Glade (pour développer uniquement)

##### Installer
[https://ftp.gnome.org/pub/GNOME/binaries/win32/glade/3.8/](https://ftp.gnome.org/pub/GNOME/binaries/win32/glade/3.8/)

### 2 - DLLs & build issue

#### DLLs issue

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