echo 'export PATH="/Users/${whoami}/.brew/opt/qt/bin:$PATH"' >> ~/.zshrc
export LDFLAGS="-L/Users/${whoami}/.brew/opt/qt/lib"
export CPPFLAGS="-I/Users/${whoami}/.brew/opt/qt/include"
export PKG_CONFIG_PATH="/Users/${whoami}/.brew/opt/qt/lib/pkgconfig"
cmake . -Bbuild -Wdev -Werror=dev && cmake --build ./build
