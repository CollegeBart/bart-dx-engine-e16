Aller dans le dossier bulletBuild et unzipper le lib.zip.

Pour vos projets, include path a ajouter:

..\..\..\bart-dx-engine-e16\bulletSource\src


Dans Linker -> General -> Additionnal library dependencies :

..\..\..\bart-dx-engine-e16\bulletBuild\lib\Debug

Dans Linker -> Input:

BulletDynamics_Debug.lib
BulletCollision_Debug.lib
LinearMath_Debug.lib


Use x86