echo 'export PATH="/Users/akrasnosel/.brew/opt/qt/bin:$PATH"' >> ~/.zshrc
export LDFLAGS="-L/Users/akrasnosel/.brew/opt/qt/lib"
export CPPFLAGS="-I/Users/akrasnosel/.brew/opt/qt/include"
export PKG_CONFIG_PATH="/Users/akrasnosel/.brew/opt/qt/lib/pkgconfig"
cmake . -Bbuild -Wdev -Werror=dev && cmake --build ./build
