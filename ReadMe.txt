#
# PROJECT : COARA_MATH_MOV
#
# GOAL : CONTENTS MAKING TOOL FOR MATH EDUCATION MOVIE 
#
#




# REQUIRES BELOWS:
#               INSTRUCTIONS WHAT I TOOK
# IMGUI 		(CLONED FROM GITHUB, NO MAKE, JUST DOWNLOAD)
#				|
#				- COPIED TO PROJECT'S DIRECTORY AND MAKE PATH
# 				- DELETED UNWANTED FILES (IN backends, example etc.)
#
# GLEW          (CLONED FROM GITHUB, BUILT FROM SOURCE, CMAKE, THEN MAKE INSTALL)
# GLFW          (CLONED FROM GITHUB, BUILT FROM SOURCE, CMAKE, THEN MAKE INSTALL)
# STB           (CLONED FROM GITHUB, NO MAKE, JUST DOWNLOAD)
#               |
#               - COPIED TO PROJECT'S DIRECTORY AND MADE PATH
#               - #define STB_IMAGE_IMPLEMENTATION MACRO IS CRITICAL TO BE IMPLEMENTED!
# 				- DELETED tests FOLDER
#               - DELETED tools FOLDER
# GLM           (brew install glm THEN MADE SYMBOLIC LINK)
#				|
#				- < LINKING GLM DIRECTORY FROM HOMEBREW INSTALLATION TO MY DIRECTORY WANT TO INCLUDE >
#				- ln -s /usr/local/Cellar/glm/0.9.9.8/include/glm /usr/local/include/glm
#
# FREETYPE      (brew install freetype THEN MADE SYMBOLIC LINK)
# 			    |
#				- 
# 				- ln -s /usr/local/Cellar/freetype/2.12.1/include/freetype2/freetype /usr/local/include/freetype
# 				- ln -s /usr/local/Cellar/freetype/2.12.1/include/freetype2/ft2build.h /usr/local/include/ft2build.h
# ZLIB          (brew install zlib)
#   			|
# 				- IT WAS NEEDED FOR OPENCV !
# OPENCV        (CLONED FROM GITHUB, BUILT FROM SOURCE, CMAKE, THEN MAKE INSTALL)
# 				|
# 				- BEFORE .configure WE COMMENTED OUT 'ZLIB_BUILD' BECAUSE IT OCCURED AN ERROR !
#
# SDL3          (CLONED FROM GITHUB, BUILT FROM SOURCE, CMAKE, THEN MAKE INSTALL  <-- READ INSTALL DOC)
# 				|
# SDL_MIXER     (CLONED FROM GITHUB, ./configure -> make -> make install)
# 				|