Aller dans le dossier bulletBuild et unzipper le lib.zip.

Pour vos projets, include path a ajouter:

..\..\..\bulletSource\src


Dans Linker -> General -> Additionnal library dependencies :

..\..\bulletBuild\lib\Debug

Dans Linker -> Input:

BulletDynamics_Debug.lib
BulletCollision_Debug.lib
LinearMath_Debug.lib